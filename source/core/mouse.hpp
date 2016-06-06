
#ifndef CORE_MOUSE_HPP
#define CORE_MOUSE_HPP

namespace core
{

	class Mouse
	{
		public:

		enum MouseButton
		{
			MOUSE_LEFT,
			MOUSE_RIGHT,
			MOUSE_MIDDLE,
			MOUSE_XBUTTON1,
			MOUSE_XBUTTON2,
			MOUSE_WHEEL
		};

		Mouse();
		~Mouse();

		int			GetButton(int button);
		void		SetXY(int x, int y);
		int			GetX();
		int			GetY();
		int			GetDeltaX();
		int			GetDeltaY();
	};

} // namespace core


#endif