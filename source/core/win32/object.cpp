
#include <core/object.hpp>

using namespace core;

Object::Object() :
	refs(0)
{
	InitializeCriticalSection(&mutex);
}

Object::Object( const Object& ) :
	refs(0)
{
	InitializeCriticalSection(&mutex);
}

Object::~Object()																		
{
	CORE_ASSERT(refs ==0);
	DeleteCriticalSection(&mutex);
}

Object& Object::operator=( const Object& )
{
	return *this;
}

void Object::AddRef()
{
	CORE_ASSERT(refs >= 0);
	InterlockedIncrement(&refs);
}

void Object::Release()
{
	CORE_ASSERT(refs  > 0);
	if(InterlockedDecrement(&refs) == 0) delete this;
}

Object::Lock::Lock(const Object& o)
{
	mutex = o.mutex;
	EnterCriticalSection(&mutex);
}

Object::Lock::Lock(const Object* o)
{
	mutex = o->mutex;
	EnterCriticalSection(&mutex);
}

Object::Lock::~Lock()
{
	LeaveCriticalSection(&mutex);
}



