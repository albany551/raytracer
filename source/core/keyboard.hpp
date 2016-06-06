
#ifndef CORE_KEYBOARD_HPP
#define CORE_KEYBOARD_HPP

namespace core
{
	class Keyboard
	{
		public:

		enum KeyCode
		{
			KEY_ESC          = 0x01,
			KEY_1            = 0x02,
			KEY_2            = 0x03,
			KEY_3            = 0x04,
			KEY_4            = 0x05,
			KEY_5            = 0x06,
			KEY_6            = 0x07,
			KEY_7            = 0x08,
			KEY_8            = 0x09,
			KEY_9            = 0x0a,
			KEY_0            = 0x0b,
			KEY_A            = 0x10,
			KEY_B            = 0x11,
			KEY_C            = 0x12,
			KEY_D            = 0x13,
			KEY_E            = 0x14,
			KEY_F            = 0x15,
			KEY_G            = 0x16,
			KEY_H            = 0x17,
			KEY_I            = 0x18,
			KEY_J            = 0x19,
			KEY_K            = 0x1a,
			KEY_L            = 0x1b,
			KEY_M            = 0x1c,
			KEY_N            = 0x1d,
			KEY_O            = 0x1e,
			KEY_P            = 0x1f,
			KEY_Q            = 0x20,
			KEY_R            = 0x21,
			KEY_S            = 0x22,
			KEY_T            = 0x23,
			KEY_U            = 0x24,
			KEY_V            = 0x25,
			KEY_W            = 0x26,
			KEY_X            = 0x27,
			KEY_Y            = 0x28,
			KEY_Z            = 0x29,
			KEY_F1           = 0x30,
			KEY_F2           = 0x31,
			KEY_F3           = 0x32,
			KEY_F4           = 0x33,
			KEY_F5           = 0x34,
			KEY_F6           = 0x35,
			KEY_F7           = 0x36,
			KEY_F8           = 0x37,
			KEY_F9           = 0x38,
			KEY_F10          = 0x39,
			KEY_F11          = 0x3a,
			KEY_F12          = 0x3b,
			KEY_NUMPAD0      = 0x40,
			KEY_NUMPAD1      = 0x41,
			KEY_NUMPAD2      = 0x42,
			KEY_NUMPAD3      = 0x43,
			KEY_NUMPAD4      = 0x44,
			KEY_NUMPAD5      = 0x45,
			KEY_NUMPAD6      = 0x46,
			KEY_NUMPAD7      = 0x47,
			KEY_NUMPAD8      = 0x48,
			KEY_NUMPAD9      = 0x49,
			KEY_NUMLOCK      = 0x4a,
			KEY_DIVIDE       = 0x4b,
			KEY_MULTIPLY     = 0x4c,
			KEY_SUBTRACT     = 0x4d,
			KEY_ADDITION     = 0x4e,
			KEY_ENTER        = 0x4f,
			KEY_DECIMAL      = 0x50,
			KEY_BACKSPACE    = 0x60,
			KEY_TAB          = 0x61,
			KEY_RETURN       = 0x62,
			KEY_LEFT_CTRL    = 0x63,
			KEY_RIGHT_CTRL   = 0x64,
			KEY_LEFT_SHIFT   = 0x65,
			KEY_RIGHT_SHIFT  = 0x66,
			KEY_LEFT_ALT     = 0x67,
			KEY_RIGHT_ALT    = 0x68,
			KEY_SPACE        = 0x69,
			KEY_CAPSLOCK     = 0x6a,
			KEY_PRINT_SCREEN = 0x6b,
			KEY_SCROLL_LOCK  = 0x6c,
			KEY_PAGEUP       = 0x6d,
			KEY_PAGEDOWN     = 0x6e,
			KEY_INSERT       = 0x6f,
			KEY_DELETE       = 0x70,
			KEY_HOME         = 0x71,
			KEY_END          = 0x72,
			KEY_LEFT         = 0x73,
			KEY_RIGHT        = 0x74,
			KEY_UP			 = 0x75,
			KEY_DOWN         = 0x76
		};

		Keyboard();
		~Keyboard();

		int GetKey(int keycode);
	};

} // namespace core


#endif