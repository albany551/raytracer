
#include <gfx/opengl/glcontext.hpp>

#include <core/window.hpp>
#include <gfx/opengl/gl.hpp>
#include <gfx/opengl/glmesh.hpp>
#include <gfx/opengl/gltexture.hpp>
#include <gfx/opengl/glshader.hpp>


using namespace gfx;

static ContextHandle CreateContext(WindowHandle& handle)
{
	ContextHandle context;

#ifdef CORE_PLATFORM_WIN32

	context.hwnd = handle.hwnd;

	int bits=32;
	unsigned pf;
	static PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if(!(context.hdc=GetDC(context.hwnd)))
		CORE_THROW("Couldn't create an OpenGL device context");

	if(!(pf=ChoosePixelFormat(context.hdc,&pfd)))
		CORE_THROW("Couldn't find a suitable pixel format.");

	if(!SetPixelFormat(context.hdc,pf,&pfd))
		CORE_THROW("Couldn't set the pixel format.");

	if(!(context.hrc=wglCreateContext(context.hdc)))
		CORE_THROW("Couldn't create a rendering context");

	if(!wglMakeCurrent(context.hdc,context.hrc))
		CORE_THROW("Couldn't activate the rendering context");

#endif

	glLoadExtensions();

	return context;
}

static void DeleteContext(ContextHandle& handle)
{

#ifdef CORE_PLATFORM_WIN32

	if(!wglMakeCurrent(NULL,NULL))
		CORE_THROW("Release Of DC And RC Failed.");

	if(!wglDeleteContext(handle.hrc))
		CORE_THROW("Release Rendering Context Failed.");

	if(handle.hdc && !ReleaseDC(handle.hwnd, handle.hdc))
		CORE_THROW("Release Device Context Failed.");

	handle.hrc;
	handle.hdc;

#endif

}

static bool ExtensionSupported(const char *extension)
{
	char* extStr = (char*)glGetString(GL_EXTENSIONS);
	if (extStr)
	{
		size_t len = strlen(extension);
		while ((extStr = strstr(extStr, extension)) != 0)
		{
			extStr += len;
			if (*extStr == ' ' || *extStr == '\0') return true;
		}
	}
	return false;
}


GlContext::GlContext(Window* window) :
	Context(window)
{
	handle = CreateContext(window->GetHandle());
	
	if(!ExtensionSupported("GL_ARB_vertex_buffer_object"))
		CORE_THROW("GL_ARB_vertex_buffer_object not supported by device");

	if(!ExtensionSupported("GL_ARB_fragment_program"))
		CORE_THROW("GL_ARB_fragment_program not supported by device");

	if(!ExtensionSupported("GL_ARB_fragment_shader"))
		CORE_THROW("GL_ARB_fragment_shader not supported by device");

	if(!ExtensionSupported("GL_ARB_texture_compression"))
		CORE_THROW("GL_ARB_texture_compression not supported by device");

	// get some info about the device
	//glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, (int*)&mTextureUnits);
	//glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &mMaxAnisotropy);
	//if(GL_ARB_draw_buffers_supported) glGetIntegerv(GL_MAX_DRAW_BUFFERS_ARB, (int*)&mMaxDrawBuffers);

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 1.f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	glViewport(0, 0, window->GetWidth(), window->GetHeight());
	glScissor(0, 0, window->GetWidth(), window->GetHeight());

/*
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping
	mFont = glGenLists(96);								// Storage For 96 Characters
	font = CreateFont(-12,								// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_NORMAL,//FW_BOLD,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						"Courier New");					// Font Name

	oldfont = (HFONT)SelectObject(mHandle.hdc, font);
	wglUseFontBitmaps(mHandle.hdc, 32, 96, mFont);
	SelectObject(mHandle.hdc, oldfont);
	DeleteObject(font);
*/

	//DebugPrint((char*)glGetString(GL_EXTENSIONS));
}

GlContext::~GlContext()
{
	DeleteContext(handle);
}


void GlContext::Present()
{

#ifdef CORE_PLATFORM_WIN32
	SwapBuffers(handle.hdc);
#endif

	// move these elsewhere
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_TRUE);
}

void GlContext::SetViewport(int x0, int y0, int x1, int y1)
{
	glViewport(x0, y0, x1, y1);
}

Mesh* GlContext::CreateMesh(const char* name)
{
	GlMesh* mesh = new GlMesh();
	mesh->name = name;
	return mesh;
}

Texture* GlContext::CreateTexture(const char* name)
{
	GlTexture* texture = new GlTexture();
	texture->name = name;
	return texture;
}

Shader*	GlContext::CreateShader(const char* name)
{
	GlShader* shader = new GlShader();
	shader->name = name;
	return shader;
}

