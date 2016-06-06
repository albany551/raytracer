
#ifndef GFX_CONTEXT_HPP
#define GFX_CONTEXT_HPP

#include <core/object.hpp>

namespace core
{
	class Window;
}

namespace gfx
{
	using core::Object;
	using core::Window;

	class Context : public Object
	{
		public:

		Context(Window* window);
		virtual ~Context();

		// swap back buffer to screen and clear viewport
		virtual void Present() = 0;

		virtual void SetViewport(int x0, int y0, int x1, int y1) = 0;

		// create a shared mesh object
		virtual class Mesh* CreateMesh(const char* name) { return 0; }
		// create a shared texture object
		virtual class Texture* CreateTexture(const char* name) { return 0; }
		// create a shared shader object
		virtual class Shader* CreateShader(const char* name) { return 0; }

		

		protected:

		ContextHandle handle;
		
	};


} // namespace gfx


#endif
