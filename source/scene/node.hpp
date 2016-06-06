
#ifndef SCENE_NODE_HPP
#define SCENE_NODE_HPP

#include <core/object.hpp>

namespace scene
{
	using core::Object;
	using core::Ref;

	class Node : public Object
    {
        public:

		// ====================================
		// types
		// ====================================

		enum Flags
		{
			TRANSFORM	= (1<<0),
			VISUAL			= (1<<1),
			CAMERA			= (1<<2),
			LIGHT			= (1<<3),
			SHAPENODE		= (1<<4),
			SPHERENODE		= (1<<5),
			SCENE			= (1<<10)
		};
            
        Node();
        virtual ~Node();   

		// ====================================
		// scene graph
		// ====================================

        void        AttachTo(Node* parent);
        void        Detach();
        Node*       GetParent() const { return parent; }
        Node*       GetFirstChild() const { return child; } 
        Node*       GetNextChild(const Node* child) const  { return child->next; } 
        Node*       GetRoot() const;
        Node*       GetNext() const;

		bool		HasFlag(Flags f) const;

		protected:
		
		void		SetFlags(uint16 value);
		
        private: 
            
        Ref<Node>   child;
        Ref<Node>   next;
        Node*       parent;
        Node*       previous;

		uint16		type;

		friend class TransformNode;

    };
}

#endif
