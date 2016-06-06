
#include <gfx/shader.hpp>

using namespace gfx;


void Shader::SetVertexShader(const char* source)
{
	sourcevs = source;
}

void Shader::SetFragmentShader(const char* source)
{
	sourcefs = source;
}
