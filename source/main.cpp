
#include <core/timer.hpp>
#include <core/window.hpp>
#include <core/keyboard.hpp>
#include <core/mouse.hpp>

#include <core/hashmap.hpp>

#include <scene/meshnode.hpp>
#include <scene/light.hpp>
#include <scene/camera.hpp>
#include <raytrace/raytracer.hpp>
#include <raytrace/spherenode.hpp>

#include <gfx/opengl/glcontext.hpp>
#include <gfx/texture.hpp>
#include <gfx/shader.hpp>

using namespace core;
using namespace gfx;
using namespace scene;
using namespace raytrace;

/*

	gfx/
		- Context
			create textures, meshes, shaders
			viewport settings
			begin/end scene
		- Canvas
			Has an array of display lists
			Render(context*) method draws all display lists in sequence, then deletes them
			can call context->SetRenderTarget() beforehand to render on overlay onto a texture

			Ref<Canvas> canvas = context->CreateCanvas();
			{
				Canvas::Draw draw(canvas); // performs Begin() and End() methods exception-safe, starts a new display list
				canvas->DrawLine(x1..
			}
			overlay->Render(context);

			

			- Shader	 


 GfxResource : Resource
 {
	void AddToContext(bool deletelocal = false);
	void RemoveFromContext();
 }


TODO:

*/
/*
#include <core/thread.hpp>

class RaytraceThread : public Thread
{
	public:

	RaytraceThread(Camera* value)
	{
		camera = value;
	}

	void Run()
	{
		Printf("Beginning ray trace...\n");
		Raytracer raytracer;
		raytracer.Render(camera, "test.bmp", 256, 256, Raytracer::ADAPTIVE);
		Printf("Done!\n");
	}

	Camera* camera;
};
*/

class AppWindow : public Window
{
	private:

	Ref<Context> context;

	Ref<Node> scene;
	Ref<Camera> camera;

	Keyboard keyboard;
	Mouse mouse;
	Raytracer raytracer;

	public:

	AppWindow()
	{
		Open(512,512,"Amphora",false);

		context = new GlContext(this);
		Ref<Shader> shd1 = context->CreateShader("base");
		Ref<Texture> texture1 = context->CreateTexture("data/test.tga");
		Ref<Texture> texture2 = context->CreateTexture("data/eagle.tga");
		texture1->Load();
		texture2->Load();

		scene = new Node();       	
		
		camera = new Camera();
		camera->SetHorizontalFov(45);
		camera->SetPosition(vector3f(-.3,.9f,3));
		camera->LookAt(vector3f(0,0,0));	
		camera->AttachTo(scene);

		Ref<SphereNode> sphere1 = new SphereNode();
		sphere1->SetPosition(vector3f(0,.5,-4));
		sphere1->SetRadius(.7f);
		//sphere1->SetDiffuse(vector4f(0.2f, 0.6f, 0.1f, 1.f));
		sphere1->SetShader(shd1);
		sphere1->SetTexture(Shader::DIFFMAP0, texture1);
		sphere1->AttachTo(scene);

		Ref<Mesh> mesh2 = context->CreateMesh("data/floor.smf");
		mesh2->Load();
		Ref<MeshNode> meshnode2 = new MeshNode();
		meshnode2->SetMesh(mesh2);
		meshnode2->SetPosition(vector3f(0,0,.5));
		meshnode2->SetDiffuse(vector4f(0.2f, 0.3f, 0.8f, .7f));
		meshnode2->SetReflective(1.f);
		meshnode2->SetShader(shd1);
		meshnode2->AttachTo(scene);

		Ref<Mesh> mesh1 = context->CreateMesh("data/eagle.nvx2");
		mesh1->Load();
		Ref<MeshNode> meshnode1 = new MeshNode();
		meshnode1->SetMesh(mesh1);
		meshnode1->SetPosition(vector3f(.0f,.4f,0));
		//meshnode1->SetDiffuse(vector4f(.5f, .1f, .4f, 1.0));
		meshnode1->SetShader(shd1);
		meshnode1->SetTexture(Shader::DIFFMAP0, texture2);
		meshnode1->AttachTo(scene);

		Ref<Light> light2 = new Light();
		light2->SetPosition(vector3f(-1.5,2,.5));
		light2->SetColor(vector3f(1.0,1.0,1.0));
		light2->AttachTo(scene);

	}

	~AppWindow()
	{
	}

	void Update(float dt)
	{
		camera->Render(context);

		raytracer.RenderDebugInfo(camera, context);

		context->Present();

		//float yaw = .01f * mouse.GetDeltaX();
		//float pitch = .01f * mouse.GetDeltaY();
		//camera->Rotate(vector3f(pitch, 0.0, 0.0));

		//Printf("%f, %f\n", yaw, pitch);
		//quat_t pitch(vec3_t(1,0,0),pitch_angle);
		//quat_t yaw(vec3_t(0,1,0),yaw_angle);

	//rot = rot * pitch * yaw;
	//LookAt(pos+rot.dir());	// this gets rid of the roll


		
		if(keyboard.GetKey(Keyboard::KEY_W))
		{
			vector3f campos = camera->GetPosition();
			vector3f camdir = camera->GetDirection();
			camera->SetPosition(campos + 10 * dt * camdir);
		}
		if(keyboard.GetKey(Keyboard::KEY_S))
		{
			vector3f campos = camera->GetPosition();
			vector3f camdir = camera->GetDirection();
			camera->SetPosition(campos - 10 * dt * camdir);
		}
		if(keyboard.GetKey(Keyboard::KEY_A))
		{
			vector3f campos = camera->GetPosition();
			vector3f camdir = CrossProduct(camera->GetDirection(), vector3f(0.0, 1.0, 0.0));
			camera->SetPosition(campos - 10 * dt * camdir);
		}
		if(keyboard.GetKey(Keyboard::KEY_D))
		{
			vector3f campos = camera->GetPosition();
			vector3f camdir = CrossProduct(camera->GetDirection(), vector3f(0.0, 1.0, 0.0));
			camera->SetPosition(campos + 10 * dt * camdir);
		}

		if(keyboard.GetKey(Keyboard::KEY_SPACE))
		{
			Printf("Beginning ray trace...\n");
			
			Timer timer;
			raytracer.SetParams(camera, "test.bmp", 512, 512, Raytracer::ADAPTIVE);
			raytracer.Render();

			Printf("Done! %f second.\n", timer.GetTime());
		}

		if(keyboard.GetKey(Keyboard::KEY_ESC))
			Quit();

	}

};


int main(int argc, char *argv[])
{
	try
	{
		//CORE_THROW("hey!");
		AppWindow window;
		window.Run();
	}
	catch (Exception& e)
	{
		e.Display();
	}

    return 0;
}
