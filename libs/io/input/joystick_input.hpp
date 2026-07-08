#pragma once

#define JOYSTICK_BTN_0 1
#define JOYSTICK_BTN_1 1
#define JOYSTICK_BTN_2 1
#define JOYSTICK_BTN_3 1
#define JOYSTICK_BTN_4 1
#define JOYSTICK_BTN_5 1
#define JOYSTICK_BTN_6 1
#define JOYSTICK_BTN_7 1
#define JOYSTICK_BTN_8 1
#define JOYSTICK_BTN_9 1

#define JOYSTICK_AXIS_0 1
#define JOYSTICK_AXIS_1 0
#define JOYSTICK_AXIS_2 0
#define JOYSTICK_AXIS_3 0
#define JOYSTICK_AXIS_4 0
#define JOYSTICK_AXIS_5 0


namespace input
{

#ifdef NO_JOYSTICK
    constexpr unsigned N_JOYSTICK_BUTTONS = 0;
    constexpr unsigned N_JOYSTICK_AXES = 0;
#else

    constexpr unsigned N_JOYSTICK_BUTTONS = 
    JOYSTICK_BTN_0 +
    JOYSTICK_BTN_1 +
    JOYSTICK_BTN_2 +
    JOYSTICK_BTN_3 +
    JOYSTICK_BTN_4 +
    JOYSTICK_BTN_5 +
    JOYSTICK_BTN_6 +
    JOYSTICK_BTN_7 +
    JOYSTICK_BTN_8 +
    JOYSTICK_BTN_9;

    constexpr unsigned N_JOYSTICK_AXES = 
    JOYSTICK_AXIS_0 +
    JOYSTICK_AXIS_1 +
    JOYSTICK_AXIS_2 +
    JOYSTICK_AXIS_3 +
    JOYSTICK_AXIS_4 +
    JOYSTICK_AXIS_5;

#endif
}


/* joystick */

namespace input
{
	class JoystickInput
	{
	public:
		static constexpr auto btn_count = N_JOYSTICK_BUTTONS;
		static constexpr auto axis_count = N_JOYSTICK_AXES;

		
		enum class Btn : u32
		{
		#if JOYSTICK_BTN_0
			Btn_0,
		#endif
		#if JOYSTICK_BTN_1
			Btn_1,
		#endif
		#if JOYSTICK_BTN_2
			Btn_2,
		#endif
		#if JOYSTICK_BTN_3
			Btn_3,
		#endif
		#if JOYSTICK_BTN_4
			Btn_4,
		#endif
		#if JOYSTICK_BTN_5
			Btn_5,
		#endif
		#if JOYSTICK_BTN_6
			Btn_6,
		#endif
		#if JOYSTICK_BTN_7
			Btn_7,
		#endif
		#if JOYSTICK_BTN_8
			Btn_8,
		#endif
		#if JOYSTICK_BTN_9
			Btn_9,
		#endif

			Count,

		#if JOYSTICK_BTN_0
			North,
		#endif
		#if JOYSTICK_BTN_1
			South,
		#endif
		#if JOYSTICK_BTN_2
			West,
		#endif
		#if JOYSTICK_BTN_3
			East,
		#endif
		#if JOYSTICK_BTN_4
			Up,
		#endif
		#if JOYSTICK_BTN_5
			Down,
		#endif
		#if JOYSTICK_BTN_6
			Left,
		#endif
		#if JOYSTICK_BTN_7
			Right,
		#endif
		#if JOYSTICK_BTN_8
			Btn_8X,
		#endif
		#if JOYSTICK_BTN_9
			Btn_9X,
		#endif


			None
		};
	
		u64 handle = 0;
	
		union
		{
			ButtonState buttons[btn_count];

			struct
			{
			#if JOYSTICK_BTN_0
				ButtonState btn_0;
			#endif
			#if JOYSTICK_BTN_1
				ButtonState btn_1;
			#endif
			#if JOYSTICK_BTN_2
				ButtonState btn_2;
			#endif
			#if JOYSTICK_BTN_3
				ButtonState btn_3;
			#endif
			#if JOYSTICK_BTN_4
				ButtonState btn_4;
			#endif
			#if JOYSTICK_BTN_5
				ButtonState btn_5;
			#endif
			#if JOYSTICK_BTN_6
				ButtonState btn_6;
			#endif
			#if JOYSTICK_BTN_7
				ButtonState btn_7;
			#endif
			#if JOYSTICK_BTN_8
				ButtonState btn_8;
			#endif
			#if JOYSTICK_BTN_9
				ButtonState btn_9;
			#endif
			};

			// named buttons
			struct
			{
			#if JOYSTICK_BTN_0
				ButtonState north;
			#endif
			#if JOYSTICK_BTN_1
				ButtonState south;
			#endif
			#if JOYSTICK_BTN_2
				ButtonState west;
			#endif
			#if JOYSTICK_BTN_3
				ButtonState east;
			#endif
			#if JOYSTICK_BTN_4
				ButtonState up;
			#endif
			#if JOYSTICK_BTN_5
				ButtonState down;
			#endif
			#if JOYSTICK_BTN_6
				ButtonState left;
			#endif
			#if JOYSTICK_BTN_7
				ButtonState right;
			#endif
			#if JOYSTICK_BTN_8
				ButtonState btn_8;
			#endif
			#if JOYSTICK_BTN_9
				ButtonState btn_9;
			#endif
			} btn;
		};

		union
		{
			f32 axes[axis_count];

			struct
			{
			#if JOYSTICK_AXIS_0
				f32 axis_0;
			#endif
			#if JOYSTICK_AXIS_1
				f32 axis_1;
			#endif
			#if JOYSTICK_AXIS_2
				f32 axis_2;
			#endif
			#if JOYSTICK_AXIS_3
				f32 axis_3;
			#endif
			#if JOYSTICK_AXIS_4
				f32 axis_4;
			#endif
			#if JOYSTICK_AXIS_5
				f32 axis_5;
			#endif
			};
		};

		static constexpr const char* btn_str[btn_count] = {
			#if JOYSTICK_BTN_0
				"btn_0",
			#endif
			#if JOYSTICK_BTN_1
				"btn_1",
			#endif
			#if JOYSTICK_BTN_2
				"btn_2",
			#endif
			#if JOYSTICK_BTN_3
				"btn_3",
			#endif
			#if JOYSTICK_BTN_4
				"btn_4",
			#endif
			#if JOYSTICK_BTN_5
				"btn_5",
			#endif
			#if JOYSTICK_BTN_6
				"btn_6",
			#endif
			#if JOYSTICK_BTN_7
				"btn_7",
			#endif
			#if JOYSTICK_BTN_8
				"btn_8",
			#endif
			#if JOYSTICK_BTN_9
				"btn_9",
			#endif
		};

		static constexpr const char* axes_str[axis_count] = {
			#if JOYSTICK_AXIS_0
				"axis_0",
			#endif
			#if JOYSTICK_AXIS_1
				"axis_1",
			#endif
			#if JOYSTICK_AXIS_2
				"axis_2",
			#endif
			#if JOYSTICK_AXIS_3
				"axis_3",
			#endif
			#if JOYSTICK_AXIS_4
				"axis_4",
			#endif
			#if JOYSTICK_AXIS_5
				"axis_5",
			#endif
		};
	};
	
}

