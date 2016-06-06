
#include <scene/transformnode.hpp>

using namespace scene;

TransformNode::TransformNode() :
	xformdirty(true),
	rotation(0,0,0,1.0),
	position(vector3f(0.0, 0.0, 0.0)),
	scale(vector3f(1.0, 1.0, 1.0))
{
	SetFlags(Node::TRANSFORM);
}
       
void TransformNode::SetPosition(const vector3f& value)
{
	position = value;
	xformdirty = true;
}

void TransformNode::SetScale(const vector3f& value)
{
	scale = value;
	xformdirty = true;
}


void TransformNode::LookAt(const vector3f& target, const vector3f& up)
{
    matrix44f viewmat;
    viewmat.SetView(target, GetWorldTransform().GetTranslation(), up);
	rotation = quat4f(viewmat);
	xformdirty = true;
}

void TransformNode::LookAt(TransformNode* target, const vector3f& up)
{
    LookAt(target->GetWorldTransform().GetTranslation(),up);
}

const vector3f& TransformNode::GetPosition() const
{
	return position;
}

const quat4f& TransformNode::GetRotation() const
{
	return rotation;
}

const vector3f& TransformNode::GetScale() const
{
	return scale;
}

vector3f TransformNode::GetDirection() const
{
    return rotation.GetDirection();
}

matrix44f TransformNode::GetTransform()
{
	// scale * rotation * translation

	if(xformdirty)
	{
		QuaternionToMatrix(rotation, xformcache);
		xformcache.SetTranslation(position);
		xformdirty = false;
	}

	return xformcache;
}

matrix44f TransformNode::GetWorldTransform()
{
    matrix44f world = GetTransform();
	for(Node* node = parent; node != 0; node = node->parent)
	{
		if(node->HasFlag(Node::TRANSFORM))
		{
			TransformNode* tnode = static_cast<TransformNode*>(node);
			world = tnode->GetTransform() * world;
		}
	}
	return world;
}


