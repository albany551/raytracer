
#ifndef CORE_EXCEPTION_HPP
#define CORE_EXCEPTION_HPP

namespace core
{
	class Exception
	{
		public:

		Exception(const char* method, const char* message, const char* filename, int line = -1);

		void Display();

		private:

		const char*	method;
		const char*	message;
		const char*	filename;
		int			line;
	};

	// exception macros

	#define CORE_THROW(message) \
        do { \
            throw core::Exception(CORE_FUNCTION,message,CORE_FILENAME,CORE_LINE); \
        } while ( 0 )

    #define CORE_ASSERT( expression ) \
        do { \
            if(!(expression)) \
                throw core::Exception(CORE_FUNCTION,"N/A",CORE_FILENAME,CORE_LINE); \
        } while( 0 )

} // namespace core

#endif
