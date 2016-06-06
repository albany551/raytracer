
#ifndef CORE_STREAM_HPP
#define CORE_STREAM_HPP

#include <core/endian.hpp>

namespace core
{	
	class Stream
	{
		public:

		enum AccessMode
		{
			READ		= 1,
			WRITE		= 2,
			READWRITE	= READ | WRITE
		};

		enum SeekMode
		{
			START,		// seek from the start of the stream
			CURRENT,	// seek from the current stream position
			END			// seek from the end of the stream
		};

		virtual	int		Seek(int delta, SeekMode mode) = 0;
		virtual	int		Read(void* target, int bytes) = 0;
		virtual	int		Write(const void* source, int bytes) = 0;
		virtual	int		GetOffset() const = 0;
		virtual	int		GetSize() const = 0;
		virtual	bool	IsOpen() const = 0;
		virtual	bool	IsEnd() const = 0;
		
		// endian templates
		
    	template <typename T> 
    	T ReadLittleEndian()
    	{
    		T v;
    		Read(&v,sizeof(v));
    		return ToLittleEndian(v);
    	}
    
    	template <typename T>
    	T ReadBigEndian()
    	{
    		T v;
    		Read(&v,sizeof(v));
    		return ToBigEndian(v);
    	}
    
    	template <typename T>
    	void WriteLittleEndian(T v)
    	{
    		v = ToLittleEndian(v);
            Write(&v,sizeof(v));
    	}
    
    	template <typename T>
    	void WriteBigEndian(T v)
    	{
    		v = ToBigEndian(v);
    		Write(&v,sizeof(v));
    	}
		
	};

	#define CORE_CODE32(n0,n1,n2,n3) ((uint32)(uint8)(n0) | ((uint32)(uint8)(n1) << 8) | ((uint32)(uint8)(n2) << 16) | ((uint32)(uint8)(n3) << 24 ))

}

#endif // namespace core
