
#ifndef SCENE_SPHERENODE_HPP
#define SCENE_SPHERENODE_HPP

#include <scene/visual.hpp>

namespace gfx
{
	class Context;
}

namespace scene
{
	using gfx::Context;

    class SphereNode : public Visual
    {
        public:

		SphereNode();
        
		void	Render(Context* context, Camera* camera, int priority = 0);

        void    SetRadius(float radius) { this->radius = radius; }
        float   GetRadius() { return this->radius; }
        
        private:
        
        float radius;
    };
    
}

#endif
