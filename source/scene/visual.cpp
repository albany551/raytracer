
#include <scene/visual.hpp>

using namespace scene;

Visual::Visual() :
	diffuse(1.0, 1.0, 1.0, 1.0),
	specular(1.0, 1.0, 1.0, 0.0),
	reflective(0.0),
	refractive(0.0),
	refractiveindex(0.0)
{
	SetFlags(Node::VISUAL | Node::TRANSFORM);

}

void Visual::SetShader(Shader* fx)
{
	shader = fx;
}

void Visual::SetTexture(Shader::Param param, Texture* texture)
{
	switch(param)
	{
	case Shader::DIFFMAP0:
		diffmap0 = texture;
		break;
	}
}

void Visual::SetLocalBox(const box3f& box)
{
	boundbox = box;
}

Shader*	Visual::GetShader()
{
	return shader;
}

Texture* Visual::GetTexture(Shader::Param param)
{
	switch(param)
	{
	case Shader::DIFFMAP0:
		return diffmap0;
	}

	return 0;
}

box3f Visual::GetWorldBox()
{
	matrix44f worldtm = GetWorldTransform();

	// transform the 8 corners of the local box to world space
	vector3f corners[8];
	boundbox.GetPoints(corners, worldtm);

	// now calculate an AABB to fit all the corners
	box3f result;
	for(int i=0; i<8; i++)
		result.FitPoint(corners[i]);

	return result;
}
