
#include <raytrace/hitinfo.hpp>
#include <raytrace/spherenode.hpp>
#include <math/intersect.hpp>
#include <scene/meshnode.hpp>

using namespace gfx;
using namespace scene;
using namespace raytrace;

static bool IntersectShapeNode(const matrix44f& worldtm, const ray3f& ray, float& enter, vector3f& normal, vector2f& texcoord, MeshNode* shape)
{
	Mesh* mesh = shape->GetMesh();
	if(!mesh)
		return false;

	float t = math::INFINITY, baryu = 0.f, baryv = 0.f;

	vector3f normal0, normal1, normal2;
	vector2f tex0, tex1, tex2;
	bool hasuv = false;

	
	uint32* indices = mesh->GetIndexDataPtr();
	float* vertices = mesh->GetVertexDataPtr() + mesh->GetVertexFormat().GetOffset(VertexFormat::COORD);
	float* normals = mesh->GetVertexDataPtr() + mesh->GetVertexFormat().GetOffset(VertexFormat::NORMAL);
	float* texcoords = mesh->GetVertexDataPtr() + mesh->GetVertexFormat().GetOffset(VertexFormat::UV0);
	int stride = mesh->GetVertexFormat().GetWidth();

	// for each face in the mesh
	for(int i = 0; i+3<=mesh->GetIndexCount(); i+=3)
	{
		uint32 index0 = *(indices + i + 0);
		uint32 index1 = *(indices + i + 1);
		uint32 index2 = *(indices + i + 2);

		vector3f v0 = worldtm * vector3f(vertices[index0 * stride + 0], vertices[index0 * stride + 1], vertices[index0 * stride + 2]);
		vector3f v1 = worldtm * vector3f(vertices[index1 * stride + 0], vertices[index1 * stride + 1], vertices[index1 * stride + 2]);
		vector3f v2 = worldtm * vector3f(vertices[index2 * stride + 0], vertices[index2 * stride + 1], vertices[index2 * stride + 2]);

		float res = math::INFINITY, bu, bv;
		if(IntersectRayTriangle(ray, v0, v1, v2, res, bu, bv))
		{
			if(res < t)
			{
				// this is the closet intersection so save some info
				t = res;
				normal0 = worldtm * vector3f(normals[index0 * stride + 0], normals[index0 * stride + 1], normals[index0 * stride + 2]);
				normal1 = worldtm * vector3f(normals[index1 * stride + 0], normals[index1 * stride + 1], normals[index1 * stride + 2]);
				normal2 = worldtm * vector3f(normals[index2 * stride + 0], normals[index2 * stride + 1], normals[index2 * stride + 2]);
				baryu = bu;
				baryv = bv;

				if(mesh->GetVertexFormat().Contains(VertexFormat::UV0))
				{
					tex0 = vector2f(texcoords[index0 * stride + 0], texcoords[index0 * stride + 1]);
					tex1 = vector2f(texcoords[index1 * stride + 0], texcoords[index1 * stride + 1]);
					tex2 = vector2f(texcoords[index2 * stride + 0], texcoords[index2 * stride + 1]);
					hasuv = true;
				}
			}
		}
	}

	if(t != math::INFINITY)
	{
		enter = t;

		// flat shading
		//normal = Normalize((normal0 + normal1 + normal2) / 3) ;

		// gouraud shading
		normal = Normalize(normal0 + ((normal1 - normal0) * baryu) + ((normal2 - normal0 ) * baryv));

		if(hasuv)
			texcoord = tex0 + ((tex1 - tex0) * baryu) + ((tex2 - tex0 ) * baryv);

		return true;
	}

	return false;
}


static bool IntersectSphereNode(const matrix44f& worldtm, const ray3f& ray, float& enter, vector3f& normal, SphereNode* sphere)
{
	// get our sphere's origin in world coords
    vector3f origin = worldtm.GetTranslation();

	// do ray-sphere intersection
	float t;
	if(IntersectRaySphere(ray, sphere3f(origin, sphere->GetRadius()), t, normal))
	{
		enter = t;
		return true;
	}

	return false;
}

bool raytrace::FindCloserIntersection(HitInfo& result, const ray3f& ray, const matrix44f& worldtm, Visual* visual)
{
	float distance;
	vector3f normal, point;
	vector2f uv(0,0);
	bool inside = false;

	if(visual->HasFlag(Node::SHAPENODE))
	{
		MeshNode* shape = static_cast<MeshNode*>(visual);

		if(!IntersectShapeNode(worldtm, ray, distance, normal, uv, shape))
			return false;

		point = ray.origin + distance * ray.direction;
		result.texcoord = uv;
	}
	else if(visual->HasFlag(Node::SPHERENODE))
	{
		SphereNode* sphere = static_cast<SphereNode*>(visual);

		if(!IntersectSphereNode(worldtm, ray, distance, normal, sphere))
			return false;

		result.texcoord.u = math::ATan(normal.x, normal.z) / (2.0 * math::PI) + .5;
		result.texcoord.v = math::ASin(-normal.y) / math::PI + .5;

		point = ray.origin + distance * ray.direction;
		float length = Length(point - (worldtm * sphere->GetPosition()));
		if(length < sphere->GetRadius())
			inside = true;
	}
	else
	{
		return false;
	}

	result.success = true;

	if(distance < result.distance)
	{
		result.distance = distance;
		result.point = point;
		result.normal = normal;
		result.visual = visual;
		result.inside = inside;
	}

	return true;
}
