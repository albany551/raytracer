
#include <gfx/opengl/gl.hpp>

// ========================================
// opengl extensions
// ========================================

#ifdef CORE_PLATFORM_LINUX
	#define wglGetProcAddress(a) glXGetProcAddressARB((const GLubyte *) (a))
#endif

// GL_ARB_shader_objects
PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
PFNGLGETHANDLEARBPROC glGetHandleARB;
PFNGLDETACHOBJECTARBPROC glDetachObjectARB;
PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
PFNGLVALIDATEPROGRAMARBPROC glValidateProgramARB;
PFNGLUNIFORM1FARBPROC glUniform1fARB;
PFNGLUNIFORM2FARBPROC glUniform2fARB;
PFNGLUNIFORM3FARBPROC glUniform3fARB;
PFNGLUNIFORM4FARBPROC glUniform4fARB;
PFNGLUNIFORM1IARBPROC glUniform1iARB;
PFNGLUNIFORM2IARBPROC glUniform2iARB;
PFNGLUNIFORM3IARBPROC glUniform3iARB;
PFNGLUNIFORM4IARBPROC glUniform4iARB;
PFNGLUNIFORM1FVARBPROC glUniform1fvARB;
PFNGLUNIFORM2FVARBPROC glUniform2fvARB;
PFNGLUNIFORM3FVARBPROC glUniform3fvARB;
PFNGLUNIFORM4FVARBPROC glUniform4fvARB;
PFNGLUNIFORM1IVARBPROC glUniform1ivARB;
PFNGLUNIFORM2IVARBPROC glUniform2ivARB;
PFNGLUNIFORM3IVARBPROC glUniform3ivARB;
PFNGLUNIFORM4IVARBPROC glUniform4ivARB;
PFNGLUNIFORMMATRIX2FVARBPROC glUniformMatrix2fvARB;
PFNGLUNIFORMMATRIX3FVARBPROC glUniformMatrix3fvARB;
PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fvARB;
PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfvARB;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
PFNGLGETATTACHEDOBJECTSARBPROC glGetAttachedObjectsARB;
PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;
PFNGLGETACTIVEUNIFORMARBPROC glGetActiveUniformARB;
PFNGLGETUNIFORMFVARBPROC glGetUniformfvARB;
PFNGLGETUNIFORMIVARBPROC glGetUniformivARB;
PFNGLGETSHADERSOURCEARBPROC glGetShaderSourceARB;

// GL_ARB_vertex_buffer_object
PFNGLBINDBUFFERARBPROC glBindBufferARB;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;
PFNGLGENBUFFERSARBPROC glGenBuffersARB;
PFNGLISBUFFERARBPROC glIsBufferARB;
PFNGLBUFFERDATAARBPROC glBufferDataARB;
PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB;
PFNGLGETBUFFERSUBDATAARBPROC glGetBufferSubDataARB;
PFNGLMAPBUFFERARBPROC glMapBufferARB;
PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB;
PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB;
PFNGLGETBUFFERPOINTERVARBPROC glGetBufferPointervARB;


void glLoadExtensions()
{
	glDeleteObjectARB        = (PFNGLDELETEOBJECTARBPROC)        wglGetProcAddress("glDeleteObjectARB");
	glGetHandleARB           = (PFNGLGETHANDLEARBPROC)           wglGetProcAddress("glGetHandleARB");
	glDetachObjectARB        = (PFNGLDETACHOBJECTARBPROC)        wglGetProcAddress("glDetachObjectARB");
	glCreateShaderObjectARB  = (PFNGLCREATESHADEROBJECTARBPROC)  wglGetProcAddress("glCreateShaderObjectARB");
	glShaderSourceARB        = (PFNGLSHADERSOURCEARBPROC)        wglGetProcAddress("glShaderSourceARB");
	glCompileShaderARB       = (PFNGLCOMPILESHADERARBPROC)       wglGetProcAddress("glCompileShaderARB");
	glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC) wglGetProcAddress("glCreateProgramObjectARB");
	glAttachObjectARB        = (PFNGLATTACHOBJECTARBPROC)        wglGetProcAddress("glAttachObjectARB");
	glLinkProgramARB         = (PFNGLLINKPROGRAMARBPROC)         wglGetProcAddress("glLinkProgramARB");
	glUseProgramObjectARB    = (PFNGLUSEPROGRAMOBJECTARBPROC)    wglGetProcAddress("glUseProgramObjectARB");
	glValidateProgramARB     = (PFNGLVALIDATEPROGRAMARBPROC)     wglGetProcAddress("glValidateProgramARB");
	glUniform1fARB = (PFNGLUNIFORM1FARBPROC) wglGetProcAddress("glUniform1fARB");
	glUniform2fARB = (PFNGLUNIFORM2FARBPROC) wglGetProcAddress("glUniform2fARB");
	glUniform3fARB = (PFNGLUNIFORM3FARBPROC) wglGetProcAddress("glUniform3fARB");
	glUniform4fARB = (PFNGLUNIFORM4FARBPROC) wglGetProcAddress("glUniform4fARB");
	glUniform1iARB = (PFNGLUNIFORM1IARBPROC) wglGetProcAddress("glUniform1iARB");
	glUniform2iARB = (PFNGLUNIFORM2IARBPROC) wglGetProcAddress("glUniform2iARB");
	glUniform3iARB = (PFNGLUNIFORM3IARBPROC) wglGetProcAddress("glUniform3iARB");
	glUniform4iARB = (PFNGLUNIFORM4IARBPROC) wglGetProcAddress("glUniform4iARB");
	glUniform1fvARB = (PFNGLUNIFORM1FVARBPROC) wglGetProcAddress("glUniform1fvARB");
	glUniform2fvARB = (PFNGLUNIFORM2FVARBPROC) wglGetProcAddress("glUniform2fvARB");
	glUniform3fvARB = (PFNGLUNIFORM3FVARBPROC) wglGetProcAddress("glUniform3fvARB");
	glUniform4fvARB = (PFNGLUNIFORM4FVARBPROC) wglGetProcAddress("glUniform4fvARB");
	glUniform1ivARB = (PFNGLUNIFORM1IVARBPROC) wglGetProcAddress("glUniform1ivARB");
	glUniform2ivARB = (PFNGLUNIFORM2IVARBPROC) wglGetProcAddress("glUniform2ivARB");
	glUniform3ivARB = (PFNGLUNIFORM3IVARBPROC) wglGetProcAddress("glUniform3ivARB");
	glUniform4ivARB = (PFNGLUNIFORM4IVARBPROC) wglGetProcAddress("glUniform4ivARB");
	glUniformMatrix2fvARB = (PFNGLUNIFORMMATRIX2FVARBPROC) wglGetProcAddress("glUniformMatrix2fvARB");
	glUniformMatrix3fvARB = (PFNGLUNIFORMMATRIX3FVARBPROC) wglGetProcAddress("glUniformMatrix3fvARB");
	glUniformMatrix4fvARB = (PFNGLUNIFORMMATRIX4FVARBPROC) wglGetProcAddress("glUniformMatrix4fvARB");
	glGetObjectParameterfvARB = (PFNGLGETOBJECTPARAMETERFVARBPROC) wglGetProcAddress("glGetObjectParameterfvARB");
	glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC) wglGetProcAddress("glGetObjectParameterivARB");
	glGetInfoLogARB           = (PFNGLGETINFOLOGARBPROC)           wglGetProcAddress("glGetInfoLogARB");
	glGetAttachedObjectsARB   = (PFNGLGETATTACHEDOBJECTSARBPROC)   wglGetProcAddress("glGetAttachedObjectsARB");
	glGetUniformLocationARB   = (PFNGLGETUNIFORMLOCATIONARBPROC)   wglGetProcAddress("glGetUniformLocationARB");
	glGetActiveUniformARB     = (PFNGLGETACTIVEUNIFORMARBPROC)     wglGetProcAddress("glGetActiveUniformARB");
	glGetUniformfvARB         = (PFNGLGETUNIFORMFVARBPROC)         wglGetProcAddress("glGetUniformfvARB");
	glGetUniformivARB         = (PFNGLGETUNIFORMIVARBPROC)         wglGetProcAddress("glGetUniformivARB");
	glGetShaderSourceARB      = (PFNGLGETSHADERSOURCEARBPROC)      wglGetProcAddress("glGetShaderSourceARB");

	glBindBufferARB    = (PFNGLBINDBUFFERARBPROC)    wglGetProcAddress("glBindBufferARB");
	glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress("glDeleteBuffersARB");
	glGenBuffersARB    = (PFNGLGENBUFFERSARBPROC)    wglGetProcAddress("glGenBuffersARB");
	glIsBufferARB      = (PFNGLISBUFFERARBPROC)      wglGetProcAddress("glIsBufferARB");
	glBufferDataARB       = (PFNGLBUFFERDATAARBPROC)       wglGetProcAddress("glBufferDataARB");
	glBufferSubDataARB    = (PFNGLBUFFERSUBDATAARBPROC)    wglGetProcAddress("glBufferSubDataARB");
	glGetBufferSubDataARB = (PFNGLGETBUFFERSUBDATAARBPROC) wglGetProcAddress("glGetBufferSubDataARB");
	glMapBufferARB   = (PFNGLMAPBUFFERARBPROC)   wglGetProcAddress("glMapBufferARB");
	glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC) wglGetProcAddress("glUnmapBufferARB");
	glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC) wglGetProcAddress("glGetBufferParameterivARB");
	glGetBufferPointervARB    = (PFNGLGETBUFFERPOINTERVARBPROC)    wglGetProcAddress("glGetBufferPointervARB");
}



