
#include <scene/light.hpp>

using namespace scene;

Light::Light() :
	color(1.f,1.f,1.f)
{
	SetFlags(Node::LIGHT | Node::TRANSFORM);
}

void Light::SetColor(const vector3f& value)
{
	color = value;
}
