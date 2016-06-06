
#ifndef GFX_VERTEXFORMAT_HPP
#define GFX_VERTEXFORMAT_HPP

namespace gfx
{
	class VertexFormat
	{
		public:

		enum Components
		{
			COORD    = (1<<0),
			NORMAL   = (1<<1),
			UV0      = (1<<2),
			UV1      = (1<<3),
			UV2      = (1<<4),
			UV3      = (1<<5),
			COLOR    = (1<<6),
			TANGENT  = (1<<7),
			BINORMAL = (1<<8),
			WEIGHTS  = (1<<9),
			JINDICES = (1<<10),
			ALL      = (1<<30) 
		};

		VertexFormat() :
			value(0)
		{
		}

		VertexFormat(int components) :
			value(components)
		{
			width = GetOffset(ALL);
		}

		bool Contains(int component)
		{
			return (value & component) ? true : false;
		}

		int GetWidth()
		{
			return width;
		}

		int GetByteWidth()
		{
			return width * sizeof(float);
		}

		// these sizes refer to float elements, not bytes!
		int GetOffset(int component)
		{
			int offset = 0;
			if(component <= COORD) return offset; if(value & COORD) offset += 3;
			if(component <= NORMAL) return offset; if(value & NORMAL) offset += 3; 
			if(component <= UV0) return offset; if(value & UV0) offset += 2; 
			if(component <= UV1) return offset; if(value & UV1) offset += 2;
			if(component <= UV2) return offset; if(value & UV2) offset += 2;
			if(component <= UV3) return offset; if(value & UV3) offset += 2;
			if(component <= COLOR) return offset; if(value & COLOR) offset += 4;
			if(component <= TANGENT) return offset; if(value & TANGENT) offset += 3;
			if(component <= BINORMAL) return offset; if(value & BINORMAL) offset += 3;
			if(component <= WEIGHTS) return offset; if(value & WEIGHTS) offset += 4;
			if(component <= JINDICES) return offset; if(value & JINDICES) offset += 4;
			return offset;
		}

		int value;
		int width;
	};

}


#endif
