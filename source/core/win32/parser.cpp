
#include <core/parser.hpp>
#include <core/common.hpp>
#include <math/common.hpp>

using namespace core;

Parser::Parser()
{
}

Parser::Parser(const char* buffer, const int size)
{
	Start(buffer, size);
}

void Parser::Start(const char* buff, const int size)
{
	buffer=(char*)buff;
	this->line=0;

	if(size==0)
	{
		buf_end=buff+strlen((char*)buff);
	}
	else
	{
		buf_end=buff+size;
	}
}

bool Parser::Advance()
{
	char    *token_p;

	if(!buffer) return 0;
	if(buffer==buf_end) return 0;

	*token=0;

skipspace:	// skip whitespace

	while (*buffer <= 32)
	{
		if(*buffer=='\n') line++;
		if (buffer >= buf_end) return 0;
		buffer++;
	}

	if (buffer >= buf_end) return 0;

	// comments preceded by #
	if (*buffer == '#')
	{
		while (*buffer++ != '\n')
			if (buffer >= buf_end) return 0;
		line++;
		goto skipspace;
	}

	token_p = token;

	while ( *buffer > 32 && *buffer != '\n')
	{
		*token_p++ = *buffer++;
		if (buffer == buf_end) break;
		if (token_p == &token[MAX_TOKEN_LENGTH]) break;
	}


	*token_p = 0;

	return 1;
}

const float fast_atof_table[] =
{
	0.f,
	0.1f,
	0.01f,
	0.001f,
	0.0001f,
	0.00001f,
	0.000001f,
	0.0000001f,
	0.00000001f,
	0.000000001f,
	0.0000000001f,
	0.00000000001f,
	0.000000000001f,
	0.0000000000001f,
	0.00000000000001f,
	0.000000000000001f
};

//! Provides a fast function for converting a string into a f32,
//! about 6 times faster than atof in win32.
// If you find any bugs, please send them to me, niko (at) code3d.com.
inline char* fast_atof_move(char* c, float& out)
{
	bool inv = false;
	char *t;
	float f;

	if (*c=='-')
	{
		c++;
		inv = true;
	}

	f = (float)strtol(c, &t, 10);
	c = t;

	if (*c == '.')
	{
		c++;
		float pl = (float)strtol(c, &t, 10);
		pl *= fast_atof_table[t-c];
		f += pl;
		c = t;

		if (*c == 'e')
		{
			++c;
			float exp = (float)strtol(c, &t, 10);
			f *= (float)math::Pow(10.0f, exp);
		}
	}

	if (inv) f *= -1.0f;
	
	out = f;
	return c;
}

float Parser::Atof(const char* buffer)
{
	float ret;
	fast_atof_move(const_cast<char*>(buffer), ret);
	return ret;
}