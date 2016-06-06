
#include <scene/camera.hpp>
#include <scene/meshnode.hpp>

#include <gfx/opengl/gl.hpp>

using namespace gfx;
using namespace scene;

Camera::Camera() :
	fov(45.0)
{
	SetFlags(Node::CAMERA | Node::TRANSFORM);
}

void Camera::SetHorizontalFov(float value)
{
	fov = value;
}

void Camera::Render(Context* context)
{
	float aspect = 1.0f;
	cachedproj.SetPerspective(fov, aspect, 0.1f, 100.0f);
	cachedview = GetWorldTransform().Inverse();

	Node* root = GetRoot();
	for(Node* node = root; node; node=node->GetNext())
	{
		if(node->HasFlag(Node::VISUAL))
		{
			Visual* visual = static_cast<Visual*>(node);
			visual->Render(context, this);
		}
		else if(node->HasFlag(Node::LIGHT))
		{
			// render a light shape
		}
	}

}

float Camera::GetHorizontalFov() const
{
	return fov;
}

const matrix44f& Camera::GetCachedViewTransform() const
{
	return cachedview;
} 

const matrix44f& Camera::GetCachedProjTransform() const
{
	return cachedproj;
} 
