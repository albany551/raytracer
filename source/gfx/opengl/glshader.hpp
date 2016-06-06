
#ifndef GFX_OPENGL_GLSHADER_HPP
#define GFX_OPENGL_GLSHADER_HPP

#include <core/common.hpp>
#include <gfx/shader.hpp>

namespace gfx
{
	class GlShader : public Shader
	{
		public:

		GlShader();
		~GlShader();

		int Begin();
		void End();

		void SetVertexShader(const char* source);
		void SetFragmentShader(const char* source);

		void SetParam(Param p, bool val);
		void SetParam(Param p, int val);
		void SetParam(Param p, float val);
		void SetParam(Param p, const vector3f& val);
		void SetParam(Param p, const matrix44f& val);

		void SetParam(const char* name, bool val);
		void SetParam(const char* name, int val);
		void SetParam(const char* name, float val);
		void SetParam(const char* name, const vector3f& val);
		void SetParam(const char* name, const matrix44f& val);

		protected:

		uint32 vshandle;
		uint32 fshandle;
		uint32 handle;
	};

} // namespace gfx

#endif
