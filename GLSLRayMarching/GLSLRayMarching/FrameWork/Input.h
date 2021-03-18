//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#ifndef _Input_h_
#define _Input_h_

#include "Platform.h"
#include "Component.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"

class Input
{
public:
	enum class KeyCode
	{
		None,	//	Not assigned(never returned as the result of a keystroke).
		Backspace,	//	The backspace key.
		Delete,	//	The forward delete key.
		Tab,	//	The tab key.
		Clear,	//	The Clear key.
		Return,	//	Return key.
		Pause,	//	Pause on PC machines.
		Escape,	//	Escape key.
		Space,	//	Space key.
		Keypad0,	//	Numeric keypad 0.
		Keypad1,	//	Numeric keypad 1.
		Keypad2,	//	Numeric keypad 2.
		Keypad3,	//	Numeric keypad 3.
		Keypad4,	//	Numeric keypad 4.
		Keypad5,	//	Numeric keypad 5.
		Keypad6,	//	Numeric keypad 6.
		Keypad7,	//	Numeric keypad 7.
		Keypad8,	//	Numeric keypad 8.
		Keypad9,	//	Numeric keypad 9.
		KeypadPeriod,	//  Numeric keypad '.'.
		KeypadDivide,	//  Numeric keypad '/'.
		KeypadMultiply,	//  Numeric keypad '*'.
		KeypadMinus,	// Numeric keypad '-'.
		KeypadPlus,	// Numeric keypad '+'.
		KeypadEnter,	// Numeric keypad Enter.
		KeypadEquals,	// Numeric keypad '='.
		UpArrow,	// Up arrow key.
		DownArrow,	// Down arrow key.
		RightArrow,	// Right arrow key.
		LeftArrow,	// Left arrow key.
		Insert,	// Insert key key.
		Home,	// Home key.
		End,	// End key.
		PageUp,	// Page up.
		PageDown,	// Page down.
		F1,	// F1 function key.
		F2,	// F2 function key.
		F3,	// F3 function key.
		F4,	// F4 function key.
		F5,	// F5 function key.
		F6,	// F6 function key.
		F7,	// F7 function key.
		F8,	// F8 function key.
		F9,	// F9 function key.
		F10,	// F10 function key.
		F11,	// F11 function key.
		F12,	// F12 function key.
		F13,	// F13 function key.
		F14,	// F14 function key.
		F15,	// F15 function key.
		Alpha0,	// The '0' key on the top of the alphanumeric keyboard.
		Alpha1,	// The '1' key on the top of the alphanumeric keyboard.
		Alpha2,	// The '2' key on the top of the alphanumeric keyboard.
		Alpha3,	// The '3' key on the top of the alphanumeric keyboard.
		Alpha4,	// The '4' key on the top of the alphanumeric keyboard.
		Alpha5,	// The '5' key on the top of the alphanumeric keyboard.
		Alpha6,	// The '6' key on the top of the alphanumeric keyboard.
		Alpha7,	// The '7' key on the top of the alphanumeric keyboard.
		Alpha8,	// The '8' key on the top of the alphanumeric keyboard.
		Alpha9,	// The '9' key on the top of the alphanumeric keyboard.
		Exclaim,	// Exclamation mark key '!'.
		DoubleQuote,	// Double quote key '"'.
		Hash,	// Hash key '#'.
		Dollar,	// Dollar sign key '$'.
		Percent,	// Percent '%' key.
		Ampersand,	// Ampersand key '&'.
		Quote,	// Quote key '.
		LeftParen,	// Left Parenthesis key '('.
		RightParen,	// Right Parenthesis key ')'.
		Asterisk,	// Asterisk key '*'.
		Plus,	// Plus key '+'.
		Comma,	// Comma ',' key.
		Minus,	// Minus '-' key.
		Period,	// Period '.' key.
		Slash,	// Slash '/' key.
		Colon,	// Colon ':' key.
		Semicolon,	// Semicolon ';' key.
		Less,	// Less than '<' key.
		Equals,	// Equals '=' key.
		Greater,	// Greater than '>' key.
		Question,	// Question mark '?' key.
		At,	// At key '@'.
		LeftBracket,	// Left square bracket key '['.
		Backslash,	// Backslash key '\'.
		RightBracket,	// Right square bracket key ']'.
		Caret,	// Caret key '^'.
		Underscore,	// Underscore '_' key.
		BackQuote,	// Back quote key '`'.
		A,	// 'a' key.
		B,	// 'b' key.
		C,	// 'c' key.
		D,	// 'd' key.
		E,	// 'e' key.
		F,	// 'f' key.
		G,	// 'g' key.
		H,	// 'h' key.
		I,	// 'i' key.
		J,	// 'j' key.
		K,	// 'k' key.
		L,	// 'l' key.
		M,	// 'm' key.
		N,	// 'n' key.
		O,	// 'o' key.
		P,	// 'p' key.
		Q,	// 'q' key.
		R,	// 'r' key.
		S,	// 's' key.
		T,	// 't' key.
		U,	// 'u' key.
		V,	// 'v' key.
		W,	// 'w' key.
		X,	// 'x' key.
		Y,	// 'y' key.
		Z,	// 'z' key.
		LeftCurlyBracket,	// Left curly bracket key '{'.
		Pipe,	// Pipe '|' key.
		RightCurlyBracket,	// Right curly bracket key '}'.
		Tilde,	// Tilde '~' key.
		Numlock,	// Numlock key.
		CapsLock,	// Capslock key.
		ScrollLock,	// Scroll lock key.
		RightShift,	// Right shift key.
		LeftShift,	// Left shift key.
		RightControl,	// Right Control key.
		LeftControl,	// Left Control key.
		RightAlt,	// Right Alt key.
		LeftAlt,	// Left Alt key.
		LeftCommand,	// Left Command key.
		LeftApple,	// Left Command key.
		LeftWindows,	// Left Windows key.
		RightCommand,	// Right Command key.
		RightApple,	// Right Command key.
		RightWindows,	// Right Windows key.
		Help,	// Help key.
		Print,	// Print key.
		Break,	// Break key.
		Menu,	// Menu key.
		Mouse0,	// The Left(or primary) mouse button.
		Mouse1,	// Right mouse button(or secondary mouse button).
		Mouse2,	// Middle mouse button(or third button).
		Mouse3,	// Additional(fourth) mouse button.
		Mouse4,	// Additional(fifth) mouse button.
		Mouse5,	// Additional(or sixth) mouse button.
		Mouse6,	// Additional(or seventh) mouse button.
		JoystickButton0,	// Button 0 on any joystick.
		JoystickButton1,	// Button 1 on any joystick.
		JoystickButton2,	// Button 2 on any joystick.
		JoystickButton3,	// Button 3 on any joystick.
		JoystickButton4,	// Button 4 on any joystick.
		JoystickButton5,	// Button 5 on any joystick.
		JoystickButton6,	// Button 6 on any joystick.
		JoystickButton7,	// Button 7 on any joystick.
		JoystickButton8,	// Button 8 on any joystick.
		JoystickButton9,	// Button 9 on any joystick.
		JoystickButton10,	// Button 10 on any joystick.
		JoystickButton11,	// Button 11 on any joystick.
		JoystickButton12,	// Button 12 on any joystick.
		JoystickButton13,	// Button 13 on any joystick.
		JoystickButton14,	// Button 14 on any joystick.
		JoystickButton15,	// Button 15 on any joystick.
		JoystickButton16,	// Button 16 on any joystick.
		JoystickButton17,	// Button 17 on any joystick.
		JoystickButton18,	// Button 18 on any joystick.
		JoystickButton19,	// Button 19 on any joystick.
		Joystick1Button0,	// Button 0 on first joystick.
		Joystick1Button1,	// Button 1 on first joystick.
		Joystick1Button2,	// Button 2 on first joystick.
		Joystick1Button3,	// Button 3 on first joystick.
		Joystick1Button4,	// Button 4 on first joystick.
		Joystick1Button5,	// Button 5 on first joystick.
		Joystick1Button6,	// Button 6 on first joystick.
		Joystick1Button7,	// Button 7 on first joystick.
		Joystick1Button8,	// Button 8 on first joystick.
		Joystick1Button9,	// Button 9 on first joystick.
		Joystick1Button10,	// Button 10 on first joystick.
		Joystick1Button11,	// Button 11 on first joystick.
		Joystick1Button12,	// Button 12 on first joystick.
		Joystick1Button13,	// Button 13 on first joystick.
		Joystick1Button14,	// Button 14 on first joystick.
		Joystick1Button15,	// Button 15 on first joystick.
		Joystick1Button16,	// Button 16 on first joystick.
		Joystick1Button17,	// Button 17 on first joystick.
		Joystick1Button18,	// Button 18 on first joystick.
		Joystick1Button19,	// Button 19 on first joystick.
		Joystick2Button0,	// Button 0 on second joystick.
		Joystick2Button1,	// Button 1 on second joystick.
		Joystick2Button2,	// Button 2 on second joystick.
		Joystick2Button3,	// Button 3 on second joystick.
		Joystick2Button4,	// Button 4 on second joystick.
		Joystick2Button5,	// Button 5 on second joystick.
		Joystick2Button6,	// Button 6 on second joystick.
		Joystick2Button7,	// Button 7 on second joystick.
		Joystick2Button8,	// Button 8 on second joystick.
		Joystick2Button9,	// Button 9 on second joystick.
		Joystick2Button10,	// Button 10 on second joystick.
		Joystick2Button11,	// Button 11 on second joystick.
		Joystick2Button12,	// Button 12 on second joystick.
		Joystick2Button13,	// Button 13 on second joystick.
		Joystick2Button14,	// Button 14 on second joystick.
		Joystick2Button15,	// Button 15 on second joystick.
		Joystick2Button16,	// Button 16 on second joystick.
		Joystick2Button17,	// Button 17 on second joystick.
		Joystick2Button18,	// Button 18 on second joystick.
		Joystick2Button19,	// Button 19 on second joystick.
		Joystick3Button0,	// Button 0 on third joystick.
		Joystick3Button1,	// Button 1 on third joystick.
		Joystick3Button2,	// Button 2 on third joystick.
		Joystick3Button3,	// Button 3 on third joystick.
		Joystick3Button4,	// Button 4 on third joystick.
		Joystick3Button5,	// Button 5 on third joystick.
		Joystick3Button6,	// Button 6 on third joystick.
		Joystick3Button7,	// Button 7 on third joystick.
		Joystick3Button8,	// Button 8 on third joystick.
		Joystick3Button9,	// Button 9 on third joystick.
		Joystick3Button10,	// Button 10 on third joystick.
		Joystick3Button11,	// Button 11 on third joystick.
		Joystick3Button12,	// Button 12 on third joystick.
		Joystick3Button13,	// Button 13 on third joystick.
		Joystick3Button14,	// Button 14 on third joystick.
		Joystick3Button15,	// Button 15 on third joystick.
		Joystick3Button16,	// Button 16 on third joystick.
		Joystick3Button17,	// Button 17 on third joystick.
		Joystick3Button18,	// Button 18 on third joystick.
		Joystick3Button19,	// Button 19 on third joystick.
		Joystick4Button0,	// Button 0 on forth joystick.
		Joystick4Button1,	// Button 1 on forth joystick.
		Joystick4Button2,	// Button 2 on forth joystick.
		Joystick4Button3,	// Button 3 on forth joystick.
		Joystick4Button4,	// Button 4 on forth joystick.
		Joystick4Button5,	// Button 5 on forth joystick.
		Joystick4Button6,	// Button 6 on forth joystick.
		Joystick4Button7,	// Button 7 on forth joystick.
		Joystick4Button8,	// Button 8 on forth joystick.
		Joystick4Button9,	// Button 9 on forth joystick.
		Joystick4Button10,	// Button 10 on forth joystick.
		Joystick4Button11,	// Button 11 on forth joystick.
		Joystick4Button12,	// Button 12 on forth joystick.
		Joystick4Button13,	// Button 13 on forth joystick.
		Joystick4Button14,	// Button 14 on forth joystick.
		Joystick4Button15,	// Button 15 on forth joystick.
		Joystick4Button16,	// Button 16 on forth joystick.
		Joystick4Button17,	// Button 17 on forth joystick.
		Joystick4Button18,	// Button 18 on forth joystick.
		Joystick4Button19,	// Button 19 on forth joystick.
		Joystick5Button0,	// Button 0 on fifth joystick.
		Joystick5Button1,	// Button 1 on fifth joystick.
		Joystick5Button2,	// Button 2 on fifth joystick.
		Joystick5Button3,	// Button 3 on fifth joystick.
		Joystick5Button4,	// Button 4 on fifth joystick.
		Joystick5Button5,	// Button 5 on fifth joystick.
		Joystick5Button6,	// Button 6 on fifth joystick.
		Joystick5Button7,	// Button 7 on fifth joystick.
		Joystick5Button8,	// Button 8 on fifth joystick.
		Joystick5Button9,	// Button 9 on fifth joystick.
		Joystick5Button10,	// Button 10 on fifth joystick.
		Joystick5Button11,	// Button 11 on fifth joystick.
		Joystick5Button12,	// Button 12 on fifth joystick.
		Joystick5Button13,	// Button 13 on fifth joystick.
		Joystick5Button14,	// Button 14 on fifth joystick.
		Joystick5Button15,	// Button 15 on fifth joystick.
		Joystick5Button16,	// Button 16 on fifth joystick.
		Joystick5Button17,	// Button 17 on fifth joystick.
		Joystick5Button18,	// Button 18 on fifth joystick.
		Joystick5Button19,	// Button 19 on fifth joystick.
		Joystick6Button0,	// Button 0 on sixth joystick.
		Joystick6Button1,	// Button 1 on sixth joystick.
		Joystick6Button2,	// Button 2 on sixth joystick.
		Joystick6Button3,	// Button 3 on sixth joystick.
		Joystick6Button4,	// Button 4 on sixth joystick.
		Joystick6Button5,	// Button 5 on sixth joystick.
		Joystick6Button6,	// Button 6 on sixth joystick.
		Joystick6Button7,	// Button 7 on sixth joystick.
		Joystick6Button8,	// Button 8 on sixth joystick.
		Joystick6Button9,	// Button 9 on sixth joystick.
		Joystick6Button10,	// Button 10 on sixth joystick.
		Joystick6Button11,	// Button 11 on sixth joystick.
		Joystick6Button12,	// Button 12 on sixth joystick.
		Joystick6Button13,	// Button 13 on sixth joystick.
		Joystick6Button14,	// Button 14 on sixth joystick.
		Joystick6Button15,	// Button 15 on sixth joystick.
		Joystick6Button16,	// Button 16 on sixth joystick.
		Joystick6Button17,	// Button 17 on sixth joystick.
		Joystick6Button18,	// Button 18 on sixth joystick.
		Joystick6Button19,	// Button 19 on sixth joystick.
		Joystick7Button0,	// Button 0 on seventh joystick.
		Joystick7Button1,	// Button 1 on seventh joystick.
		Joystick7Button2,	// Button 2 on seventh joystick.
		Joystick7Button3,	// Button 3 on seventh joystick.
		Joystick7Button4,	// Button 4 on seventh joystick.
		Joystick7Button5,	// Button 5 on seventh joystick.
		Joystick7Button6,	// Button 6 on seventh joystick.
		Joystick7Button7,	// Button 7 on seventh joystick.
		Joystick7Button8,	// Button 8 on seventh joystick.
		Joystick7Button9,	// Button 9 on seventh joystick.
		Joystick7Button10,	// Button 10 on seventh joystick.
		Joystick7Button11,	// Button 11 on seventh joystick.
		Joystick7Button12,	// Button 12 on seventh joystick.
		Joystick7Button13,	// Button 13 on seventh joystick.
		Joystick7Button14,	// Button 14 on seventh joystick.
		Joystick7Button15,	// Button 15 on seventh joystick.
		Joystick7Button16,	// Button 16 on seventh joystick.
		Joystick7Button17,	// Button 17 on seventh joystick.
		Joystick7Button18,	// Button 18 on seventh joystick.
		Joystick7Button19,	// Button 19 on seventh joystick.
		Joystick8Button0,	// Button 0 on eighth joystick.
		Joystick8Button1,	// Button 1 on eighth joystick.
		Joystick8Button2,	// Button 2 on eighth joystick.
		Joystick8Button3,	// Button 3 on eighth joystick.
		Joystick8Button4,	// Button 4 on eighth joystick.
		Joystick8Button5,	// Button 5 on eighth joystick.
		Joystick8Button6,	// Button 6 on eighth joystick.
		Joystick8Button7,	// Button 7 on eighth joystick.
		Joystick8Button8,	// Button 8 on eighth joystick.
		Joystick8Button9,	// Button 9 on eighth joystick.
		Joystick8Button10,	// Button 10 on eighth joystick.
		Joystick8Button11,	// Button 11 on eighth joystick.
		Joystick8Button12,	// Button 12 on eighth joystick.
		Joystick8Button13,	// Button 13 on eighth joystick.
		Joystick8Button14,	// Button 14 on eighth joystick.
		Joystick8Button15,	// Button 15 on eighth joystick.
		Joystick8Button16,	// Button 16 on eighth joystick.
		Joystick8Button17,	// Button 17 on eighth joystick.
		Joystick8Button18,	// Button 18 on eighth joystick.
		Joystick8Button19,	// Button 19 on eighth joystick.
	};

	enum class Type
	{
		KeyOrMouseButton = 0,
		MouseMovement,
		JoystickAxis
	};

	enum class Axis
	{
		MouseX = 1,
		MouseY,
		Axis1 = 1,
		Axis2,
		Axis4,
		Axis5,
		Axis6,
		Axis7,
		Axis8,
		Axis9,
		Axis10,
		Axis11,
		Axis12,
		Axis13,
		Axis14,
		Axis15,
		Axis16,
		Axis17,
		Axis18,
		Axis19,
		Axis20,
		Axis21,
		Axis22,
		Axis23,
		Axis24,
		Axis25,
		Axis26,
		Axis27,
		Axis28
	};

	enum class JoyStick
	{
		JOYSTICK_ANY = 0,
		JOYSTICK_1,
		JOYSTICK_2,
		JOYSTICK_4,
		JOYSTICK_5,
		JOYSTICK_6,
		JOYSTICK_7,
		JOYSTICK_8,
		JOYSTICK_9,
		JOYSTICK_10,
		JOYSTICK_11,
		JOYSTICK_12,
		JOYSTICK_13,
		JOYSTICK_14,
		JOYSTICK_15,
		JOYSTICK_16
	};

	/*
	class Compass
	{
	public:
		bool enabled;			// Used to enable or disable compass.Note, that if you want Input.compass.trueHeading property to contain a valid value, you must also enable location updates by calling Input.location.Start().
		float headingAccuracy;	// Accuracy of heading reading in degrees.
		float magneticHeading;	// The heading in degrees relative to the magnetic North Pole. (Read Only)
		Vector3 rawVector;		// The raw geomagnetic data measured in microteslas. (Read Only)
		double timestamp;		// Timestamp(in seconds since 1970) when the heading was last time updated. (Read Only)
		float trueHeading;		// The heading in degrees relative to the geographic North Pole. (Read Only)
	};

	// enumerationLeave feedback
	// Description
	// Describes physical orientation of the device as determined by the OS.
	// 
	// If device is physically situated between discrete positions, as when(for example) rotated diagonally, system will report Unknown orientation.
	// 
	// Properties
	enum class DeviceOrientation
	{
		Unknown = 0,		// The orientation of the device cannot be determined.
		Portrait,			// The device is in portrait mode, with the device held uprightand the home button at the bottom.
		PortraitUpsideDown, // The device is in portrait mode but upside down, with the device held uprightand the home button at the top.
		LandscapeLeft,		// The device is in landscape mode, with the device held uprightand the home button on the right side.
		LandscapeRight,		// The device is in landscape mode, with the device held uprightand the home button on the left side.
		FaceUp,				// The device is held parallel to the ground with the screen facing upwards.
		FaceDown			// The device is held parallel to the ground with the screen facing downwards.
	};

	class Gyroscope
	{
	public:
		Quaternion attitude;			//	Returns the attitude(ie, orientation in space) of the device.
		bool enabled;					//	Sets or retrieves the enabled status of this gyroscope.
		Vector3 gravity;				//	Returns the gravity acceleration vector expressed in the device's reference frame.
		Vector3 rotationRate;			//	Returns rotation rate as measured by the device's gyroscope.
		Vector3 rotationRateUnbiased;	//	Returns unbiased rotation rate as measured by the device's gyroscope.
		float updateInterval;			//	Sets or retrieves gyroscope interval in seconds.
		Vector3 userAcceleration;		//	Returns the acceleration that the user is giving to the device.
	};

	enum class IMECompositionMode
	{
		Auto = 0,
		On,
		Off,
	};

	class LocationService
	{
	public:
		bool isEnabledByUser;		      //  Specifies whether location service is enabled in user settings.
		//LocationInfo lastData;		  //  Last measured device geographical location.
		//LocationServiceStatus status;   //  Returns location service status.
	};

	// static Vector3 acceleration;											// Last measured linear acceleration of a device in three - dimensional space. (Read Only)
	// static int accelerationEventCount;									// Number of acceleration measurements which occurred during last frame.
	// static std::vector<Vector3> accelerationEvents;						// Returns list of acceleration measurements which occurred during the last frame. (Read Only) (Allocates temporary variables).
	// static bool anyKey;													// Is any key or mouse button currently held down ? (Read Only)
	// static bool anyKeyDown;												// Returns true the first frame the user hits any key or mouse button. (Read Only)
	// static bool backButtonLeavesApp;										// Should Back button quit the application ? Only usable on Android, Windows Phone or Windows Tablets.
	// static Compass compass;												// Property for accessing compass(handheld devices only). (Read Only)
	// static bool compensateSensors;										// This property controls if input sensors should be compensated for screen orientation.
	// static Vector2 compositionCursorPos;									// The current text input position used by IMEs to open windows.
	// static std::string compositionString;								// The current IME composition string being typed by the user.
	// static DeviceOrientation deviceOrientation;							// Device physical orientation as reported by OS. (Read Only)
	// static Gyroscope gyro;												// Returns default gyroscope.
	// static IMECompositionMode imeCompositionMode;						// Controls enablingand disabling of IME input composition.
	// static bool imeIsSelected;											// Does the user have an IME keyboard input source selected ?
	// static std::string inputString;										// Returns the keyboard input entered this frame. (Read Only)
	// static LocationService	location;									// Property for accessing device location(handheld devices only). (Read Only)
	// static Vector3 mousePosition;										// The current mouse position in pixel coordinates. (Read Only).
	// static bool mousePresent;											// Indicates if a mouse device is detected.
	// static Vector2 mouseScrollDelta;										// The current mouse scroll delta. (Read Only)
	// static bool multiTouchEnabled;										// Property indicating whether the system handles multiple touches.
	// static bool simulateMouseWithTouches;								// Enables / Disables mouse simulation with touches.By default this option is enabled.
	// static bool stylusTouchSupported;									// Returns true when Stylus Touch is supported by a device or platform.
	// static int touchCount;												// Number of touches.Guaranteed not to change throughout the frame. (Read Only)
	// static std::vector<Touch> touches;									// Returns list of objects representing status of all touches during last frame. (Read Only) (Allocates temporary variables).
	// static bool touchPressureSupported;									// Bool value which let's users check if touch pressure is supported.
	// static bool touchSupported;											// Returns whether the device on which application is currently running supports touch input.
	*/

	class AccelerationEvent
	{
	public:
		Vector3 acceleration;
		float deltaTime;
	};

	class Touch
	{
	public:
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////
	Input();
	Input(const std::string& name_,
		const std::string& descriptivePositiveName_,
		const std::string& descriptiveNegativeName_,
		KeyCode positiveButton_,
		KeyCode negativeButton_,
		KeyCode positiveAltButton_,
		KeyCode negativeAltButton_,
		float gravity_,
		float deadzone_,
		float sensitivity_,
		bool snap_,
		bool invert_,
		Type type_,
		Axis axis_,
		JoyStick joystick_);
	virtual ~Input();
	float GetAxis();
	float GetAxisRaw();
private:
	void Update();
	void UpdateKeyOrMouseButton();
	void UpdateMouseMovement();
	void UpdateJoystickAxis();	

	std::string name;
	std::string descriptivePositiveName;
	std::string descriptiveNegativeName;
	KeyCode positiveButton;
	KeyCode negativeButton;
	KeyCode positiveAltButton;
	KeyCode negativeAltButton;
	float gravity;
	float deadzone;
	float sensitivity;
	bool snap;
	bool invert;
	Type type;
	Axis axis;
	JoyStick joystick;

	float lastValue;
	float currentValue;
	float rawValue;

////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	class Service;

	class Manager
	{
		friend class Input;
		friend class Service;
		Manager();
		~Manager();
	public:
		static Input::Manager& GetInstance();

		Input& GetInput(const char* axisName);

		float GetAxis(const char* axisName);							// Returns the value of the virtual axis identified by axisName.
		float GetAxisRaw(const char* axisName);							// Returns the value of the virtual axis identified by axisName with no smoothing filtering applied.
		bool GetButton(const char* axisName);							// Returns true while the virtual button identified by axisName is held down.
		bool GetButtonDown(const char* axisName);						// Returns true during the frame the user pressed down the virtual button identified by axisName.
		bool GetButtonUp(const char* axisName);							// Returns true the first frame the user releases the virtual button identified by axisName.
		
		bool GetKey(Input::KeyCode keyCode);							// Returns true while the user holds down the key identified by buttonName.
		bool GetKeyDown(Input::KeyCode keyCode);						// Returns true during the frame the user starts pressing down the key identified by buttonName.
		bool GetKeyUp(Input::KeyCode keyCode);							// Returns true during the frame the user releases the key identified by buttonName.
		
		bool GetMouseButton(int button);								// Returns whether the given mouse button is held down.
		bool GetMouseButtonDown(int button);							// Returns true during the frame the user pressed the given mouse button.
		bool GetMouseButtonUp(int button); 								// Returns true during the frame the user releases the given mouse button.
		Vector2 GetMouseMovement();

		//const std::vector<std::string>& GetJoystickaxisNames();		// Retrieves a list of input device axisNames corresponding to the index of an Axis configured within Input Manager.

		// Touch GetTouch(int index);									// Call Input.GetTouch to obtain a Touch struct.
		const AccelerationEvent& GetAccelerationEvent(int i) const;		// Returns specific acceleration measurement which occurred during last frame. (Does not allocate temporary variables).
	private:
		int GetKeyValue(Input::KeyCode keyCode);

		bool Initialize();
		bool Process();
		bool Pause();
		void Resume();
		void Terminate();
	private:
		std::map<const char*, Input> inputs;
		std::vector<AccelerationEvent> accelerationEvents;

		std::vector<bool> lastValues;
		std::vector<bool> currentValues;
		Vector2 mouseMovement;
	};

	///////////////////////////////////////////////////////////////////////
public:
	class Service
	{
	public:
		static bool Initialize();
		static bool Process();
		static bool Pause();
		static void Resume();
		static void Terminate();
	};
};

#endif