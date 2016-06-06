#ifndef CORE_OBJECT_HPP
#define CORE_OBJECT_HPP

#include <core/ref.hpp>
#include <core/mutex.hpp>

namespace core
{
	class Object
	{
		public:

		Object();
		Object(const Object&);
		virtual ~Object();

		// Returns this object
		Object&	operator=( const Object& );

		// Increments reference count
		void	AddRef();

		// Decrements reference count by one and deletes the object if the 
        // count reaches zero. 
		void	Release();

		// Helper for synchronizing access to an Object
		struct Lock
		{
			Lock(const Object& o);
			Lock(const Object* o);
			~Lock();
			MutexHandle mutex;
		};

		private:

		friend struct	Lock;
		MutexHandle		mutex;
		long			refs;

	};

} // namespace core

#endif
