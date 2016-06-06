
#ifndef GFX_OPENGL_GLCONTEXT_HPP
#define GFX_OPENGL_GLCONTEXT_HPP

#include <gfx/context.hpp>

namespace gfx
{
	class GlContext : public Context
	{
		public:

		GlContext(core::Window* window);
		virtual ~GlContext();

		void		Present();


		Shader*		CreateShader(const char* name);
		Texture*	CreateTexture(const char* name);
		Mesh*		CreateMesh(const char* name);

		void		SetViewport(int x0, int y0, int x1, int y1);

	};


} // namespace gfx


#endif
