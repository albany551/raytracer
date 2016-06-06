
#ifndef GFX_TEXTURE_HPP
#define GFX_TEXTURE_HPP

#include <core/resource.hpp>
#include <core/stream.hpp>
#include <gfx/pixelformat.hpp>
#include <math/vector3.hpp>

namespace gfx
{
	using core::Resource;
	using core::Stream;

	class Texture : public Resource
    {
        public:
           
    	Texture();
    	~Texture();
    
    	int			GetWidth() const { return width;  }
    	int			GetHeight() const { return height; }
 
		void		SetPixels(int xsize, int ysize, uint8* pixels);
		void		SetPixels(int xsize, int ysize, vector3f* pixels);
		void		Downsample();

		vector3f	GetTexel(float u, float v);

    	bool		SaveBMP(const char *filename);
    
    	protected:

		virtual bool	LoadResource();

		bool		LoadData();
		bool		LoadDataDDS(Stream& stream);
		bool		LoadDataTGA(Stream& stream);

        uint8*		image;
		int			pixelformat;
    	int			width;
		int			height;
		int			miplevels;
    
    };

} // namespace gfx

#endif
