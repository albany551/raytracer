
#ifndef _LANG_REF_
#define _LANG_REF_

namespace core
{
	// Reference-counting smart pointer to a class derived from Object
	template <class T> class Ref
	{
		public:

		Ref() :
		  object(0)
		{
		}
 
		~Ref()
		{
			if (object) object->Release();
		}

		Ref(const Ref<T>& other)
		{
			T* o = other.Ptr();
			if(o) o->AddRef();
			object = o;
		}

		Ref(T* other)
		{
			if(other) other->AddRef(); 
			object = other;
		}

		Ref<T>& operator=(const Ref<T>& other)
		{
			T* o = other.Ptr();
			if(o) o->AddRef(); 
			if(object) object->Release();
			object = o;
			return *this;
		}

		T&        operator*() const { return *object; }
		T*        operator->() const { return object; }
		operator  T*() const { return object; }
		T*        Ptr() const { return object; }

		private:

		T* object;
	};


} // namespace core

#endif // _LANG_REF_

