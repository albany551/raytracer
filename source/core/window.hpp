
#ifndef CORE_WINDOW_HPP
#define CORE_WINDOW_HPP

#include <core/common.hpp>

namespace core
{
	class Window
	{
		public:

		Window();
		virtual ~Window();

		bool			Open(int width, int height, const char* name, bool fullscreen);
		void			Close();
		void			Run();
		void			Quit();
		int				GetWidth() const { return width; }
		int				GetHeight() const { return height; }
		WindowHandle	GetHandle() const { return handle; }

		void	SetEnabled(bool value);
		bool	GetEnabled() const;
		void	Rename(const char* name);
		void	Resize(int width, int height);
		void	Move(int x, int y);
		void	SetCursor(bool value);


		// main loop for application
		virtual	void Update(float dt)
		{
		}

		// virtual Events

		virtual	void EventDraw()
		{

		}

		virtual	void EventSize(int width, int height)
		{
			// default handler: do nothing
		}

		virtual	void EventFocus(bool enter)
		{
			// default handler: do nothing
		}

		virtual	void EventDropFile(const char* filename)
		{
			// default handler: do nothing
		}

		virtual	void EventClose()
		{
			// default handler: do nothing
		}

		private:

		int				width;
		int				height;
		bool			opened;
		bool			active;
		bool			quit;
		WindowHandle	handle;
	};	

}

#endif
