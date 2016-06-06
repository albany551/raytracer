
#include <raytrace/raytracer.hpp>

#include <core/array.hpp>
#include <core/timer.hpp>
#include <math/intersect.hpp>
#include <scene/light.hpp>
#include <scene/visual.hpp>
#include <raytrace/hitinfo.hpp>

#include <gfx/texture.hpp>
#include <gfx/opengl/gl.hpp>


using namespace core;
using namespace gfx;
using namespace math;
using namespace scene;
using namespace raytrace;

// cached info for rendering
static Array<Visual*>	visualnodes;
static Array<matrix44f>	visualtransforms;
static Array<box3f>		visualboxes;
static Array<Light*>	lightnodes;
static Array<matrix44f>	lighttransforms;

// image plane
static vector3f origin;
static vector3f p;
static vector3f incrementx;
static vector3f	incrementy;

int width = 0;
int height = 0;
bool setparams = false;
bool postprocess = false;
Raytracer::AntiAlias mode = Raytracer::NONE;


Array<ray3f> debugrays;

// count of rays traced through scene
static int raycount;

// max trace depth
static int tracedepth = 2;

Raytracer::Raytracer() :
	buffer(0)

{

}

Raytracer::~Raytracer()
{
	delete[] buffer;
}

static vector3f TraceRay(const ray3f& ray, int depth = 0, float refraction = 1.0)
{
	vector3f color(0,0,0);

	raycount++;
	HitInfo intersection;

	// find closest intersection (if any)
	for(int i=0; i<visualnodes.GetSize(); i++)
	{
		// test for intersection with AABB
		if(!IntersectRayBox(ray, visualboxes[i]))
			continue;

		FindCloserIntersection(intersection, ray, visualtransforms[i], visualnodes[i]);
	}

	if(!intersection.success)
		return color;

	for(int i=0; i<lightnodes.GetSize(); i++)
	{
		// check for blocking object, this will give shadows
		float strength = 1.f;

		vector3f L = lighttransforms[i].GetTranslation() - intersection.point;	// light vector

		ray3f shadow(intersection.point + (.001f * Normalize(L)), lighttransforms[i].GetTranslation());

		for(int j=0; j<visualnodes.GetSize(); j++)
		{
			HitInfo shadowint;
			if(FindCloserIntersection(shadowint, shadow, visualtransforms[j], visualnodes[j]))
			{
				if(shadowint.distance > 0.001f && shadowint.distance < Length(L))
				{
					strength = 0.1f;
				}
			}
		}
	
		L.Normalize();
		vector3f Il = lightnodes[i]->GetColor() * strength;					// light source intensity
		vector3f N = intersection.normal;						// surface normal
		vector4f C = intersection.visual->GetDiffuse();		// surface color
		float cosT = DotProduct(L,N);							// angle between normal and light direction
		
		vector3f Ia(1.0, 1.0, 1.0);						// ambient color
		float Ka = 0.3f;								// ambient coefficient
		float Kd = C.a;		// diffuse coefficient
		vector3f V = ray.direction;						// ray (view) direction
		vector3f R = L - 2.0f * cosT * N;				// reflected ray (L reflected over N )
		float cosP = DotProduct(V,R);					// angle between view and reflection direction
		float Ks = intersection.visual->GetSpecular().a;	// specular alpha
		float n = 10;									// specular exponent
		
		color3f texcolor(1,1,1);
		Texture* texture = intersection.visual->GetTexture(Shader::DIFFMAP0);
		if(texture)
		{
			float u = intersection.texcoord.u;
			float v = intersection.texcoord.v;
			texcolor = texture->GetTexel(u, v);
		}

		color += (Ia * Ka * C  * texcolor);						// ambient lighting
		if(cosT > 0) color += (Il * Kd * cosT * C * texcolor);	// diffuse (Lambert) lighting
		if(cosP > 0) color += (Il * Ks * math::Pow(cosP, n) * C);	// specular (Phong) lighting
	}

	
	float refl = intersection.visual->GetReflective();
	if (refl > 0.0f && depth <= tracedepth) 
	{
		ray3f reflray;
		vector3f N = intersection.normal;
		vector3f R = ray.direction - 2.0f * DotProduct(ray.direction, N) * N;
		reflray.origin = intersection.point + R * 0.001f; //math::EPSILON;
		reflray.direction = R;
		//debugrays.PushBack(reflray);

		vector3f reflcolor = TraceRay(reflray, depth+1);
		color += refl * reflcolor * intersection.visual->GetDiffuse();
	}

	float refr = intersection.visual->GetRefractive();
	if (refr > 0.0f && depth <= tracedepth) 
	{
		float rindex = intersection.visual->GetRefractiveIndex();
		float n = refraction / rindex;
		vector3f N = intersection.normal; // negate if intersection inside primitive
		if(intersection.inside) N = -N;
		float cosI = -DotProduct(N, ray.direction);
		float cosT2 = 1.0f - n * n * (1.f - cosI * cosI);
		if(cosT2 > 0.0f)
		{
			vector3f T = Normalize((n * ray.direction) + ((n * cosI - Sqrt(cosT2)) * N));

			ray3f refrray;
			refrray.origin = intersection.point + T * 0.001f; //math::EPSILON;
			refrray.direction = T;
			vector3f refrcolor = TraceRay(refrray, depth+1, rindex);

			//debugrays.PushBack(refrray);

			color += refr * refrcolor * intersection.visual->GetDiffuse();
		}
	}

	// saturate color values 
	color.r = math::Min(color.r, 1.f);
	color.g = math::Min(color.g, 1.f);
	color.b = math::Min(color.b, 1.f);
    return color;
}

static vector3f RecurseAdaptive(const vector3f& origin, const vector3f& p, 
							 vector3f tl, vector3f tr, vector3f br, vector3f bl, int level = 0)
{
	static float tolerance = .1f;

	if(level > 2) 
		return (tl + tr + br + bl) / 4;

	if(	WithinTolerance(tl,tr,tolerance) &&
		WithinTolerance(tr,br,tolerance) && 
		WithinTolerance(br,bl,tolerance) &&
		WithinTolerance(bl,tl,tolerance))
	{
		return (tl + tr + br + bl) / 4;
	}


	/*
      tl--A--tr
	  |   |   |
	  B---C---D
      |   |   |
	  bl--E--br

	  we need to calculate five new colors: A, B, C, D, and E
	  then we will make 4 recursive calls, one for each sub-pixel
		and return the average of these calls as the final color

	*/

	float inc = .5f;
	int divide = level;
	while(divide--)
		inc = inc / 2.f;

	// positions in world coords (along the image plane) of each point of the sub-pixels
	vector3f ptl = p;
	vector3f ptr = p + (2*inc)*incrementx;
	vector3f pbr = p + (2*inc)*incrementx - (2*inc)*incrementy;
	vector3f pbl = p - (2*inc)*incrementy; 
	vector3f pA = p + inc*incrementx;
	vector3f pB = p - inc*incrementy;
	vector3f pC = p + inc*incrementx - inc*incrementy;
	vector3f pD = p + (2*inc)*incrementx - inc*incrementy;
	vector3f pE = p + inc*incrementx - (2*inc)*incrementy;

	// get the 5 new colors by tracing additional rays
	vector3f A = TraceRay(ray3f(origin,pA));
	vector3f B = TraceRay(ray3f(origin,pB));
	vector3f C = TraceRay(ray3f(origin,pC));
	vector3f D = TraceRay(ray3f(origin,pD));
	vector3f E = TraceRay(ray3f(origin,pE));

	// now recurse for each subpixel
	vector3f c1 = RecurseAdaptive(origin, ptl, tl, A, C, B, level+1);
	vector3f c2 = RecurseAdaptive(origin, pA, A, tr, D, C, level+1);
	vector3f c3 = RecurseAdaptive(origin, pC, C, D, br, E, level+1);
	vector3f c4 = RecurseAdaptive(origin, pB, B, C, E, bl, level+1);

	// return the average of the subpixels
	return (c1 + c2 + c3 + c4) / 4;
}


void Raytracer::SetParams(Camera* camera, const char* path, int w, int h, AntiAlias aa)
{
	//Timer timer;
	raycount = 0;
	mode = aa;
	width = w;
	height = h;

	visualnodes.Clear();
	visualtransforms.Clear();
	visualboxes.Clear();
	lightnodes.Clear();
	lighttransforms.Clear();

	// go through the scene graph and cache stuff
	Node* root = camera->GetRoot();
	for(Node* node = root; node; node=node->GetNext())
	{
		if(node->HasFlag(Node::VISUAL))
		{
			Visual* visual = static_cast<Visual*>(node);
			visualnodes.PushBack(visual);
			visualtransforms.PushBack(visual->GetWorldTransform());
			visualboxes.PushBack(visual->GetWorldBox());
		}
		else if(node->HasFlag(Node::LIGHT))
		{
			Light* light = static_cast<Light*>(node);
			lightnodes.PushBack(light);
			lighttransforms.PushBack(light->GetWorldTransform());
		}
	}

	if(mode == ADAPTIVE)
	{
		width++;
		height++;
	}
	else if(mode == SUPERSAMPLE2X)
	{
		width *= 2;
		height *= 2;
	}
   
    // calculate image plane stuff
    float d = 10;     // distance from camera to plane (n'importe quoi...)        
    float sj = 2 * d * math::Tan(math::ToRadians(camera->GetHorizontalFov() / 2));	// width of plane
    float sk = sj * ((float)height/(float)width);	// height
	origin = camera->GetWorldTransform().GetTranslation(); 
    vector3f dirz = camera->GetWorldTransform().GetDirection();	// forward vector
    vector3f diry = vector3f(0,1,0);    // up vector
    vector3f dirx = Normalize(CrossProduct(dirz,diry));    // side vector
    p = origin + (d * dirz) - ((sj/2) * dirx) + ((sk/2) * diry);	// upper-left pixel of image plane
	incrementx = (sj*(1.f/(width-1))*dirx);	// one-pixel increment in x direction
	incrementy = (sk*(1.f/(height-1))*diry);	// one-pixel increment in y direction

	delete[] buffer;
	buffer = new vector3f[width * height];

	setparams = true;
}

bool Raytracer::Render()
{
	if(!setparams)
		return false;

	Timer timer;

    // now cast the rays!
    // start from camera origin and go through each pixel in image plane

	for(int y = 0; y < height; y++)
		for(int x = 0; x < width; x++)
		{
			vector3f target = p + (x*incrementx) - (y*incrementy);
			buffer[y * width + x] = TraceRay(ray3f(origin,target));	// (height-y-1) * width + x
		}

	if(mode == ADAPTIVE)
	{
		// create a new frame buffer for adaptive anti-aliasing
		vector3f* newbuffer = new vector3f[(width-1)*(height-1)];
		
		for(int y = 0; y < height-1; y++)
			for(int x = 0; x < width-1; x++)
			{
				vector3f tl = buffer[(y+0) * width + (x+0)];	// top left
				vector3f tr = buffer[(y+0) * width + (x+1)];	// top right
				vector3f br = buffer[(y+1) * width + (x+1)];	// bottom right
				vector3f bl = buffer[(y+1) * width + (x+0)];	// bottom left

				vector3f target = p + ((float)x*incrementx) - ((float)y*incrementy);
				newbuffer[y * (width-1) + x] = RecurseAdaptive(origin, target, tl, tr, br, bl);
			}

		delete[] buffer;
		buffer = newbuffer;
		width--;
		height--;
		
	}

	Texture texture;
	texture.SetPixels(width, height, buffer);

	if(mode == SUPERSAMPLE2X)
	{
		texture.Downsample();
	}

	texture.SaveBMP("test.bmp");

	setparams = false;
	return true;
}

int	Raytracer::GetRayCount() const
{
	return raycount;
}

static void RenderRays(Array<ray3f>& rays)
{
	glBegin(GL_LINES);
	for(int i=0; i< rays.GetSize(); i++)
	{
		vector3f a = rays[i].origin;
		//vector3f b = rays[i].ray.origin + rays[i].length * rays[i].ray.direction;
		vector3f b = rays[i].origin + rays[i].direction;
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(b.x, b.y, b.z);
	}
	glEnd();
}

void Raytracer::RenderDebugInfo(Camera* camera, Context* context)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(camera->GetCachedProjTransform().m16);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(camera->GetCachedViewTransform().m16);
	//glMultMatrixf(GetWorldTransform().m16);

	glDisable(GL_LIGHTING);
	glColor3f(1.0, 0.0, 0.0);


	RenderRays(debugrays);

	glEnable(GL_LIGHTING);

	/*
	if(buffer)
	{

    glDrawPixels(width, 
              height, 
              GL_RGB, 
              GL_FLOAT, 
              buffer);

	}
	*/

}