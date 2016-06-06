
#include <gfx/texture.hpp>
#include <core/filestream.hpp>
#include <core/filename.hpp>

using namespace core;
using namespace gfx;

Texture::Texture() :
    image(0),
    width(0),
    height(0),
	miplevels(0)
{

}

Texture::~Texture()
{
    delete[] this->image; 
}

void Texture::SetPixels(int xsize, int ysize, uint8* pixels)
{
	CORE_ASSERT( (xsize > 0 && ysize > 0) );
	delete[] image;	

	width  = xsize;
	height = ysize;
	image  = reinterpret_cast<uint8*>(pixels);
}

void Texture::SetPixels(int xsize, int ysize, vector3f* pixels)
{
	CORE_ASSERT( (xsize > 0 && ysize > 0) );
	delete[] image;	
	image = new uint8[xsize * ysize * 3];
	width  = xsize;
	height = ysize;

    for(int j=0; j<xsize; j++ )
        for(int k=0; k<ysize; k++ )
        {
			int offset = k * xsize + j;
			vector3f color = pixels[offset];
			image[offset * 3 + 0] = static_cast<uint8>(color.r * 255);
			image[offset * 3 + 1] = static_cast<uint8>(color.g * 255);
			image[offset * 3 + 2] = static_cast<uint8>(color.b * 255);

        }

	pixelformat = PixelFormat::RGB888;
}

void Texture::Downsample()
{
	if(pixelformat != PixelFormat::RGB888)
		return;

	int newhres = width / 2;
	int newvres = height / 2;
	uint8* newpixels = new uint8[newhres * newvres * 3];

	for(int j=0; j < newhres; j++)
		for(int k=0; k < newvres; k++)
		{
			int offset = (k * newhres + j) * 3;
			int offset00 = (k*2 * width + j*2) * 3;
			int offset01 = (k*2 * width + (j+1)*2) * 3;
			int offset10 = ((k+1)*2 * width + j*2) * 3;
			int offset11 = ((k+1)*2 * width + (j+1)*2) * 3;

			newpixels[offset+0] = (image[offset00+0]+image[offset01+0]+image[offset10+0]+image[offset11+0])/4;
			newpixels[offset+1] = (image[offset00+1]+image[offset01+1]+image[offset10+1]+image[offset11+1])/4;
			newpixels[offset+2] = (image[offset00+2]+image[offset01+2]+image[offset10+2]+image[offset11+2])/4;
		}
	
	delete[] image;
	width = newhres;
	height = newvres;
	image = newpixels;
}

vector3f Texture::GetTexel(float u, float v)
{
	if(pixelformat != PixelFormat::RGB888)
		return vector3f(0, 0, 0);

	int x = static_cast<int>(u * (width-1));
	int y = static_cast<int>(v * (height-1));
	int depth = PixelFormat::GetDepth(pixelformat);

	CORE_ASSERT(x >= 0 && x < width);
	CORE_ASSERT(y >= 0 && y < height);

	int offset = (y * width + x) * depth;
	return vector3f((float)image[offset+0] / 255, (float)image[offset+1] / 255, (float)image[offset+2] / 255);

}

// =================================================
// BMP Codec
// =================================================

#pragma pack (push, 1)

struct BMPHeader
{
	uint16 bmpIdentifier;
	uint8  junk[16];
	uint32 width;
	uint32 height;
	uint16 junk2;
	uint16 bpp;
	uint16 compression;
	uint8  junk3[22];
};

#pragma pack (pop)

bool Texture::SaveBMP(const char *filename)
{
	if(pixelformat != PixelFormat::RGB888)
		return false;

	FileStream file(filename, Stream::WRITE);
	if(!file.IsOpen()) return false;

	int nChannels = 3;
	BMPHeader header = 
    {
		('M' << 8) | 'B',
		{0x36, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 
        0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00},
		width,
		height,
		0x0001,
		nChannels * 8,
		0,
		{0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x13, 0x0b, 0x00, 
        0x00, 0x13, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00}
	};

	file.Write(&header, sizeof(header));
	
	bool useAlpha = (nChannels == 4);
	int paddBytes = (width * nChannels) & 3;
	if (paddBytes) paddBytes = 4 - paddBytes;

	int len, size = (width * nChannels + paddBytes) * height;
	uint8 *dest, *src, *buffer = new uint8[size];

	for (int y = 0; y < height; y++)
    {
		dest = buffer + (height - y - 1) * (width * nChannels + paddBytes);
		src  = image + y * width * nChannels;
		len = width;
		do {
			*dest++ = src[2];
			*dest++ = src[1];
			*dest++ = src[0];
			if (useAlpha) *dest++ = src[3];
			src += nChannels;
		} while (--len);
	}
	file.Write(buffer, size);
	delete[] buffer;
	
	return true;
}

bool Texture::LoadResource()
{
	if(!LoadData())
		return false;

	return true;
}

bool Texture::LoadData()
{
	// create stream
	FileStream stream(GetName(), Stream::READ);
	if(!stream.IsOpen()) return false;

	String ext = GetFilenameEXT(GetName());
	ext.ToLower();

	// launch codec
	if(ext=="dds")
		LoadDataDDS(stream);
	else if(ext=="tga")
		LoadDataTGA(stream);
	else
		return false;

	return true;
}

// =================================================
// DDS Codec
// =================================================

struct DDPIXELFORMAT
{
	uint32       dwSize;                 // size of structure
	uint32       dwFlags;                // pixel format flags
	uint32       dwFourCC;               // (FOURCC code)
	union
	{
		uint32   dwRGBBitCount;          // how many bits per pixel
		uint32   dwYUVBitCount;          // how many bits per pixel
		uint32   dwZBufferBitDepth;      // how many total bits/pixel in z buffer (including any stencil bits)
		uint32   dwAlphaBitDepth;        // how many bits for alpha channels
		uint32   dwLuminanceBitCount;    // how many bits per pixel
		uint32   dwBumpBitCount;         // how many bits per "buxel", total
		uint32   dwPrivateFormatBitCount;// Bits per pixel of private driver formats. Only valid in texture
										// format list and if DDPF_D3DFORMAT is set
	};
	union
	{
		uint32   dwRBitMask;             // mask for red bit
		uint32   dwYBitMask;             // mask for Y bits
		uint32   dwStencilBitDepth;      // how many stencil bits (note: dwZBufferBitDepth-dwStencilBitDepth is total Z-only bits)
		uint32   dwLuminanceBitMask;     // mask for luminance bits
		uint32   dwBumpDuBitMask;        // mask for bump map U delta bits
		uint32   dwOperations;           // DDPF_D3DFORMAT Operations
	};
	union
	{
		uint32   dwGBitMask;             // mask for green bits
		uint32   dwUBitMask;             // mask for U bits
		uint32   dwZBitMask;             // mask for Z bits
		uint32   dwBumpDvBitMask;        // mask for bump map V delta bits
		struct
		{
			uint16    wFlipMSTypes;       // Multisample methods supported via flip for this D3DFORMAT
			uint16    wBltMSTypes;        // Multisample methods supported via blt for this D3DFORMAT
		} MultiSampleCaps;

	};
	union
	{
		uint32   dwBBitMask;             // mask for blue bits
		uint32   dwVBitMask;             // mask for V bits
		uint32   dwStencilBitMask;       // mask for stencil bits
		uint32   dwBumpLuminanceBitMask; // mask for luminance in bump map
	};
	union
	{
		uint32   dwRGBAlphaBitMask;      // mask for alpha channel
		uint32   dwYUVAlphaBitMask;      // mask for alpha channel
		uint32   dwLuminanceAlphaBitMask;// mask for alpha channel
		uint32   dwRGBZBitMask;          // mask for Z channel
		uint32   dwYUVZBitMask;          // mask for Z channel
	};
};

struct DDCOLORKEY
{
	uint32       dwColorSpaceLowValue;   // low boundary of color space that is to be treated as Color Key, inclusive
	uint32       dwColorSpaceHighValue;  // high boundary of color space that is to be treated as Color Key, inclusive
};

struct DDSCAPS2
{
	uint32 dwCaps;         // capabilities of surface wanted
	uint32 dwCaps2;
	uint32 dwCaps3;
	union
	{
		uint32 dwCaps4;
		uint32 dwVolumeDepth;
	};
};

struct DDSURFACEDESC2
{
	uint32               dwSize;                 // size of the DDSURFACEDESC structure
	uint32               dwFlags;                // determines what fields are valid
	uint32               dwHeight;               // height of surface to be created
	uint32               dwWidth;                // width of input surface
	union
	{
		int32            lPitch;                 // distance to start of next line (return value only)
		uint32           dwLinearSize;           // Formless late-allocated optimized surface size
	};
	union
	{
		uint32           dwBackBufferCount;      // number of back buffers requested
		uint32           dwDepth;                // the depth if this is a volume texture 
	};
	union
	{
		uint32           dwMipMapCount;          // number of mip-map levels requestde
												// dwZBufferBitDepth removed, use ddpfPixelFormat one instead
		uint32           dwRefreshRate;          // refresh rate (used when display mode is described)
		uint32           dwSrcVBHandle;          // The source used in VB::Optimize
	};
	uint32               dwAlphaBitDepth;        // depth of alpha buffer requested
	uint32               dwReserved;             // reserved
	void*              lpSurface;                // pointer to the associated surface memory
	union
	{
		DDCOLORKEY      ddckCKDestOverlay;       // color key for destination overlay use
		uint32           dwEmptyFaceColor;       // Physical color for empty cubemap faces
	};
	DDCOLORKEY          ddckCKDestBlt;           // color key for destination blt use
	DDCOLORKEY          ddckCKSrcOverlay;        // color key for source overlay use
	DDCOLORKEY          ddckCKSrcBlt;            // color key for source blt use
	union
	{
		DDPIXELFORMAT   ddpfPixelFormat;         // pixel format description of the surface
		uint32           dwFVF;                  // vertex format description of vertex buffers
	};
	DDSCAPS2            ddsCaps;                 // direct draw surface capabilities
	uint32               dwTextureStage;         // stage in multitexture cascade
};

bool Texture::LoadDataDDS(Stream& stream)
{
	// reset stream
	stream.Seek(0,Stream::START);

	// check header
	uint32 magic = stream.ReadLittleEndian<uint32>();
	if ( magic != CORE_CODE32('D','D','S',' ') )
		return false;

	// read surface description
	// TODO: read in endianess aware format
	DDSURFACEDESC2 ddsd;
	stream.Read((char*)&ddsd,sizeof(ddsd));

	// get properties
	int factor = 0;
	width = ddsd.dwWidth;
	height = ddsd.dwHeight;
	miplevels = ddsd.dwMipMapCount+1;
	int bufsize = ddsd.dwLinearSize;

	if(!bufsize) bufsize = (height*width)/2;	// HACK to deal with dwLinearSize==0
	CORE_ASSERT((width % 4) == 0);
	CORE_ASSERT((height % 4) == 0);
	CORE_ASSERT(bufsize);

	// initialize decode target
	switch(ddsd.ddpfPixelFormat.dwFourCC)
	{
		case CORE_CODE32('D','X','T','1'): pixelformat = PixelFormat::DXT1; factor = 2; break;
		case CORE_CODE32('D','X','T','3'): pixelformat = PixelFormat::DXT3; factor = 4; break;
		case CORE_CODE32('D','X','T','5'): pixelformat = PixelFormat::DXT5; factor = 4; break;
	}
	if(miplevels > 1) bufsize *= factor;

	// need to skip mipmaps here based on texture detail setting
	// int detail = Texture::GetDetailLevel();
	// stream.Seek(current+whatever);

	image = new uint8[bufsize];
	stream.Read(image,bufsize);

	return true;
}


// =================================================
// TGA Codec
// =================================================

struct HeaderTGA
{
	uint8		idfield_length;
	uint8		colormap_type;
	uint8		data_type;
	uint16		colormap_origin;
	uint16		colormap_length;
	uint8		colormap_bitsize;
	uint16		image_origin_x;
	uint16		image_origin_y;
	uint16		image_width;
	uint16		image_height;
	uint8		pixelsize;
	uint8		descriptor;

	bool ReadHeader(Stream& stream)
	{
		// read header
		idfield_length   = stream.ReadLittleEndian<uint8>();
		colormap_type    = stream.ReadLittleEndian<uint8>();
		data_type        = stream.ReadLittleEndian<uint8>();
		colormap_origin  = stream.ReadLittleEndian<uint16>();
		colormap_length  = stream.ReadLittleEndian<uint16>();
		colormap_bitsize = stream.ReadLittleEndian<uint8>();
		image_origin_x   = stream.ReadLittleEndian<uint16>();
		image_origin_y   = stream.ReadLittleEndian<uint16>();
		image_width      = stream.ReadLittleEndian<uint16>();
		image_height     = stream.ReadLittleEndian<uint16>();
		pixelsize        = stream.ReadLittleEndian<uint8>();
		descriptor       = stream.ReadLittleEndian<uint8>();

		// validate header
		switch ( data_type )
		{
			case 1:
			case 2:
			case 9:
			case 10: break;
			default: return false;
		}

		switch ( pixelsize )
		{
			case 8:
			case 16:
			case 24:
			case 32: break;
			default: return false;
		}

		if ( colormap_type > 1 )
			return false;

		if ( (data_type == 1 || data_type == 9) && (colormap_bitsize != 24 || colormap_length > 256) )
			return false;

		// everything seems to be in order
		return true;
	}

};

static void UnpackLINEAR(Stream& stream, uint8* scan, int nextscan, int width, int height, int depth)
{
	int pitch = width * depth;
	for ( int y=0; y<height; ++y, scan+=nextscan )
	{
		// decode scanline
		stream.Read(scan,pitch);
		// go through and swap bytes to get RGBA
	}
}


static void UnpackRLE(Stream& stream, uint8* scan, int nextscan, int width, int height, int depth)
{
	int x = 0;
	int y = 0;
	uint8* buffer = scan;

	for ( ;; )
	{
		uint8 sample = stream.ReadLittleEndian<uint8>();
		int count = (sample & 0x7f) + 1;

		if ( sample & 0x80 )
		{
			uint8 color[4];
			stream.Read(color,depth);

			for ( ; count; )
			{
				int left = width - x;
				int size = math::Min(count,left);
				
				count -= size;
				x += size;
				
				for ( ; size; --size )
					for ( int j=0; j<depth; ++j )
						*buffer++ = color[j];
					
				if ( x >= width )
				{
					if ( ++y >= height )
						return;

					x = 0;
					scan += nextscan;
					buffer = scan;
				}
			}
		}
		else
		{
			for ( ; count; )
			{
				int left = width - x;
				int size = math::Min(count,left);
				
				stream.Read(buffer,size*depth);
				buffer += size*depth;
				count -= size;

				x += size;
				if ( x >= width )
				{
					if ( ++y >= height )
						return;

					x = 0;
					scan += nextscan;
					buffer = scan;
				}
			}
		}
	}
}

bool Texture::LoadDataTGA(Stream& stream)
{
	// reset stream
	stream.Seek(0,Stream::START);

	// read header
	HeaderTGA header;
	if ( !header.ReadHeader(stream) )
		return false;

	// skip idfield
	stream.Seek(header.idfield_length, Stream::CURRENT);

	// information
	switch(header.pixelsize)
	{
		case 24:	pixelformat = PixelFormat::RGB888; break;
		case 32:	pixelformat = PixelFormat::RGBA888; break;
		default:	return false;
	}

	// read palette
	switch(header.data_type)
	{
		case 2:
		case 10:
		{
			int delta = header.colormap_length * ((header.colormap_bitsize + 1) >> 3);
			stream.Seek(delta, Stream::CURRENT);
			break;
		}
	}

	// setup info
	width  = header.image_width;
	height = header.image_height;
	miplevels = 1;
	int depth  = PixelFormat::GetDepth(pixelformat);
	int pitch  = width * depth;

	// set image
	image = new uint8[height * pitch];

	// image orientation
	uint8* scan   = (header.descriptor & 32) ? image : image + (height - 1) * pitch;
	int nextscan = (header.descriptor & 32) ? pitch : -pitch;

	// decode image
	switch ( header.data_type )
	{
		case 2:	UnpackLINEAR(stream,scan,nextscan,width,height,depth); break;	// rgb 16,24,32bit (linear)
		case 10: UnpackRLE(stream,scan,nextscan,width,height,depth); break;		// rgb 16,24,32bit (rle)
	}

	// re-order bytes
	if(depth==3)
	{
		uint8 temp;
		for(int i=0; i<height*pitch-3; i=i+3)
		{
			temp = image[i+0];
			image[i+0] = image[i+2];
			image[i+2] = temp;
		}
	}

	return true;
}




