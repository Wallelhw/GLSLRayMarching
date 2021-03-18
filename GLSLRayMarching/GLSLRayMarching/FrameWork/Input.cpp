//////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2020, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk          //
//																				//
// Author : Mac Lin																//
// Module : Magnum Engine v0.7.0												//
// Date   : 05/Nov/2020															//
//																				//
//////////////////////////////////////////////////////////////////////////////////
#include "Platform.h"
#include "Input.h"

#include <Windows.h>
#include <WinUser.h>

std::map<Input::KeyCode, int> keyCode2VKs =
{
	{ Input::KeyCode::None					,		0			},	//	Not assigned(never returned as the result of a keystroke).
	{ Input::KeyCode::Backspace				,	VK_BACK			},	//	The backspace key.
	{ Input::KeyCode::Delete				,	VK_DELETE		},	//	The forward delete key.
	{ Input::KeyCode::Tab					,	VK_TAB			},	//	The tab key.
	{ Input::KeyCode::Clear					,	VK_CLEAR		},  //	The Clear key.
	{ Input::KeyCode::Return				,	VK_RETURN		},	//	Return key.
	{ Input::KeyCode::Pause					,	VK_PAUSE		},	//	Pause on PC machines.
	{ Input::KeyCode::Escape				,	VK_ESCAPE		},	//	Escape key.
	{ Input::KeyCode::Space					,	VK_SPACE		},	//	Space key.
	{ Input::KeyCode::Keypad0				,	VK_NUMPAD0		},	//	Numeric keypad 0.
	{ Input::KeyCode::Keypad1				,	VK_NUMPAD1		},	//	Numeric keypad 1.
	{ Input::KeyCode::Keypad2				,	VK_NUMPAD2		},	//	Numeric keypad 2.
	{ Input::KeyCode::Keypad3				,	VK_NUMPAD3		},	//	Numeric keypad 3.
	{ Input::KeyCode::Keypad4				,	VK_NUMPAD4		},	//	Numeric keypad 4.
	{ Input::KeyCode::Keypad5				,	VK_NUMPAD5		},	//	Numeric keypad 5.
	{ Input::KeyCode::Keypad6				,	VK_NUMPAD6		},	//	Numeric keypad 6.
	{ Input::KeyCode::Keypad7				,	VK_NUMPAD7		},	//	Numeric keypad 7.
	{ Input::KeyCode::Keypad8				,	VK_NUMPAD8		},	//	Numeric keypad 8.
	{ Input::KeyCode::Keypad9				,	VK_NUMPAD9		},	//	Numeric keypad 9.
	{ Input::KeyCode::KeypadPeriod			,	VK_OEM_PERIOD	},	//  Numeric keypad '.'.
	{ Input::KeyCode::KeypadDivide			,	VK_DIVIDE		},	//  Numeric keypad '/'.
	{ Input::KeyCode::KeypadMultiply		,	VK_MULTIPLY		},	//  Numeric keypad '*'.
	{ Input::KeyCode::KeypadMinus			,	VK_OEM_MINUS	},	// Numeric keypad '-'.
	{ Input::KeyCode::KeypadPlus			,	VK_OEM_PLUS		},	// Numeric keypad '+'.
	{ Input::KeyCode::KeypadEnter			,	VK_RETURN		},	// Numeric keypad Enter.
	{ Input::KeyCode::KeypadEquals			,	VK_OEM_NEC_EQUAL},	// Numeric keypad '='.
	{ Input::KeyCode::UpArrow				,	VK_UP			},	// Up arrow key.
	{ Input::KeyCode::DownArrow				,	VK_DOWN			},	// Down arrow key.
	{ Input::KeyCode::RightArrow			,	VK_RIGHT		},	// Right arrow key.
	{ Input::KeyCode::LeftArrow				,	VK_LEFT			},	// Left arrow key.
	{ Input::KeyCode::Insert				,	VK_INSERT		},	// Insert key key.
	{ Input::KeyCode::Home					,	VK_HOME			},	// Home key.
	{ Input::KeyCode::End					,	VK_END			},	// End key.
	{ Input::KeyCode::PageUp				,	VK_PRIOR		},	// Page up.
	{ Input::KeyCode::PageDown				,	VK_NEXT			},	// Page down.
	{ Input::KeyCode::F1					,	VK_F1			}, // F1 function key.
	{ Input::KeyCode::F2					,	VK_F2			}, // F2 function key.
	{ Input::KeyCode::F3					,	VK_F3			}, // F3 function key.
	{ Input::KeyCode::F4					,	VK_F4			}, // F4 function key.
	{ Input::KeyCode::F5					,	VK_F5			}, // F5 function key.
	{ Input::KeyCode::F6					,	VK_F6			}, // F6 function key.
	{ Input::KeyCode::F7					,	VK_F7			}, // F7 function key.
	{ Input::KeyCode::F8					,	VK_F8			}, // F8 function key.
	{ Input::KeyCode::F9					,	VK_F9			}, // F9 function key.
	{ Input::KeyCode::F10					,	VK_F10			}, // F10 function key.
	{ Input::KeyCode::F11					,	VK_F11			}, // F11 function key.
	{ Input::KeyCode::F12					,	VK_F12			}, // F12 function key.
	{ Input::KeyCode::F13					,	VK_F13			}, // F13 function key.
	{ Input::KeyCode::F14					,	VK_F14			}, // F14 function key.
	{ Input::KeyCode::F15					,	VK_F15			}, // F15 function key.
	{ Input::KeyCode::Alpha0				,	'0'				}, // The '0' key on the top of the alphanumeric keyboard.
	{ Input::KeyCode::Alpha1				,	'1'				}, // The '1' key on the top of the alphanumeric keyboard.
	{ Input::KeyCode::Alpha2				,	'2'				}, // The '2' key on the top of the alphanumeric keyboard.
	{ Input::KeyCode::Alpha3				,	'3'				}, // The '3' key on the top of the alphanumeric keyboard.
	{ Input::KeyCode::Alpha4				,	'4'				}, // The '4' key on the top of the alphanumeric keyboard.
	{ Input::KeyCode::Alpha5				,	'5'				}, // The '5' key on the top of the alphanumeric keyboard.
	{ Input::KeyCode::Alpha6				,	'6'				}, // The '6' key on the top of the alphanumeric keyboard.
	{ Input::KeyCode::Alpha7				,	'7'				}, // The '7' key on the top of the alphanumeric keyboard.
	{ Input::KeyCode::Alpha8				,	'8'				}, // The '8' key on the top of the alphanumeric keyboard.
	{ Input::KeyCode::Alpha9				,	'9'				}, // The '9' key on the top of the alphanumeric keyboard.
	{ Input::KeyCode::Exclaim				,	'!'				}, // Exclamation mark key '!'.
	{ Input::KeyCode::DoubleQuote			,	'"'				}, // Double quote key '"'.
	{ Input::KeyCode::Hash					,	'#'				}, // Hash key '#'.
	{ Input::KeyCode::Dollar				,	'$'				}, // Dollar sign key '$'.
	{ Input::KeyCode::Percent				,	'%'				}, // Percent '%' key.
	{ Input::KeyCode::Ampersand				,	'&'				}, // Ampersand key '&'.
	{ Input::KeyCode::Quote					,	'.'				}, // Quote key '.
	{ Input::KeyCode::LeftParen				,	'('				}, // Left Parenthesis key '('.
	{ Input::KeyCode::RightParen			,	')'				}, // Right Parenthesis key ')'.
	{ Input::KeyCode::Asterisk				,	'*'				}, // Asterisk key '*'.
	{ Input::KeyCode::Plus					,	'+'				}, // Plus key '+'.
	{ Input::KeyCode::Comma					,	','				}, // Comma ',' key.
	{ Input::KeyCode::Minus					,	'-'				}, // Minus '-' key.
	{ Input::KeyCode::Period				,	'.'				}, // Period '.' key.
	{ Input::KeyCode::Slash					,	'/'				}, // Slash '/' key.
	{ Input::KeyCode::Colon					,	':'				}, // Colon ':' key.
	{ Input::KeyCode::Semicolon				,	';'				}, // Semicolon ';' key.
	{ Input::KeyCode::Less					,	'<'				}, // Less than '<' key.
	{ Input::KeyCode::Equals				,	'='				}, // Equals '=' key.
	{ Input::KeyCode::Greater				,	'>'				}, // Greater than '>' key.
	{ Input::KeyCode::Question				,	'?'				}, // Question mark '?' key.
	{ Input::KeyCode::At					,	'@'				}, // At key '@'.
	{ Input::KeyCode::LeftBracket			,	'['				}, // Left square bracket key '['.
	{ Input::KeyCode::Backslash				,	'\\'			}, // Backslash key '\'.
	{ Input::KeyCode::RightBracket			,	']'				}, // Right square bracket key ']'.
	{ Input::KeyCode::Caret					,	'^'				}, // Caret key '^'.
	{ Input::KeyCode::Underscore			,	'_'				}, // Underscore '_' key.
	{ Input::KeyCode::BackQuote				,	'`'				}, // Back quote key '`'.
	{ Input::KeyCode::A						,	'a'				}, // 'a' key.
	{ Input::KeyCode::B						,	'b'				}, // 'b' key.
	{ Input::KeyCode::C						,	'c'				}, // 'c' key.
	{ Input::KeyCode::D						,	'd'				}, // 'd' key.
	{ Input::KeyCode::E						,	'e'				}, // 'e' key.
	{ Input::KeyCode::F						,	'f'				}, // 'f' key.
	{ Input::KeyCode::G						,	'g'				}, // 'g' key.
	{ Input::KeyCode::H						,	'h'				}, // 'h' key.
	{ Input::KeyCode::I						,	'i'				}, // 'i' key.
	{ Input::KeyCode::J						,	'j'				}, // 'j' key.
	{ Input::KeyCode::K						,	'k'				}, // 'k' key.
	{ Input::KeyCode::L						,	'l'				}, // 'l' key.
	{ Input::KeyCode::M						,	'm'				}, // 'm' key.
	{ Input::KeyCode::N						,	'n'				}, // 'n' key.
	{ Input::KeyCode::O						,	'o'				}, // 'o' key.
	{ Input::KeyCode::P						,	'p'				}, // 'p' key.
	{ Input::KeyCode::Q						,	'q'				}, // 'q' key.
	{ Input::KeyCode::R						,	'r'				}, // 'r' key.
	{ Input::KeyCode::S						,	's'				}, // 's' key.
	{ Input::KeyCode::T						,	't'				}, // 't' key.
	{ Input::KeyCode::U						,	'u'				}, // 'u' key.
	{ Input::KeyCode::V						,	'v'				}, // 'v' key.
	{ Input::KeyCode::W						,	'w'				}, // 'w' key.
	{ Input::KeyCode::X						,	'x'				}, // 'x' key.
	{ Input::KeyCode::Y						,	'y'				}, // 'y' key.
	{ Input::KeyCode::Z						,	'z'				}, // 'z' key.
	{ Input::KeyCode::LeftCurlyBracket		,	'{'				}, // Left curly bracket key '{'.
	{ Input::KeyCode::Pipe					,	'|'				}, // Pipe '|' key.
	{ Input::KeyCode::RightCurlyBracket		,	'}'				}, // Right curly bracket key '}'.
	{ Input::KeyCode::Tilde					,	'~'				}, // Tilde '~' key.
	{ Input::KeyCode::Numlock				,	VK_NUMLOCK		}, // Numlock key.
	{ Input::KeyCode::CapsLock				,	VK_CAPITAL		}, // Capslock key.
	{ Input::KeyCode::ScrollLock			,	VK_SCROLL		}, // Scroll lock key.
	{ Input::KeyCode::RightShift			,	VK_RSHIFT		}, // Right shift key.
	{ Input::KeyCode::LeftShift				,	VK_LSHIFT		}, // Left shift key.
	{ Input::KeyCode::RightControl			,	VK_RCONTROL		}, // Right Control key.
	{ Input::KeyCode::LeftControl			,	VK_LCONTROL		}, // Left Control key.
	{ Input::KeyCode::RightAlt				,	VK_RMENU		}, // Right Alt key.
	{ Input::KeyCode::LeftAlt				,	VK_LMENU		}, // Left Alt key.
	{ Input::KeyCode::LeftCommand			,	VK_LWIN			}, // Left Command key.
	{ Input::KeyCode::LeftApple				,	VK_LWIN			}, // Left Command key.
	{ Input::KeyCode::LeftWindows			,	VK_LWIN			}, // Left Windows key.
	{ Input::KeyCode::RightCommand			,	VK_RWIN			}, // Right Command key.
	{ Input::KeyCode::RightApple			,	VK_RWIN			}, // Right Command key.
	{ Input::KeyCode::RightWindows			,	VK_RWIN			}, // Right Windows key.
	{ Input::KeyCode::Help					,	VK_HELP			}, // Help key.
	{ Input::KeyCode::Print					,	VK_PRINT		}, // Print key.
	{ Input::KeyCode::Break					,	VK_CANCEL		}, // Break key.
	{ Input::KeyCode::Menu					,	VK_MENU			}, // Menu key.
	{ Input::KeyCode::Mouse0				,	VK_LBUTTON		}, // The Left(or primary) mouse button.
	{ Input::KeyCode::Mouse1				,	VK_RBUTTON		}, // Right mouse button(or secondary mouse button).
	{ Input::KeyCode::Mouse2				,	VK_MBUTTON		}, // Middle mouse button(or third button).
	{ Input::KeyCode::Mouse3				,	VK_XBUTTON1		}, // Additional(fourth) mouse button.
	{ Input::KeyCode::Mouse4				,	VK_XBUTTON2		}, // Additional(fifth) mouse button.
	{ Input::KeyCode::Mouse5				,	0				}, // Additional(or sixth) mouse button.
	{ Input::KeyCode::Mouse6				,	0				}, // Additional(or seventh) mouse button.
	{ Input::KeyCode::JoystickButton0		,	0x0000			}, // Button 0 on any joystick.
	{ Input::KeyCode::JoystickButton1		,	0x0001			}, // Button 1 on any joystick.
	{ Input::KeyCode::JoystickButton2		,	0x0002			}, // Button 2 on any joystick.
	{ Input::KeyCode::JoystickButton3		,	0x0003			}, // Button 3 on any joystick.
	{ Input::KeyCode::JoystickButton4		,	0x0004			}, // Button 4 on any joystick.
	{ Input::KeyCode::JoystickButton5		,	0x0005			}, // Button 5 on any joystick.
	{ Input::KeyCode::JoystickButton6		,	0x0006			}, // Button 6 on any joystick.
	{ Input::KeyCode::JoystickButton7		,	0x0007			}, // Button 7 on any joystick.
	{ Input::KeyCode::JoystickButton8		,	0x0008			}, // Button 8 on any joystick.
	{ Input::KeyCode::JoystickButton9		,	0x0009			}, // Button 9 on any joystick.
	{ Input::KeyCode::JoystickButton10		,	0x000A			}, // Button 10 on any joystick.
	{ Input::KeyCode::JoystickButton11		,	0x000B			}, // Button 11 on any joystick.
	{ Input::KeyCode::JoystickButton12		,	0x000C			}, // Button 12 on any joystick.
	{ Input::KeyCode::JoystickButton13		,	0x000D			}, // Button 13 on any joystick.
	{ Input::KeyCode::JoystickButton14		,	0x000E			}, // Button 14 on any joystick.
	{ Input::KeyCode::JoystickButton15		,	0x000F			}, // Button 15 on any joystick.
	{ Input::KeyCode::JoystickButton16		,	0x0010			}, // Button 16 on any joystick.
	{ Input::KeyCode::JoystickButton17		,	0x0011			}, // Button 17 on any joystick.
	{ Input::KeyCode::JoystickButton18		,	0x0012			}, // Button 18 on any joystick.
	{ Input::KeyCode::JoystickButton19		,	0x0013			}, // Button 19 on any joystick.
	{ Input::KeyCode::Joystick1Button0		,	0x1000			},// Button 0 on first joystick.
	{ Input::KeyCode::Joystick1Button1		,	0x1001			},// Button 1 on first joystick.
	{ Input::KeyCode::Joystick1Button2		,	0x1002			},// Button 2 on first joystick.
	{ Input::KeyCode::Joystick1Button3		,	0x1003			},// Button 3 on first joystick.
	{ Input::KeyCode::Joystick1Button4		,	0x1004			},// Button 4 on first joystick.
	{ Input::KeyCode::Joystick1Button5		,	0x1005			},// Button 5 on first joystick.
	{ Input::KeyCode::Joystick1Button6		,	0x1006			},// Button 6 on first joystick.
	{ Input::KeyCode::Joystick1Button7		,	0x1000			},// Button 7 on first joystick.
	{ Input::KeyCode::Joystick1Button8		,	0x1001			},// Button 8 on first joystick.
	{ Input::KeyCode::Joystick1Button9		,	0x1002			},// Button 9 on first joystick.
	{ Input::KeyCode::Joystick1Button10		,	0x1003			},// Button 10 on first joystick.
	{ Input::KeyCode::Joystick1Button11		,	0x1004			},// Button 11 on first joystick.
	{ Input::KeyCode::Joystick1Button12		,	0x1005			},// Button 12 on first joystick.
	{ Input::KeyCode::Joystick1Button13		,	0x1006			},// Button 13 on first joystick.
	{ Input::KeyCode::Joystick1Button14		,	0x1007			},// Button 14 on first joystick.
	{ Input::KeyCode::Joystick1Button15		,	0x1008			},// Button 15 on first joystick.
	{ Input::KeyCode::Joystick1Button16		,	0x1009			},// Button 16 on first joystick.
	{ Input::KeyCode::Joystick1Button17		,	0x100A			},// Button 17 on first joystick.
	{ Input::KeyCode::Joystick1Button18		,	0x100B			},// Button 18 on first joystick.
	{ Input::KeyCode::Joystick1Button19		,	0x100C			},// Button 19 on first joystick.
	{ Input::KeyCode::Joystick2Button0		,	0x2000			},// Button 0 on second joystick.
	{ Input::KeyCode::Joystick2Button1		,	0x2001			},// Button 1 on second joystick.
	{ Input::KeyCode::Joystick2Button2		,	0x2002			},// Button 2 on second joystick.
	{ Input::KeyCode::Joystick2Button3		,	0x2003			},// Button 3 on second joystick.
	{ Input::KeyCode::Joystick2Button4		,	0x2004			},// Button 4 on second joystick.
	{ Input::KeyCode::Joystick2Button5		,	0x2005			},// Button 5 on second joystick.
	{ Input::KeyCode::Joystick2Button6		,	0x2006			},// Button 6 on second joystick.
	{ Input::KeyCode::Joystick2Button7		,	0x2007			},// Button 7 on second joystick.
	{ Input::KeyCode::Joystick2Button8		,	0x2008			},// Button 8 on second joystick.
	{ Input::KeyCode::Joystick2Button9		,	0x2009			},// Button 9 on second joystick.
	{ Input::KeyCode::Joystick2Button10		,	0x200A			},// Button 10 on second joystick.
	{ Input::KeyCode::Joystick2Button11		,	0x200B			},// Button 11 on second joystick.
	{ Input::KeyCode::Joystick2Button12		,	0x200C			},// Button 12 on second joystick.
	{ Input::KeyCode::Joystick2Button13		,	0x200D			},// Button 13 on second joystick.
	{ Input::KeyCode::Joystick2Button14		,	0x200E			},// Button 14 on second joystick.
	{ Input::KeyCode::Joystick2Button15		,	0x200F			},// Button 15 on second joystick.
	{ Input::KeyCode::Joystick2Button16		,	0x2010			},// Button 16 on second joystick.
	{ Input::KeyCode::Joystick2Button17		,	0x2010			},// Button 17 on second joystick.
	{ Input::KeyCode::Joystick2Button18		,	0x2010			},// Button 18 on second joystick.
	{ Input::KeyCode::Joystick2Button19		,	0x2010			},// Button 19 on second joystick.
	{ Input::KeyCode::Joystick3Button0		,	0x3000			},// Button 0 on third joystick.
	{ Input::KeyCode::Joystick3Button1		,	0x3001			},// Button 1 on third joystick.
	{ Input::KeyCode::Joystick3Button2		,	0x3002			},// Button 2 on third joystick.
	{ Input::KeyCode::Joystick3Button3		,	0x3003			},// Button 3 on third joystick.
	{ Input::KeyCode::Joystick3Button4		,	0x3004			},// Button 4 on third joystick.
	{ Input::KeyCode::Joystick3Button5		,	0x3005			},// Button 5 on third joystick.
	{ Input::KeyCode::Joystick3Button6		,	0x3006			},// Button 6 on third joystick.
	{ Input::KeyCode::Joystick3Button7		,	0x3007			},// Button 7 on third joystick.
	{ Input::KeyCode::Joystick3Button8		,	0x3008			},// Button 8 on third joystick.
	{ Input::KeyCode::Joystick3Button9		,	0x3009			},// Button 9 on third joystick.
	{ Input::KeyCode::Joystick3Button10		,	0x300A			},// Button 10 on third joystick.
	{ Input::KeyCode::Joystick3Button11		,	0x300B			},// Button 11 on third joystick.
	{ Input::KeyCode::Joystick3Button12		,	0x300C			},// Button 12 on third joystick.
	{ Input::KeyCode::Joystick3Button13		,	0x300D			},// Button 13 on third joystick.
	{ Input::KeyCode::Joystick3Button14		,	0x300E			},// Button 14 on third joystick.
	{ Input::KeyCode::Joystick3Button15		,	0x300F			},// Button 15 on third joystick.
	{ Input::KeyCode::Joystick3Button16		,	0x3010			},// Button 16 on third joystick.
	{ Input::KeyCode::Joystick3Button17		,	0x3011			},// Button 17 on third joystick.
	{ Input::KeyCode::Joystick3Button18		,	0x3012			},// Button 18 on third joystick.
	{ Input::KeyCode::Joystick3Button19		,	0x3013			},// Button 19 on third joystick.
	{ Input::KeyCode::Joystick4Button0		,	0x4000			},// Button 0 on forth joystick.
	{ Input::KeyCode::Joystick4Button1		,	0x4001			},// Button 1 on forth joystick.
	{ Input::KeyCode::Joystick4Button2		,	0x4002			},// Button 2 on forth joystick.
	{ Input::KeyCode::Joystick4Button3		,	0x4003			},// Button 3 on forth joystick.
	{ Input::KeyCode::Joystick4Button4		,	0x4004			},// Button 4 on forth joystick.
	{ Input::KeyCode::Joystick4Button5		,	0x4005			},// Button 5 on forth joystick.
	{ Input::KeyCode::Joystick4Button6		,	0x4006			},// Button 6 on forth joystick.
	{ Input::KeyCode::Joystick4Button7		,	0x4007			},// Button 7 on forth joystick.
	{ Input::KeyCode::Joystick4Button8		,	0x4008			},// Button 8 on forth joystick.
	{ Input::KeyCode::Joystick4Button9		,	0x4009			},// Button 9 on forth joystick.
	{ Input::KeyCode::Joystick4Button10		,	0x400A			},// Button 10 on forth joystick.
	{ Input::KeyCode::Joystick4Button11		,	0x400B			},// Button 11 on forth joystick.
	{ Input::KeyCode::Joystick4Button12		,	0x400C			},// Button 12 on forth joystick.
	{ Input::KeyCode::Joystick4Button13		,	0x400D			},// Button 13 on forth joystick.
	{ Input::KeyCode::Joystick4Button14		,	0x400E			},// Button 14 on forth joystick.
	{ Input::KeyCode::Joystick4Button15		,	0x400F			},// Button 15 on forth joystick.
	{ Input::KeyCode::Joystick4Button16		,	0x4010			},// Button 16 on forth joystick.
	{ Input::KeyCode::Joystick4Button17		,	0x4011			},// Button 17 on forth joystick.
	{ Input::KeyCode::Joystick4Button18		,	0x4012			},// Button 18 on forth joystick.
	{ Input::KeyCode::Joystick4Button19		,	0x4013			},// Button 19 on forth joystick.
	{ Input::KeyCode::Joystick5Button0		,	0x5000			},// Button 0 on fifth joystick.
	{ Input::KeyCode::Joystick5Button1		,	0x5001			},// Button 1 on fifth joystick.
	{ Input::KeyCode::Joystick5Button2		,	0x5002			},// Button 2 on fifth joystick.
	{ Input::KeyCode::Joystick5Button3		,	0x5003			},// Button 3 on fifth joystick.
	{ Input::KeyCode::Joystick5Button4		,	0x5004			},// Button 4 on fifth joystick.
	{ Input::KeyCode::Joystick5Button5		,	0x5005			},// Button 5 on fifth joystick.
	{ Input::KeyCode::Joystick5Button6		,	0x5006			},// Button 6 on fifth joystick.
	{ Input::KeyCode::Joystick5Button7		,	0x5007			},// Button 7 on fifth joystick.
	{ Input::KeyCode::Joystick5Button8		,	0x5008			},// Button 8 on fifth joystick.
	{ Input::KeyCode::Joystick5Button9		,	0x5009			},// Button 9 on fifth joystick.
	{ Input::KeyCode::Joystick5Button10		,	0x500A			},// Button 10 on fifth joystick.
	{ Input::KeyCode::Joystick5Button11		,	0x500B			},// Button 11 on fifth joystick.
	{ Input::KeyCode::Joystick5Button12		,	0x500C			},// Button 12 on fifth joystick.
	{ Input::KeyCode::Joystick5Button13		,	0x500D			},// Button 13 on fifth joystick.
	{ Input::KeyCode::Joystick5Button14		,	0x500E			},// Button 14 on fifth joystick.
	{ Input::KeyCode::Joystick5Button15		,	0x500F			},// Button 15 on fifth joystick.
	{ Input::KeyCode::Joystick5Button16		,	0x5010			},// Button 16 on fifth joystick.
	{ Input::KeyCode::Joystick5Button17		,	0x5011			},// Button 17 on fifth joystick.
	{ Input::KeyCode::Joystick5Button18		,	0x5012			},// Button 18 on fifth joystick.
	{ Input::KeyCode::Joystick5Button19		,	0x5013			},// Button 19 on fifth joystick.
	{ Input::KeyCode::Joystick6Button0		,	0x6000			},// Button 0 on sixth joystick.
	{ Input::KeyCode::Joystick6Button1		,	0x6001			},// Button 1 on sixth joystick.
	{ Input::KeyCode::Joystick6Button2		,	0x6002			},// Button 2 on sixth joystick.
	{ Input::KeyCode::Joystick6Button3		,	0x6003			},// Button 3 on sixth joystick.
	{ Input::KeyCode::Joystick6Button4		,	0x6004			},// Button 4 on sixth joystick.
	{ Input::KeyCode::Joystick6Button5		,	0x6005			},// Button 5 on sixth joystick.
	{ Input::KeyCode::Joystick6Button6		,	0x6006			},// Button 6 on sixth joystick.
	{ Input::KeyCode::Joystick6Button7		,	0x6007			},// Button 7 on sixth joystick.
	{ Input::KeyCode::Joystick6Button8		,	0x6008			},// Button 8 on sixth joystick.
	{ Input::KeyCode::Joystick6Button9		,	0x6009			},// Button 9 on sixth joystick.
	{ Input::KeyCode::Joystick6Button10		,	0x600A			},// Button 10 on sixth joystick.
	{ Input::KeyCode::Joystick6Button11		,	0x600B			},// Button 11 on sixth joystick.
	{ Input::KeyCode::Joystick6Button12		,	0x600C			},// Button 12 on sixth joystick.
	{ Input::KeyCode::Joystick6Button13		,	0x600D			},// Button 13 on sixth joystick.
	{ Input::KeyCode::Joystick6Button14		,	0x600E			},// Button 14 on sixth joystick.
	{ Input::KeyCode::Joystick6Button15		,	0x600F			},// Button 15 on sixth joystick.
	{ Input::KeyCode::Joystick6Button16		,	0x6010			},// Button 16 on sixth joystick.
	{ Input::KeyCode::Joystick6Button17		,	0x6011			},// Button 17 on sixth joystick.
	{ Input::KeyCode::Joystick6Button18		,	0x6012			},// Button 18 on sixth joystick.
	{ Input::KeyCode::Joystick6Button19		,	0x6013			},// Button 19 on sixth joystick.
	{ Input::KeyCode::Joystick7Button0		,	0x7000			},// Button 0 on seventh joystick.
	{ Input::KeyCode::Joystick7Button1		,	0x7001			},// Button 1 on seventh joystick.
	{ Input::KeyCode::Joystick7Button2		,	0x7002			},// Button 2 on seventh joystick.
	{ Input::KeyCode::Joystick7Button3		,	0x7003			},// Button 3 on seventh joystick.
	{ Input::KeyCode::Joystick7Button4		,	0x7004			},// Button 4 on seventh joystick.
	{ Input::KeyCode::Joystick7Button5		,	0x7005			},// Button 5 on seventh joystick.
	{ Input::KeyCode::Joystick7Button6		,	0x7006			},// Button 6 on seventh joystick.
	{ Input::KeyCode::Joystick7Button7		,	0x7007			},// Button 7 on seventh joystick.
	{ Input::KeyCode::Joystick7Button8		,	0x7008			},// Button 8 on seventh joystick.
	{ Input::KeyCode::Joystick7Button9		,	0x7009			},// Button 9 on seventh joystick.
	{ Input::KeyCode::Joystick7Button10		,	0x700A			},// Button 10 on seventh joystick.
	{ Input::KeyCode::Joystick7Button11		,	0x700B			},// Button 11 on seventh joystick.
	{ Input::KeyCode::Joystick7Button12		,	0x700C			},// Button 12 on seventh joystick.
	{ Input::KeyCode::Joystick7Button13		,	0x700D			},// Button 13 on seventh joystick.
	{ Input::KeyCode::Joystick7Button14		,	0x700E			},// Button 14 on seventh joystick.
	{ Input::KeyCode::Joystick7Button15		,	0x700F			},// Button 15 on seventh joystick.
	{ Input::KeyCode::Joystick7Button16		,	0x7010			},// Button 16 on seventh joystick.
	{ Input::KeyCode::Joystick7Button17		,	0x7011			},// Button 17 on seventh joystick.
	{ Input::KeyCode::Joystick7Button18		,	0x7012			},// Button 18 on seventh joystick.
	{ Input::KeyCode::Joystick7Button19		,	0x7013			},// Button 19 on seventh joystick.
	{ Input::KeyCode::Joystick8Button0		,	0x8000			},// Button 0 on eighth joystick.
	{ Input::KeyCode::Joystick8Button1		,	0x8001			},// Button 1 on eighth joystick.
	{ Input::KeyCode::Joystick8Button2		,	0x8002			},// Button 2 on eighth joystick.
	{ Input::KeyCode::Joystick8Button3		,	0x8003			},// Button 3 on eighth joystick.
	{ Input::KeyCode::Joystick8Button4		,	0x8004			},// Button 4 on eighth joystick.
	{ Input::KeyCode::Joystick8Button5		,	0x8005			},// Button 5 on eighth joystick.
	{ Input::KeyCode::Joystick8Button6		,	0x8006			},// Button 6 on eighth joystick.
	{ Input::KeyCode::Joystick8Button7		,	0x8007			},// Button 7 on eighth joystick.
	{ Input::KeyCode::Joystick8Button8		,	0x8008			},// Button 8 on eighth joystick.
	{ Input::KeyCode::Joystick8Button9		,	0x8009			},// Button 9 on eighth joystick.
	{ Input::KeyCode::Joystick8Button10		,	0x800A			},// Button 10 on eighth joystick.
	{ Input::KeyCode::Joystick8Button11		,	0x800B			},// Button 11 on eighth joystick.
	{ Input::KeyCode::Joystick8Button12		,	0x800C			},// Button 12 on eighth joystick.
	{ Input::KeyCode::Joystick8Button13		,	0x800D			},// Button 13 on eighth joystick.
	{ Input::KeyCode::Joystick8Button14		,	0x800E			},// Button 14 on eighth joystick.
	{ Input::KeyCode::Joystick8Button15		,	0x800F			},// Button 15 on eighth joystick.
	{ Input::KeyCode::Joystick8Button16		,	0x8010			},// Button 16 on eighth joystick.
	{ Input::KeyCode::Joystick8Button17		,	0x8011			},// Button 17 on eighth joystick.
	{ Input::KeyCode::Joystick8Button18		,	0x8012			},// Button 18 on eighth joystick.
	{ Input::KeyCode::Joystick8Button19		,	0x8013			},// Button 19 on eighth joystick.
};

Input::Input()
{
}

Input::Input(
	const std::string& name_,
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
	JoyStick joystick_)
: name(name_)
, descriptivePositiveName(descriptivePositiveName_)
, descriptiveNegativeName(descriptiveNegativeName_)
, positiveButton(positiveButton_)
, negativeButton(negativeButton_)
, positiveAltButton(positiveAltButton_)
, negativeAltButton(negativeAltButton_)
, gravity(gravity_)
, deadzone(deadzone_)
, sensitivity(sensitivity_)
, snap(snap_)
, invert(invert_)
, type(type_)
, axis(axis_)
, joystick(joystick_)
, lastValue(0)
, currentValue(0)
, rawValue(0)
{
}

Input::~Input()
{
}

float Input::GetAxis()
{
	return currentValue;
}

float Input::GetAxisRaw()
{
	return rawValue;
}

void Input::UpdateKeyOrMouseButton()
{
	int posVK1 = keyCode2VKs[positiveButton];
	int negVK1 = keyCode2VKs[negativeButton];
	int posVK2 = keyCode2VKs[positiveAltButton];
	int negVK2 = keyCode2VKs[negativeAltButton];

	lastValue = currentValue;

	if (Input::Manager::GetInstance().currentValues[posVK1]>deadzone || Input::Manager::GetInstance().currentValues[posVK2] > deadzone)
	{
		if (!invert)
			rawValue = 1.0;
		else
			rawValue = -1.0;

		currentValue += rawValue * sensitivity * Platform::GetElapsed();
	}
	else if (Input::Manager::GetInstance().currentValues[negVK1] > deadzone || Input::Manager::GetInstance().currentValues[negVK2] > deadzone)
	{
		if (!invert)
			rawValue = -1.0;
		else
			rawValue = 1.0;

		currentValue += rawValue * sensitivity * Platform::GetElapsed();
	}

	if (gravity != 0)
	{
		currentValue -= Math::Sgn(currentValue) * gravity * Platform::GetElapsed();
	}

	if (currentValue > 1.0)
		currentValue = 1.0;

	if (currentValue < -1.0)
		currentValue = -1.0;
}

void Input::UpdateMouseMovement()
{
}

void Input::UpdateJoystickAxis()
{
}

void Input::Update()
{
	if (type == Type::KeyOrMouseButton)
	{
		UpdateKeyOrMouseButton();
	}
	else if (type == Type::MouseMovement)
	{
		UpdateMouseMovement();
	}
	else// if (type == Type::JoystickAxis)
	{
		UpdateJoystickAxis();
	}
}

/////////////////////////////////////////////////////////////////////
Input::Manager::Manager()
: lastValues(256)
, currentValues(256)
, mouseMovement(0, 0)
{
}

Input::Manager::~Manager()
{
}

Input::Manager& Input::Manager::GetInstance()
{
	static Input::Manager instance;

	return instance;
}

Input& Input::Manager::GetInput(const char* axisName)
{
	return inputs[axisName];
}

float Input::Manager::GetAxis(const char* axisName)
{
	return GetInput(axisName).GetAxis();
}

float Input::Manager::GetAxisRaw(const char* axisName)
{
	return GetInput(axisName).GetAxisRaw();
}

bool Input::Manager::GetButton(const char* axisName)
{
	const Input& input = GetInput(axisName);

	return GetKey(input.positiveButton);// || GetKey(input.positiveAltButton);
}

bool Input::Manager::GetButtonDown(const char* axisName)
{
	const Input& input = GetInput(axisName);

	return GetKeyDown(input.positiveButton);// || GetKeyDown(input.positiveAltButton);
}

bool Input::Manager::GetButtonUp(const char* axisName)
{
	const Input& input = GetInput(axisName);

	return GetKeyUp(input.positiveButton);// || GetKeyUp(input.positiveAltButton);
}

bool Input::Manager::GetKey(Input::KeyCode keyCode)
{
	int vk = keyCode2VKs[keyCode];

	return (lastValues[vk]==1) && (currentValues[vk] == 1);
}

bool Input::Manager::GetKeyDown(Input::KeyCode keyCode)
{
	int vk = keyCode2VKs[keyCode];

	return (lastValues[vk] == -1) && (currentValues[vk] == 1);
}

bool Input::Manager::GetKeyUp(Input::KeyCode keyCode)
{
	int vk = keyCode2VKs[keyCode];

	return (lastValues[vk] == 1) && (currentValues[vk] == -1);
}

int Input::Manager::GetKeyValue(Input::KeyCode keyCode)
{
	int vk = keyCode2VKs[keyCode];

	return currentValues[vk];
}

//const std::vector<std::string>& GetJoystickNames();
bool Input::Manager::GetMouseButton(int button)
{
	if(button==0)
		return GetKey(Input::KeyCode::Mouse0);
	else if (button == 1)
		return GetKey(Input::KeyCode::Mouse1);
	else// if (button == 2)
		return GetKey(Input::KeyCode::Mouse2);
}

bool Input::Manager::GetMouseButtonDown(int button)
{
	if (button == 0)
		return GetKeyDown(Input::KeyCode::Mouse0);
	else if (button == 1)
		return GetKeyDown(Input::KeyCode::Mouse1);
	else// if (button == 2)
		return GetKeyDown(Input::KeyCode::Mouse2);
}

bool Input::Manager::GetMouseButtonUp(int button)
{
	if (button == 0)
		return GetKeyUp(Input::KeyCode::Mouse0);
	else if (button == 1)
		return GetKeyUp(Input::KeyCode::Mouse1);
	else// if (button == 2)
		return GetKeyUp(Input::KeyCode::Mouse2);
}

Vector2 Input::Manager::GetMouseMovement()
{
	return mouseMovement;
}

// Retrieves a list of input device names corresponding to the index of an Axis configured within Input Manager.
//const std::vector<std::string>& Input::Manager::GetJoystickNames()
// Touch GetTouch(int index);									// Call Input.GetTouch to obtain a Touch struct.

const Input::AccelerationEvent& Input::Manager::GetAccelerationEvent(int i) const
{
	// Returns specific acceleration measurement which occurred during last frame. (Does not allocate temporary variables).
	return accelerationEvents[i];
}

bool Input::Manager::Initialize()
{
	return true;
}

bool Input::Manager::Process()
{
	for (int i = VK_LBUTTON; i < VK_OEM_CLEAR; i++)
	{
		lastValues[i] = currentValues[i];
		currentValues[i] = ((GetAsyncKeyState(i) & 0x8000)!=0) ? 1 : -1;
	}

	for (auto& input : inputs)
	{
		input.second.Update();
	}

	return true;
}

bool Input::Manager::Pause()
{
	return true;
}

void Input::Manager::Resume()
{
}

void Input::Manager::Terminate()
{
}

/////////////////////////////////////////////////////////////////////
bool Input::Service::Initialize()
{
	return Input::Manager::GetInstance().Initialize();
}

bool Input::Service::Process()
{
	return Input::Manager::GetInstance().Process();
}

bool Input::Service::Pause()
{
	return Input::Manager::GetInstance().Pause();
}

void Input::Service::Resume()
{
	Input::Manager::GetInstance().Resume();
}

void Input::Service::Terminate()
{
	Input::Manager::GetInstance().Terminate();
}