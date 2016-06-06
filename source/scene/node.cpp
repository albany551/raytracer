
#include <scene/node.hpp>

using namespace scene;

Node::Node() :
	parent(0),
	child(0),
	next(0),
	previous(0),
	type(0)
{
}

Node::~Node()
{
    Detach();
    while (child != 0) 
        child->Detach();
}
       
void Node::AttachTo(Node* parent)
{
	CORE_ASSERT(parent);
	CORE_ASSERT(parent->parent != this);
	CORE_ASSERT(parent != this);
	Detach();

	this->next = parent->child;
	if(this->next) 
		this->next->previous = this;

	this->parent = parent;
	this->parent->child = this;   
}

void Node::Detach()
{
    if(!this->parent) return;

	Ref<Node> thisnode = this;
	
	if (this->parent->child == this)
	{
		// this is first child
		CORE_ASSERT(!this->previous);
		this->parent->child = this->next;
		if(this->next)
			this->next->previous = 0;
	}
	else
	{
		// this is not first child
		CORE_ASSERT(this->previous);
		if (this->previous)
			this->previous->next = this->next;
		if (this->next)
			this->next->previous = this->previous;
	}

	this->parent = 0;
	this->next = 0;
	this->previous = 0; 
}

Node* Node::GetRoot() const
{
	Node* root = const_cast<Node*>(this);
	while(root->GetParent())
		root = root->GetParent();
	return root;
}

Node* Node::GetNext() const
{
	if(this->child) return this->child;
    if(this->next) return this->next;

	for(Node* node = this->parent; node; node = node->parent)
		if(node->next)
			return node->next;

	return 0;
}

bool Node::HasFlag(Flags f) const
{
	return (type & f) > 0;
}

void Node::SetFlags(uint16 value)
{
	type = value;
}

