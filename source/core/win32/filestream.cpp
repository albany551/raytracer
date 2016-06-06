
#include <core/filestream.hpp>
#include <core/common.hpp>

using namespace core;

// regular OS file implementation
class File : public Stream
{
public:
	File() :
		handle(0)
	{
	}

	File(const char* path)
	{
		Open(path);
	}

	virtual ~File()
	{
		Close();
	}

	bool Open(const char* path, AccessMode mode = READ)
	{
		DWORD access = 0;
		DWORD disposition = 0;
		DWORD sharemode = 0;

		switch( mode )
		{
			case Stream::READ:		access = GENERIC_READ; break;
			case Stream::WRITE:		access = GENERIC_WRITE; break;
			case Stream::READWRITE:	access = GENERIC_READ | GENERIC_WRITE; break;
		}

		if (access & GENERIC_WRITE)
		{
			disposition = CREATE_ALWAYS;
		}
		else
		{
			disposition = OPEN_EXISTING;
			sharemode   = FILE_SHARE_READ;
		}

		handle = CreateFile(path,    // filename
                              access,           // access mode
                              sharemode,        // share mode
                              0,                // security flags
                              disposition,      // what to do if file doesn't exist
                              FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS,  // flags'n'attributes
                              0);               // template file
		if (handle == INVALID_HANDLE_VALUE)
		{
			handle = 0;
			return false;
		}
		return true;
	}

	void Close()
	{
		if (handle)
		{
			CloseHandle(handle);
			handle = 0;
		}
	}

	int Seek(int delta, SeekMode mode)
	{
		DWORD method;
		switch( mode )
		{
			case START:		method = FILE_BEGIN; break;
			case CURRENT:	method = FILE_CURRENT; break;
			case END:		method = FILE_END; break;
		}
		return SetFilePointer(handle, (LONG)delta, NULL, method);
	}

	int Read(void* target, int bytes)
	{
		DWORD read;
		ReadFile(handle, target, bytes, &read, NULL);
		return read;
	}

	int Write(const void* source, int bytes)
	{
		DWORD written;
		WriteFile(handle, source, bytes, &written, NULL);
		return written;
	}

	int GetOffset() const
	{
		return SetFilePointer(handle, 0, NULL, FILE_CURRENT);
	}

	int GetSize() const
	{
		return GetFileSize(handle, NULL);
	}

	bool IsOpen() const
	{
		return handle != 0;
	}

	bool IsEnd() const
	{
		DWORD fpos = SetFilePointer(handle,0,NULL,FILE_CURRENT);
		DWORD size = GetFileSize(handle,NULL);
		return (fpos == size);
	}

private:

	HANDLE handle;
};

static Stream* CreateStream(const char* filename, Stream::AccessMode mode)
{
	File* fs = new File();
    fs->Open(filename, mode);
	if(!fs->IsOpen()) return 0;

	return fs;
}


FileStream::FileStream(const char* path, AccessMode mode) :
	stream(0),
	streamsize(0),
	offset(0)
{
	stream = CreateStream(path, mode);
	if (!stream) return;
	streamsize = stream->GetSize();
}


FileStream::~FileStream()
{
	delete stream;
}


int FileStream::Seek(int delta, SeekMode mode)
{
	switch ( mode )
	{
		case START:		offset = delta; break;
		case CURRENT:	offset += delta; break;
		case END:		offset = streamsize - delta; break;
	}
		
	// check offset range
	CORE_ASSERT( offset >= 0 && offset <= streamsize );

	stream->Seek(offset,Stream::START);

	return offset;
}


int FileStream::Read(void* target, int bytes)
{
	return stream->Read(target,bytes);
}


int FileStream::Write(const void* source, int bytes)
{
	int delta = stream->Write(source,bytes);
	offset += delta;
	return delta;
}


int FileStream::GetOffset() const
{
	return offset;
}


int FileStream::GetSize() const
{
	return streamsize;
}


bool FileStream::IsOpen() const
{
	return (stream && stream->IsOpen());
}


bool FileStream::IsEnd() const
{
	return offset >= streamsize;
}



