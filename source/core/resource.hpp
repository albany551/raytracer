
#ifndef CORE_RESOURCE_HPP
#define CORE_RESOURCE_HPP

#include <core/object.hpp>
#include <core/string.hpp>

namespace core
{
	class Resource : public Object
	{
		public:

		enum State
		{
			INVALID,
			VALID
		};

		Resource() {}
		virtual ~Resource() {}

		// loads the resource data (stalls until completed)
		bool Load()

		{
			return LoadResource();
		}

		// loads the resource data (asynchronous, returns immediately)
		bool LoadAsync();
		// unloads the resource data
		void Unload();

		const String& GetName() const { return name; }

		String	name;

		protected:

		// override these to implement a resource
		virtual bool LoadResource() { return true; }
		virtual void UnloadResource() { }

		

		private:

		
		State	state;

		State GetState() const
		{
			Object::Lock lock(this);
			State s = state;
			return s;
		}

		void SetState(State value)
		{
			Object::Lock lock(this);
			state = value;
		}

		//amph::LinkedList<Resource*>::Iterator* mIterator;
		//friend class Manager;
		//class Manager* mManager;
	};
}
#endif

