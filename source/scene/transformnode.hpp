
#ifndef SCENE_TRANSFORMNODE_HPP
#define SCENE_TRANSFORMNODE_HPP

#include <scene/node.hpp>
#include <math/matrix44.hpp>
#include <math/quaternion.hpp>

namespace scene
{
    class TransformNode : public Node
    {
        public:
          
        TransformNode();

        void        SetPosition(const vector3f& value);
		void        SetScale(const vector3f& value);

        void        LookAt(const vector3f& target, const vector3f& up = vector3f(0,1,0));
        void        LookAt(TransformNode* target, const vector3f& up = vector3f(0,1,0));  

        const vector3f&		GetPosition() const;
		const quat4f&		GetRotation() const;
		const vector3f&		GetScale() const;
        vector3f	GetDirection() const;
        matrix44f	GetTransform();
        matrix44f	GetWorldTransform();
  
	
        private: 

		matrix44f	xformcache;
		bool		xformdirty;

		quat4f		rotation;
		vector3f	position;
		vector3f	scale;
    };
}

#endif
