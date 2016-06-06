
#ifndef CORE_PARSER_HPP
#define CORE_PARSER_HPP

namespace core
{
	class Parser
	{
		public:

		static const int MAX_TOKEN_LENGTH = 32;

		Parser();
		Parser(const char* buffer, const int size=0);
		void			Start(const char* buffer, const int size=0);
		bool			Advance();

		const char*		GetToken() const { return token; }
		int				GetLine() const { return line; }

		static float	Atof(const char* buffer);

		protected:

		char			token[MAX_TOKEN_LENGTH];		// token buffer
		int				line;

		const char*		buffer;
		const char*		buf_end;
	};

} // namespace core

#endif

