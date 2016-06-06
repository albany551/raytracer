
#include <gfx/opengl/glshader.hpp>
#include <gfx/opengl/gl.hpp>

using namespace gfx;

static const char* vertexshader_basic = "					\
void main()													\
{															\
gl_FrontColor = gl_Color;									\
gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;		\
}															\
";

static const char* fragmentshader_basic = "					\
void main()													\
{															\
gl_FragColor = gl_Color;					\
}															\
";

// gl_FragColor = gl_Color;					\


// =================================================
// opengl shader implementation
// =================================================

GlShader::GlShader()
{
	vshandle = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	fshandle = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	handle = glCreateProgramObjectARB();

	glShaderSourceARB(vshandle, 1, &vertexshader_basic, NULL);
	glShaderSourceARB(fshandle, 1, &fragmentshader_basic, NULL);

	glCompileShaderARB(vshandle);
	glCompileShaderARB(fshandle);
	
	glAttachObjectARB(handle,vshandle);
	glAttachObjectARB(handle,fshandle);

	glLinkProgramARB(handle);
	
}

GlShader::~GlShader()
{
	glDetachObjectARB(handle,vshandle);
	glDeleteObjectARB(vshandle);
	glDetachObjectARB(handle,fshandle);
	glDeleteObjectARB(fshandle);
	glDeleteObjectARB(handle);
}

int GlShader::Begin()
{
	glUseProgramObjectARB(handle);
	return 0;
}

void GlShader::End()
{
}

void GlShader::SetParam(Param p, bool val)
{
}

void GlShader::SetParam(Param p, int val)
{
}

void GlShader::SetParam(Param p, float val)
{
}

void GlShader::SetParam(Param p, const vector3f& val)
{
}

void GlShader::SetParam(Param p, const matrix44f& val)
{
	if(p == Shader::VIEW)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrixf(val.m16);
	}
	else if(p== Shader::PROJECTION)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glLoadMatrixf(val.m16);
	}
}

void GlShader::SetParam(const char* name, bool val)
{

}

void GlShader::SetParam(const char* name, int val)
{
}

void GlShader::SetParam(const char* name, float val)
{
}

void GlShader::SetParam(const char* name, const vector3f& val)
{
}

void GlShader::SetParam(const char* name, const matrix44f& val)
{
	int location = glGetUniformLocationARB(handle, name);
	CORE_ASSERT(location >= 0);
	glUniformMatrix4fvARB(location, 1, GL_TRUE, val.m16);
}



