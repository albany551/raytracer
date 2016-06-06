
#ifndef CORE_ENDIAN_HPP
#define CORE_ENDIAN_HPP

#include <core/common.hpp>

namespace core
{
	inline void ReverseBytes(uint8&)
	{
	}

	inline void ReverseBytes(int8&)
	{
	}

	inline void ReverseBytes(uint16& i)
	{
		i = static_cast<uint16>((i << 8) | (i >> 8));
	}

	inline void ReverseBytes(int16& i)
	{
		ReverseBytes(reinterpret_cast<uint16&>(i));
	}

	inline void ReverseBytes(uint32& i)
	{
		i = (i >> 24) | (i >> 8) & 0x0000ff00 | (i << 8) & 0x00ff0000 | (i << 24);
	}

	inline void ReverseBytes(int32& i)
	{
		ReverseBytes(reinterpret_cast<uint32&>(i));
	}

	inline void ReverseBytes(float32& v)
	{
		ReverseBytes(reinterpret_cast<uint32&>(v));
	}

	template <class T>
	inline T ToLittleEndian(T v)
	{
		#ifdef CORE_BIG_ENDIAN
		ReverseBytes(v);
		#endif
		return v;
	}

	template <class T>
	inline T ToBigEndian(T v)
	{
		#ifdef CORE_LITTLE_ENDIAN
		ReverseBytes(v);
		#endif
		return v;
	}

} // namespace core


#endif 
