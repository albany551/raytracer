
#ifndef RAYTRACE_RAYTRACER_HPP
#define RAYTRACE_RAYTRACER_HPP

#include <gfx/context.hpp>
#include <scene/camera.hpp>

namespace raytrace
{
	using gfx::Context;
	using scene::Camera;

	class Raytracer
    {
        public:

		enum AntiAlias
		{
			NONE,
			SUPERSAMPLE2X,
			ADAPTIVE
		};

		Raytracer();
		~Raytracer();

		// void SetParams(Camera* camera, int width, int height, AntiAlias mode = NONE);
		// int width, height
		// float3* framebuffer


        void	SetParams(Camera* camera, const char* path, int w, int h, AntiAlias aa = NONE);
		bool	Render();

		// number of rays traced by Render
		int		GetRayCount() const;

		void	RenderDebugInfo(Camera* camera, Context* context);

		private:

		vector3f* buffer;

    };  

} // namespace raytrace

#endif
