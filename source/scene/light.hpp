
#ifndef SCENE_LIGHT_HPP
#define SCENE_LIGHT_HPP

#include <scene/transformnode.hpp>

namespace scene
{
    class Light : public TransformNode
    {
        public:

		Light();

		void			SetColor(const vector3f& value);
		const vector3f&	GetColor() const { return color; }

		private:

		vector3f		color;
        
    };
}

#endif
