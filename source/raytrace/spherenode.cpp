
#include <raytrace/spherenode.hpp>
#include <scene/camera.hpp>
#include <math/sphere.hpp>
#include <math/intersect.hpp>

using namespace scene;

SphereNode::SphereNode()
{
	SetFlags(Node::SPHERENODE | Node::VISUAL | Node::TRANSFORM);
}

#include <gfx/opengl/gl.hpp>

void SphereNode::Render(Context* context, Camera* camera, int priority)
{
	Shader* fx = GetShader();
	if(!fx) return;

	fx->SetParam(Shader::PROJECTION, camera->GetCachedProjTransform());
	fx->SetParam(Shader::VIEW, camera->GetCachedViewTransform() * GetWorldTransform());

	fx->Begin();

	glBegin(GL_POLYGON);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(-0.5, -0.5, 0.0);
		glColor3f(0.0, 1.0, 1.0);
		glVertex3f(-0.5, 0.5, 0.0);
		glColor3f(1.0, 0.0, 1.0);
		glVertex3f(0.5, 0.5, 0.0);
		glColor3f(1.0, 1.0, 0.0);
		glVertex3f(0.5, -0.5, 0.0);
	glEnd();

	fx->End();
}
