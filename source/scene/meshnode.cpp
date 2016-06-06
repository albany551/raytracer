
#include <scene/meshnode.hpp>
#include <scene/camera.hpp>

using namespace gfx;
using namespace scene;

MeshNode::MeshNode() :
	group(0)
{
	SetFlags(Node::SHAPENODE | Node::VISUAL | Node::TRANSFORM);
}

#include <gfx/opengl/gl.hpp>

void MeshNode::Render(Context* context, Camera* camera, int priority)
{
	Shader* fx = GetShader();
	if(!fx) return;

	fx->SetParam(Shader::PROJECTION, camera->GetCachedProjTransform());
	fx->SetParam(Shader::VIEW, camera->GetCachedViewTransform() * GetWorldTransform());

	vector4f diffuse = GetDiffuse();
	glColor4f(diffuse.r, diffuse.g, diffuse.b, diffuse.a);

	fx->Begin();
	mesh->Render(group);
	fx->End();

	// bound box


	vector3f boxmin = GetLocalBox().vmin;
	vector3f boxmax = GetLocalBox().vmax;
	glColor3f(0.0, 1.0, 0.0);

	glBegin(GL_LINES);

		glVertex3f(boxmin.x, boxmin.y, boxmin.z); // E
		glVertex3f(boxmin.x, boxmax.y, boxmin.z); // F

		glVertex3f(boxmax.x, boxmin.y, boxmax.z); // D
		glVertex3f(boxmax.x, boxmax.y, boxmax.z); // C

		glVertex3f(boxmax.x, boxmin.y, boxmin.z); // A
		glVertex3f(boxmax.x, boxmax.y, boxmin.z); // B

		glVertex3f(boxmin.x, boxmin.y, boxmax.z); // H
		glVertex3f(boxmin.x, boxmax.y, boxmax.z); // G

		glVertex3f(boxmax.x, boxmin.y, boxmax.z); // D
		glVertex3f(boxmax.x, boxmin.y, boxmin.z); // A

		glVertex3f(boxmin.x, boxmin.y, boxmax.z); // H
		glVertex3f(boxmin.x, boxmin.y, boxmin.z); // E

		glVertex3f(boxmax.x, boxmax.y, boxmax.z); // C
		glVertex3f(boxmax.x, boxmax.y, boxmin.z); // B

		glVertex3f(boxmin.x, boxmax.y, boxmax.z); // G
		glVertex3f(boxmin.x, boxmax.y, boxmin.z); // F

		glVertex3f(boxmin.x, boxmin.y, boxmax.z); // H
		glVertex3f(boxmax.x, boxmin.y, boxmax.z); // D

		glVertex3f(boxmin.x, boxmin.y, boxmin.z); // E
		glVertex3f(boxmax.x, boxmin.y, boxmin.z); // A

		glVertex3f(boxmin.x, boxmax.y, boxmin.z); // F
		glVertex3f(boxmax.x, boxmax.y, boxmin.z); // B

		glVertex3f(boxmin.x, boxmax.y, boxmax.z); // G
		glVertex3f(boxmax.x, boxmax.y, boxmax.z); // C

	glEnd();

}

void MeshNode::SetMesh(Mesh* m)
{
	mesh = m;
	SetLocalBox(box3f(m->GetBoundMin(), m->GetBoundMax()));
}

