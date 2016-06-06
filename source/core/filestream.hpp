
#ifndef CORE_FILESTREAM_HPP
#define CORE_FILESTREAM_HPP

#include <core/stream.hpp>

namespace core
{
	class FileStream : public Stream
	{
		public:
		
		FileStream(const char* path, AccessMode mode = READ);
		virtual ~FileStream();
		
		int			Seek(int delta, SeekMode mode);
		int			Read(void* target, int bytes);
		int			Write(const void* source, int bytes);
		int			GetOffset() const;
		int			GetSize() const;
		bool		IsOpen() const;
		bool		IsEnd() const;

		private:
		
		Stream*		stream;
		int			streamsize;
		int			offset;
	};

}

#endif
