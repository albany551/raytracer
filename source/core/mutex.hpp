
#ifndef CORE_MUTEX_HPP
#define CORE_MUTEX_HPP

#include <core/common.hpp>

namespace core
{
	class Mutex
	{
	public:
		Mutex();
		virtual ~Mutex();

		void Lock();
		void Unlock();

		// Atomic increment operation for reference counting
		static void Increment(long* value);

		// Atomic decrement operation for reference counting
		static long Decrement(long* value);

		// Atomic test-and-set operation
		static long TestAndSet(long* value, long newvalue=1);
	
	private:
		MutexHandle handle;
	};

} // namespace core

#endif
