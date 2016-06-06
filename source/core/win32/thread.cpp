
#include <core/thread.hpp>

using namespace core;


typedef void* (*Func)(void*);

struct ThreadParam
{
	Func	func;
	void*	arg;
};

static unsigned __stdcall threadDispatch( void* a )
{
	ThreadParam*	param	= (ThreadParam*)a;
	Func			func	= param->func;
	void*			arg		= param->arg;

	delete param;
	param = 0;

	unsigned rv = (unsigned)func( arg );
	_endthreadex( rv );

	return rv;
}

int threadCreate( ThreadHandle* t, void* (*start)(void*), void* arg, int stackSize, Thread::Priority priority )
{

	ThreadParam* param = new ThreadParam;
	param->func = start;
	param->arg = arg;
	unsigned id;
	HANDLE h = (HANDLE)_beginthreadex( 0, stackSize, threadDispatch, param, 0, &id );
	if ( 0 == h )
	{
		*t = 0;
		delete param;
		return 1;
	}
	else
	{
		*t = (ThreadHandle)h;

		switch (priority)
		{
		case Thread::LOW:
			SetThreadPriority(t, THREAD_PRIORITY_BELOW_NORMAL);
			break;
		case Thread::NORMAL:
			SetThreadPriority(t, THREAD_PRIORITY_NORMAL);
			break;
		case Thread::HIGH:
			SetThreadPriority(t, THREAD_PRIORITY_ABOVE_NORMAL);
			break;
		}

		return 0;
	}
}

int threadDetach( ThreadHandle t )
{
	HANDLE h = (HANDLE)t;
	if ( h )
	{
		//printf( "Detaching thread (handle=%d).", (int)h );
		CloseHandle( h );
		return 0;
	}
	return 1;


}

static void* threadRun(void* arg)
{
	Thread* thread = reinterpret_cast<Thread*>(arg);

	try 
	{
		thread->Run();
	}
	catch (Exception& e)
	{
		e.Display();
	}

	//thread->m_running = false;
	thread = 0;
	return 0;
}

Thread::Thread() :
	m_handle(0),
	m_running(false)
{

}

Thread::~Thread()
{
	//printf( "~Thread");
	if (m_handle)
	{
		threadDetach(m_handle);
		m_handle = 0;
	}
}

void Thread::Start(Priority priority, int stackSize)
{
	CORE_ASSERT(!m_running);

	if(m_handle)
	{
		threadDetach(m_handle);
		m_handle = 0;
	}

	m_running = true;
	if ( threadCreate( &m_handle, threadRun, this, stackSize, priority ) )
	{
		m_running = false;
		CORE_THROW("Failed to create a thread");
	}
}

/*
void Thread::run()
{
	assert(false); // Thread is used by subclassing
}
*/

void Thread::Join()
{

}

void Thread::Sleep(long m)
{
	::Sleep(m);
}

void Thread::Signal()
{
	m_event.Signal();
}

void Thread::Wait()
{
	m_event.Wait();
}


Thread::Event::Event()
{
    m_handle = CreateEvent(NULL, FALSE, FALSE, NULL);
    CORE_ASSERT(m_handle);
}

Thread::Event::~Event()
{
	CloseHandle(m_handle);
}

void Thread::Event::Signal()
{
	SetEvent(m_handle);
}

void Thread::Event::Wait()
{
	WaitForSingleObject(m_handle, INFINITE);
}

