#pragma once

#include <stdint.h>
namespace Coffee
{

    /**
     * @defgroup core Core
     * @brief Core components of the CoffeeEngine.
     * @{
     */

	using KeyCode = uint16_t;
	
	namespace Key
	{
	    /**
         * @enum Key
         * @brief Enumeration of key codes.
         *
         * These values are from usage page 0x07 (USB keyboard page).
         */
		enum : KeyCode
		{
			// From SDL_scancode.h
			Unknown = 0,

			/**
			*  \name Usage page 0x07
			*
			*  These values are from usage page 0x07 (USB keyboard page).
			*/
			/* @{ */

			A = 4,
			B = 5,
			C = 6,
			D = 7,
			E = 8,
			F = 9,
			G = 10,
			H = 11,
			I = 12,
			J = 13,
			K = 14,
			L = 15,
			M = 16,
			N = 17,
			O = 18,
			P = 19,
			Q = 20,
			R = 21,
			S = 22,
			T = 23,
			U = 24,
			V = 25,
			W = 26,
			X = 27,
			Y = 28,
			Z = 29,

			D1 = 30,
			D2 = 31,
			D3 = 32,
			D4 = 33,
			D5 = 34,
			D6 = 35,
			D7 = 36,
			D8 = 37,
			D9 = 38,
			D0 = 39,

			Return = 40,
			Escape = 41,
			Backspace = 42,
			Tab = 43,
			Space = 44,

			Minus = 45,
			Equals = 46,
			LeftBracket = 47,
			RightBracket = 48,
			Backslash = 49, /**< Located at the lower left of the return
										*   key on ISO keyboards and at the right end
										*   of the QWERTY row on ANSI keyboards.
										*   Produces REVERSE SOLIDUS (backslash) and
										*   VERTICAL LINE in a US layout, REVERSE
										*   SOLIDUS and VERTICAL LINE in a UK Mac
										*   layout, NUMBER SIGN and TILDE in a UK
										*   Windows layout, DOLLAR SIGN and POUND SIGN
										*   in a Swiss German layout, NUMBER SIGN and
										*   APOSTROPHE in a German layout, GRAVE
										*   ACCENT and POUND SIGN in a French Mac
										*   layout, and ASTERISK and MICRO SIGN in a
										*   French Windows layout.
										*/
			NonUsHash = 50, /**< ISO USB keyboards actually use this code
										*   instead of 49 for the same key, but all
										*   OSes I've seen treat the two codes
										*   identically. So, as an implementor, unless
										*   your keyboard generates both of those
										*   codes and your OS treats them differently,
										*   you should generate BACKSLASH
										*   instead of this code. As a user, you
										*   should not rely on this code because SDL
										*   will never generate it with most (all?)
										*   keyboards.
										*/
			Semicolon = 51,
			Apostrophe = 52,
			Grave = 53, /**< Located in the top left corner (on both ANSI
									*   and ISO keyboards). Produces GRAVE ACCENT and
									*   TILDE in a US Windows layout and in US and UK
									*   Mac layouts on ANSI keyboards, GRAVE ACCENT
									*   and NOT SIGN in a UK Windows layout, SECTION
									*   SIGN and PLUS-MINUS SIGN in US and UK Mac
									*   layouts on ISO keyboards, SECTION SIGN and
									*   DEGREE SIGN in a Swiss German layout (Mac:
									*   only on ISO keyboards), CIRCUMFLEX ACCENT and
									*   DEGREE SIGN in a German layout (Mac: only on
									*   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
									*   French Windows layout, COMMERCIAL AT and
									*   NUMBER SIGN in a French Mac layout on ISO
									*   keyboards, and LESS-THAN SIGN and GREATER-THAN
									*   SIGN in a Swiss German, German, or French Mac
									*   layout on ANSI keyboards.
									*/
			Comma = 54,
			Period = 55,
			Slash = 56,

			CapsLock = 57,

			F1 = 58,
			F2 = 59,
			F3 = 60,
			F4 = 61,
			F5 = 62,
			F6 = 63,
			F7 = 64,
			F8 = 65,
			F9 = 66,
			F10 = 67,
			F11 = 68,
			F12 = 69,

			PrintScreen = 70,
			ScrollLock = 71,
			Pause = 72,
			Insert = 73, /**< insert on PC, help on some Mac keyboards (but
										does send code 73, not 117) */
			Home = 74,
			PageUp = 75,
			Delete = 76,
			End = 77,
			PageDown = 78,
			Right = 79,
			Left = 80,
			Down = 81,
			Up = 82,

			NumLockClear = 83, /**< num lock on PC, clear on Mac keyboards
											*/
			KpDivide = 84,
			KpMultiply = 85,
			KpMinus = 86,
			KpPlus = 87,
			KpEnter = 88,
			Kp1 = 89,
			Kp2 = 90,
			Kp3 = 91,
			Kp4 = 92,
			Kp5 = 93,
			Kp6 = 94,
			Kp7 = 95,
			Kp8 = 96,
			Kp9 = 97,
			Kp0 = 98,
			KpPeriod = 99,

			NonUsBackslash = 100, /**< This is the additional key that ISO
												*   keyboards have over ANSI ones,
												*   located between left shift and Y.
												*   Produces GRAVE ACCENT and TILDE in a
												*   US or UK Mac layout, REVERSE SOLIDUS
												*   (backslash) and VERTICAL LINE in a
												*   US or UK Windows layout, and
												*   LESS-THAN SIGN and GREATER-THAN SIGN
												*   in a Swiss German, German, or French
												*   layout. */
			Application = 101, /**< windows contextual menu, compose */
			Power = 102, /**< The USB document says this is a status flag,
									*   not a physical key - but some Mac keyboards
									*   do have a power key. */
			KpEquals = 103,
			F13 = 104,
			F14 = 105,
			F15 = 106,
			F16 = 107,
			F17 = 108,
			F18 = 109,
			F19 = 110,
			F20 = 111,
			F21 = 112,
			F22 = 113,
			F23 = 114,
			F24 = 115,
			Execute = 116,
			Help = 117,    /**< AL Integrated Help Center */
			Menu = 118,    /**< Menu (show menu) */
			Select = 119,
			Stop = 120,    /**< AC Stop */
			Again = 121,   /**< AC Redo/Repeat */
			Undo = 122,    /**< AC Undo */
			Cut = 123,     /**< AC Cut */
			Copy = 124,    /**< AC Copy */
			Paste = 125,   /**< AC Paste */
			Find = 126,    /**< AC Find */
			Mute = 127,
			VolumeUp = 128,
			VolumeDown = 129,
		/* not sure whether there's a reason to enable these */
		/*     LOCKINGCAPSLOCK = 130,  */
		/*     LOCKINGNUMLOCK = 131, */
		/*     LOCKINGSCROLLLOCK = 132, */
			KpComma = 133,
			KpEqualsAs400 = 134,

			International1 = 135, /**< used on Asian keyboards, see
													footnotes in USB doc */
			International2 = 136,
			International3 = 137, /**< Yen */
			International4 = 138,
			International5 = 139,
			International6 = 140,
			International7 = 141,
			International8 = 142,
			International9 = 143,
			Lang1 = 144, /**< Hangul/English toggle */
			Lang2 = 145, /**< Hanja conversion */
			Lang3 = 146, /**< Katakana */
			Lang4 = 147, /**< Hiragana */
			Lang5 = 148, /**< Zenkaku/Hankaku */
			Lang6 = 149, /**< reserved */
			Lang7 = 150, /**< reserved */
			Lang8 = 151, /**< reserved */
			Lang9 = 152, /**< reserved */

			AltErase = 153,    /**< Erase-Eaze */
			SysReq = 154,
			Cancel = 155,      /**< AC Cancel */
			Clear = 156,
			Prior = 157,
			Return2 = 158,
			Separator = 159,
			Out = 160,
			Oper = 161,
			ClearAgain = 162,
			CrSel = 163,
			ExSel = 164,

			Kp00 = 176,
			Kp000 = 177,
			ThousandsSeparator = 178,
			DecimalSeparator = 179,
			CurrencyUnit = 180,
			CurrencySubUnit = 181,
			KpLeftParen = 182,
			KpRightParen = 183,
			KpLeftBrace = 184,
			KpRightBrace = 185,
			KpTab = 186,
			KpBackspace = 187,
			KpA = 188,
			KpB = 189,
			KpC = 190,
			KpD = 191,
			KpE = 192,
			KpF = 193,
			KpXor = 194,
			KpPower = 195,
			KpPercent = 196,
			KpLess = 197,
			KpGreater = 198,
			KpAmpersand = 199,
			KpDblAmpersand = 200,
			KpVerticalBar = 201,
			KpDblVerticalBar = 202,
			KpColon = 203,
			KpHash = 204,
			KpSpace = 205,
			KpAt = 206,
			KpExclam = 207,
			KpMemStore = 208,
			KpMemRecall = 209,
			KpMemClear = 210,
			KpMemAdd = 211,
			KpMemSubtract = 212,
			KpMemMultiply = 213,
			KpMemDivide = 214,
			KpPlusMinus = 215,
			KpClear = 216,
			KpClearEntry = 217,
			KpBinary = 218,
			KpOctal = 219,
			KpDecimal = 220,
			KpHexadecimal = 221,

			LCtrl = 224,
			LShift = 225,
			LAlt = 226, /**< alt, option */
			LGui = 227, /**< windows, command (apple), meta */
			RCtrl = 228,
			RShift = 229,
			RAlt = 230, /**< alt gr, option */
			RGui = 231, /**< windows, command (apple), meta */

			Mode = 257,    /**< I'm not sure if this is really not covered
										*   by any of the above, but since there's a
										*   special SDL_KMOD_MODE for it I'm adding it here
										*/

			/* @} *//* Usage page 0x07 */

			/**
			*  \name Usage page 0x0C
			*
			*  These values are mapped from usage page 0x0C (USB consumer page).
			*
			*  There are way more keys in the spec than we can represent in the
			*  current scancode range, so pick the ones that commonly come up in
			*  real world usage.
			*/
			/* @{ */

			Sleep = 258,                   /**< Sleep */
			Wake = 259,                    /**< Wake */

			ChannelIncrement = 260,       /**< Channel Increment */
			ChannelDecrement = 261,       /**< Channel Decrement */

			MediaPlay = 262,          /**< Play */
			MediaPause = 263,         /**< Pause */
			MediaRecord = 264,        /**< Record */
			MediaFastForward = 265,  /**< Fast Forward */
			MediaRewind = 266,        /**< Rewind */
			MediaNextTrack = 267,    /**< Next Track */
			MediaPreviousTrack = 268, /**< Previous Track */
			MediaStop = 269,          /**< Stop */
			MediaEject = 270,         /**< Eject */
			MediaPlayPause = 271,    /**< Play / Pause */
			MediaSelect = 272,        /* Media Select */

			AcNew = 273,              /**< AC New */
			AcOpen = 274,             /**< AC Open */
			AcClose = 275,            /**< AC Close */
			AcExit = 276,             /**< AC Exit */
			AcSave = 277,             /**< AC Save */
			AcPrint = 278,            /**< AC Print */
			AcProperties = 279,       /**< AC Properties */

			AcSearch = 280,           /**< AC Search */
			AcHome = 281,             /**< AC Home */
			AcBack = 282,             /**< AC Back */
			AcForward = 283,          /**< AC Forward */
			AcStop = 284,             /**< AC Stop */
			AcRefresh = 285,          /**< AC Refresh */
			AcBookmarks = 286,        /**< AC Bookmarks */

			/* @} *//* Usage page 0x0C */


			/**
			*  \name Mobile keys
			*
			*  These are values that are often used on mobile phones.
			*/
			/* @{ */

			SoftLeft = 287, /**< Usually situated below the display on phones and
											used as a multi-function feature key for selecting
											a software defined function shown on the bottom left
											of the display. */
			SoftRight = 288, /**< Usually situated below the display on phones and
											used as a multi-function feature key for selecting
											a software defined function shown on the bottom right
											of the display. */
			Call = 289, /**< Used for accepting phone calls. */
			EndCall = 290, /**< Used for rejecting phone calls. */

			/* @} *//* Mobile keys */

			/* Add any other keys here. */

			Reserved = 400,    /**< 400-500 reserved for dynamic keycodes */

			Count = 512 /**< not a key, just marks the number of scancodes for array bounds */
		};
	}
    /** @} */
}
