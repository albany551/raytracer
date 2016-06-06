
#include <core/mutex.hpp>

using namespace core;

Mutex::Mutex()
{
	InitializeCriticalSection(&handle);
}

Mutex::~Mutex()
{
	DeleteCriticalSection(&handle);
}

void Mutex::Lock()
{
	EnterCriticalSection(&handle);
}

void Mutex::Unlock()
{
	LeaveCriticalSection(&handle);
}

void Mutex::Increment(long* value)
{
    InterlockedIncrement(value);
}

long Mutex::Decrement(long* value)
{
    return InterlockedDecrement(value);
}

long Mutex::TestAndSet(long* value, long newvalue)
{
	return InterlockedExchange(value, newvalue);
}
