
#ifndef GFX_PIXELFORMAT_HPP
#define GFX_PIXELFORMAT_HPP

namespace gfx
{
	struct PixelFormat
	{
		enum
		{
			RGB888,
			RGBA888,
			DXT1,
			DXT3,
			DXT5
		};

		static int GetDepth(int value)
		{
			switch(value)
			{
			case RGB888: return 3;
			case RGBA888: return 4;
			case DXT1: return 3;
			case DXT3: return 4;
			case DXT5: return 4;
			default: return 0;
			}
		}

	};

}


#endif
