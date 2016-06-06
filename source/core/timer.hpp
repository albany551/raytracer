
#ifndef CORE_TIMER_HPP
#define CORE_TIMER_HPP

#include <core/common.hpp>

namespace core
{
	class Timer
	{
		public:

		Timer();
		
		float		GetTime() const;
		void		Reset();

		private:

		int64		freq;
		int64		time0;
	};

} // namespace core

#endif
