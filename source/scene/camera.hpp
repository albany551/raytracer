
#ifndef SCENE_CAMERA_HPP
#define SCENE_CAMERA_HPP

#include <scene/transformnode.hpp>

namespace gfx
{
	class Context;
}

namespace scene
{
	using gfx::Context;

    class Camera : public TransformNode
    {
        public:

		Camera();
        
        void    SetHorizontalFov(float value);
		void	SetFront(float value);
		void	SetBack(float value);

		void	Render(Context* context);

        float			GetHorizontalFov() const;
		const matrix44f& GetCachedViewTransform() const;
		const matrix44f& GetCachedProjTransform() const; 
        
        private:

		matrix44f	cachedview;
		matrix44f	cachedproj;
        
        float   fov;   
    };

} // namespace scene

#endif
