
#ifndef CORE_THREAD_HPP
#define CORE_THREAD_HPP

#include <core/common.hpp>

namespace core
{
	// =================================================
	// OS thread wrapper ( an abstract class, must derive and implement run() )
	// =================================================
	
	class Thread
	{
		public:

		// thread priority levels
		enum Priority
		{
			LOW,
			NORMAL,
			HIGH
		};

		Thread();
		virtual ~Thread();

		// start the execution of Run() method in a new thread
		void Start( Priority priority=NORMAL, int stackSize=0 );

		// suspends execution of the calling thread until this thread terminates
		void Join();

		// actual thread method (override this!)
		virtual void Run() = 0;

		// halt execution of current thread for specified number of milliseconds
		static void Sleep(long m);\

		// re-engage this thread
		void Signal();

		// yield this thread
		void Wait();

		// for signaling between threads
		class Event
		{
		public:
			Event();
			~Event();
			void Signal();
			void Wait();
			bool WaitTimed(int ms);
		private:
			ThreadHandle m_handle;
		};

	private:
		ThreadHandle	m_handle;
		bool			m_running;
		Event			m_event;
	};


} // namespace core

#endif
