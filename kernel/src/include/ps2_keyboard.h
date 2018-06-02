
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define KEY_ESCAPE          0x01
#define KEY_1               0x02
#define KEY_2               0x03
#define KEY_3               0x04
#define KEY_4               0x05
#define KEY_5               0x06
#define KEY_6               0x07
#define KEY_7               0x08
#define KEY_8               0x09
#define KEY_9               0x0A
#define KEY_0               0x0B
#define KEY_MINUS           0x0C    /* - on main keyboard */
#define KEY_EQUALS          0x0D
#define KEY_BACK            0x0E    /* backspace */
#define KEY_TAB             0x0F
#define KEY_Q               0x10
#define KEY_W               0x11
#define KEY_E               0x12
#define KEY_R               0x13
#define KEY_T               0x14
#define KEY_Y               0x15
#define KEY_U               0x16
#define KEY_I               0x17
#define KEY_O               0x18
#define KEY_P               0x19
#define KEY_LBRACKET        0x1A
#define KEY_RBRACKET        0x1B
#define KEY_RETURN          0x1C    /* Enter on main keyboard */
#define KEY_LCONTROL        0x1D
#define KEY_A               0x1E
#define KEY_S               0x1F
#define KEY_D               0x20
#define KEY_F               0x21
#define KEY_G               0x22
#define KEY_H               0x23
#define KEY_J               0x24
#define KEY_K               0x25
#define KEY_L               0x26
#define KEY_SEMICOLON       0x27
#define KEY_APOSTROPHE      0x28
#define KEY_GRAVE           0x29    /* accent grave */
#define KEY_LSHIFT          0x2A
#define KEY_BACKSLASH       0x2B
#define KEY_Z               0x2C
#define KEY_X               0x2D
#define KEY_C               0x2E
#define KEY_V               0x2F
#define KEY_B               0x30
#define KEY_N               0x31
#define KEY_M               0x32
#define KEY_COMMA           0x33
#define KEY_PERIOD          0x34    /* . on main keyboard */
#define KEY_SLASH           0x35    /* / on main keyboard */
#define KEY_RSHIFT          0x36
#define KEY_MULTIPLY        0x37    /* * on numeric keypad */
#define KEY_LMENU           0x38    /* left Alt */
#define KEY_SPACE           0x39
#define KEY_CAPITAL         0x3A
#define KEY_F1              0x3B
#define KEY_F2              0x3C
#define KEY_F3              0x3D
#define KEY_F4              0x3E
#define KEY_F5              0x3F
#define KEY_F6              0x40
#define KEY_F7              0x41
#define KEY_F8              0x42
#define KEY_F9              0x43
#define KEY_F10             0x44
#define KEY_NUMLOCK         0x45
#define KEY_SCROLL          0x46    /* Scroll Lock */
#define KEY_NUMPAD7         0x47
#define KEY_NUMPAD8         0x48
#define KEY_NUMPAD9         0x49
#define KEY_SUBTRACT        0x4A    /* - on numeric keypad */
#define KEY_NUMPAD4         0x4B
#define KEY_NUMPAD5         0x4C
#define KEY_NUMPAD6         0x4D
#define KEY_ADD             0x4E    /* + on numeric keypad */
#define KEY_NUMPAD1         0x4F
#define KEY_NUMPAD2         0x50
#define KEY_NUMPAD3         0x51
#define KEY_NUMPAD0         0x52
#define KEY_DECIMAL         0x53    /* . on numeric keypad */
#define KEY_OEM_102         0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
#define KEY_F11             0x57
#define KEY_F12             0x58
#define KEY_F13             0x64    /*                     (NEC PC98) */
#define KEY_F14             0x65    /*                     (NEC PC98) */
#define KEY_F15             0x66    /*                     (NEC PC98) */
#define KEY_KANA            0x70    /* (Japanese keyboard)            */
#define KEY_ABNT_C1         0x73    /* /? on Brazilian keyboard */
#define KEY_CONVERT         0x79    /* (Japanese keyboard)            */
#define KEY_NOCONVERT       0x7B    /* (Japanese keyboard)            */
#define KEY_YEN             0x7D    /* (Japanese keyboard)            */
#define KEY_ABNT_C2         0x7E    /* Numpad . on Brazilian keyboard */
#define KEY_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */
#define KEY_PREVTRACK       0x90    /* Previous Track (DIK_CIRCUMFLEX on Japanese keyboard) */
#define KEY_AT              0x91    /*                     (NEC PC98) */
#define KEY_COLON           0x92    /*                     (NEC PC98) */
#define KEY_UNDERLINE       0x93    /*                     (NEC PC98) */
#define KEY_KANJI           0x94    /* (Japanese keyboard)            */
#define KEY_STOP            0x95    /*                     (NEC PC98) */
#define KEY_AX              0x96    /*                     (Japan AX) */
#define KEY_UNLABELED       0x97    /*                        (J3100) */
#define KEY_NEXTTRACK       0x99    /* Next Track */
#define KEY_NUMPADENTER     0x9C    /* Enter on numeric keypad */
#define KEY_RCONTROL        0x9D
#define KEY_MUTE            0xA0    /* Mute */
#define KEY_CALCULATOR      0xA1    /* Calculator */
#define KEY_PLAYPAUSE       0xA2    /* Play / Pause */
#define KEY_MEDIASTOP       0xA4    /* Media Stop */
#define KEY_VOLUMEDOWN      0xAE    /* Volume - */
#define KEY_VOLUMEUP        0xB0    /* Volume + */
#define KEY_WEBHOME         0xB2    /* Web home */
#define KEY_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) */
#define KEY_DIVIDE          0xB5    /* / on numeric keypad */
#define KEY_SYSRQ           0xB7
#define KEY_RMENU           0xB8    /* right Alt */
#define KEY_PAUSE           0xC5    /* Pause */
#define KEY_HOME            0xC7    /* Home on arrow keypad */
#define KEY_UP              0xC8    /* UpArrow on arrow keypad */
#define KEY_PRIOR           0xC9    /* PgUp on arrow keypad */
#define KEY_LEFT            0xCB    /* LeftArrow on arrow keypad */
#define KEY_RIGHT           0xCD    /* RightArrow on arrow keypad */
#define KEY_END             0xCF    /* End on arrow keypad */
#define KEY_DOWN            0xD0    /* DownArrow on arrow keypad */
#define KEY_NEXT            0xD1    /* PgDn on arrow keypad */
#define KEY_INSERT          0xD2    /* Insert on arrow keypad */
#define KEY_DELETE          0xD3    /* Delete on arrow keypad */
#define KEY_LWIN            0xDB    /* Left Windows key */
#define KEY_RWIN            0xDC    /* Right Windows key */
#define KEY_APPS            0xDD    /* AppMenu key */
#define KEY_POWER           0xDE    /* System Power */
#define KEY_SLEEP           0xDF    /* System Sleep */
#define KEY_WAKE            0xE3    /* System Wake */
#define KEY_WEBSEARCH       0xE5    /* Web Search */
#define KEY_WEBFAVORITES    0xE6    /* Web Favorites */
#define KEY_WEBREFRESH      0xE7    /* Web Refresh */
#define KEY_WEBSTOP         0xE8    /* Web Stop */
#define KEY_WEBFORWARD      0xE9    /* Web Forward */
#define KEY_WEBBACK         0xEA    /* Web Back */
#define KEY_MYCOMPUTER      0xEB    /* My Computer */
#define KEY_MAIL            0xEC    /* Mail */
#define KEY_MEDIASELECT     0xED    /* Media Select */

// enum tag_keyboard_scancode1
// {
//     KEYBOARD_NULL,
//     KEYBOARD_ESCAPE,
//     KEYBOARD_NUM1,
//     KEYBOARD_NUM2,
//     KEYBOARD_NUM3,
//     KEYBOARD_NUM4,
//     KEYBOARD_NUM5,
//     KEYBOARD_NUM6,
//     KEYBOARD_NUM7,
//     KEYBOARD_NUM8,
//     KEYBOARD_NUM9,
//     KEYBOARD_NUM0,
//     KEYBOARD_MINUS,
//     KEYBOARD_EQUALS,
//     KEYBOARD_BACKSPACE,
//     KEYBOARD_TAB,
//     KEYBOARD_Q,
//     KEYBOARD_W,
//     KEYBOARD_E,
//     KEYBOARD_R,
//     KEYBOARD_T,
//     KEYBOARD_Y,
//     KEYBOARD_U,
//     KEYBOARD_I,
//     KEYBOARD_O,
//     KEYBOARD_P,
//     KEYBOARD_LEFT_BRACKETS,
//     KEYBOARD_RIGHT_BRACKETS,
//     KEYBOARD_ENTER,
//     KEYBOARD_LEFT_CTRL,
//     KEYBOARD_A,
//     KEYBOARD_S,
//     KEYBOARD_D,
//     KEYBOARD_F,
//     KEYBOARD_G,
//     KEYBOARD_H,
//     KEYBOARD_J,
//     KEYBOARD_K,
//     KEYBOARD_L,
//     KEYBOARD_SEMI_COLON,
//     KEYBOARD_SINGLE_QUOTE,
//     KEYBOARD_BACK_TICK,
//     KEYBOARD_LEFT_SHIFT,
//     KEYBOARD_BACKSLASH,
//     KEYBOARD_Z,
//     KEYBOARD_X,
//     KEYBOARD_C,
//     KEYBOARD_V,
//     KEYBOARD_B,
//     KEYBOARD_N,
//     KEYBOARD_M,
//     KEYBOARD_COMMA,
//     KEYBOARD_DOT,
//     KEYBOARD_SLASH,
//     KEYBOARD_RIGHT_SHIFT,
//     KEYBOARD_KEYPAD_ASTERISK,
//     KEYBOARD_LEFT_ALT,
//     KEYBOARD_SPACE,
//     KEYBOARD_CAPS_LOCK,
//     KEYBOARD_F1,
//     KEYBOARD_F2,
//     KEYBOARD_F3,
//     KEYBOARD_F4,
//     KEYBOARD_F5,
//     KEYBOARD_F6,
//     KEYBOARD_F7,
//     KEYBOARD_F8,
//     KEYBOARD_F9,
//     KEYBOARD_F10,
//     KEYBOARD_NUM_LOCK,
//     KEYBOARD_SCROLL_LOCK,
//     KEYBOARD_KEYPAD_NUM7,
//     KEYBOARD_KEYPAD_NUM8,
//     KEYBOARD_KEYPAD_NUM9,
//     KEYBOARD_KEYPAD_MINUS,
//     KEYBOARD_KEYPAD_NUM4,
//     KEYBOARD_KEYPAD_NUM5,
//     KEYBOARD_KEYPAD_NUM6,
//     KEYBOARD_KEYPAD_PLUS,
//     KEYBOARD_KEYPAD_NUM1,
//     KEYBOARD_KEYPAD_NUM2,
//     KEYBOARD_KEYPAD_NUM3,
//     KEYBOARD_KEYPAD_NUM0,
//     KEYBOARD_KEYPAD_DOT,
//     KEYBOARD_NULL2,
//     KEYBOARD_NULL3,
//     KEYBOARD_NULL4,
//     KEYBOARD_F11,
//     KEYBOARD_F12,
//     KEYBOARD_NULL5,
//     KEYBOARD_NULL6,
//     KEYBOARD_NULL7,
//     KEYBOARD_RELEASED_NULL = 0x80,
//     KEYBOARD_RELEASED_ESCAPE,
//     KEYBOARD_RELEASED_NUM1,
//     KEYBOARD_RELEASED_NUM2,
//     KEYBOARD_RELEASED_NUM3,
//     KEYBOARD_RELEASED_NUM4,
//     KEYBOARD_RELEASED_NUM5,
//     KEYBOARD_RELEASED_NUM6,
//     KEYBOARD_RELEASED_NUM7,
//     KEYBOARD_RELEASED_NUM8,
//     KEYBOARD_RELEASED_NUM9,
//     KEYBOARD_RELEASED_NUM0,
//     KEYBOARD_RELEASED_MINUS,
//     KEYBOARD_RELEASED_EQUALS,
//     KEYBOARD_RELEASED_BACKSPACE,
//     KEYBOARD_RELEASED_TAB,
//     KEYBOARD_RELEASED_Q,
//     KEYBOARD_RELEASED_W,
//     KEYBOARD_RELEASED_E,
//     KEYBOARD_RELEASED_R,
//     KEYBOARD_RELEASED_T,
//     KEYBOARD_RELEASED_Y,
//     KEYBOARD_RELEASED_U,
//     KEYBOARD_RELEASED_I,
//     KEYBOARD_RELEASED_O,
//     KEYBOARD_RELEASED_P,
//     KEYBOARD_RELEASED_LEFT_BRACKETS,
//     KEYBOARD_RELEASED_RIGHT_BRACKETS,
//     KEYBOARD_RELEASED_ENTER,
//     KEYBOARD_RELEASED_LEFT_CTRL,
//     KEYBOARD_RELEASED_A,
//     KEYBOARD_RELEASED_S,
//     KEYBOARD_RELEASED_D,
//     KEYBOARD_RELEASED_F,
//     KEYBOARD_RELEASED_G,
//     KEYBOARD_RELEASED_H,
//     KEYBOARD_RELEASED_J,
//     KEYBOARD_RELEASED_K,
//     KEYBOARD_RELEASED_L,
//     KEYBOARD_RELEASED_SEMI_COLON,
//     KEYBOARD_RELEASED_SINGLE_QUOTE,
//     KEYBOARD_RELEASED_BACK_TICK,
//     KEYBOARD_RELEASED_LEFT_SHIFT,
//     KEYBOARD_RELEASED_BACKSLASH,
//     KEYBOARD_RELEASED_Z,
//     KEYBOARD_RELEASED_X,
//     KEYBOARD_RELEASED_C,
//     KEYBOARD_RELEASED_V,
//     KEYBOARD_RELEASED_B,
//     KEYBOARD_RELEASED_N,
//     KEYBOARD_RELEASED_M,
//     KEYBOARD_RELEASED_COMMA,
//     KEYBOARD_RELEASED_DOT,
//     KEYBOARD_RELEASED_SLASH,
//     KEYBOARD_RELEASED_RIGHT_SHIFT,
//     KEYBOARD_RELEASED_KEYPAD_ASTERISK,
//     KEYBOARD_RELEASED_LEFT_ALT,
//     KEYBOARD_RELEASED_SPACE,
//     KEYBOARD_RELEASED_CAPS_LOCK,
//     KEYBOARD_RELEASED_F1,
//     KEYBOARD_RELEASED_F2,
//     KEYBOARD_RELEASED_F3,
//     KEYBOARD_RELEASED_F4,
//     KEYBOARD_RELEASED_F5,
//     KEYBOARD_RELEASED_F6,
//     KEYBOARD_RELEASED_F7,
//     KEYBOARD_RELEASED_F8,
//     KEYBOARD_RELEASED_F9,
//     KEYBOARD_RELEASED_F10,
//     KEYBOARD_RELEASED_NUM_LOCK,
//     KEYBOARD_RELEASED_SCROLL_LOCK,
//     KEYBOARD_RELEASED_KEYPAD_NUM7,
//     KEYBOARD_RELEASED_KEYPAD_NUM8,
//     KEYBOARD_RELEASED_KEYPAD_NUM9,
//     KEYBOARD_RELEASED_KEYPAD_MINUS,
//     KEYBOARD_RELEASED_KEYPAD_NUM4,
//     KEYBOARD_RELEASED_KEYPAD_NUM5,
//     KEYBOARD_RELEASED_KEYPAD_NUM6,
//     KEYBOARD_RELEASED_KEYPAD_PLUS,
//     KEYBOARD_RELEASED_KEYPAD_NUM1,
//     KEYBOARD_RELEASED_KEYPAD_NUM2,
//     KEYBOARD_RELEASED_KEYPAD_NUM3,
//     KEYBOARD_RELEASED_KEYPAD_NUM0,
//     KEYBOARD_RELEASED_KEYPAD_DOT,
//     KEYBOARD_RELEASED_NULL2,
//     KEYBOARD_RELEASED_NULL3,
//     KEYBOARD_RELEASED_NULL4,
//     KEYBOARD_RELEASED_F11,
//     KEYBOARD_RELEASED_F12,
//     KEYBOARD_RELEASED_NULL5,
//     KEYBOARD_RELEASED_NULL6,
//     KEYBOARD_RELEASED_NULL7,
//     KEYBOARD_MULTIMEDIA_PREV_TRACK = 0xE0,
//     KEYBOARD_NULL8,
//     KEYBOARD_NULL9,
//     KEYBOARD_NULL10,
//     KEYBOARD_NULL11,
//     KEYBOARD_MULTIMEDIA_NEXT_TRACK,
//     KEYBOARD_NULL12,
//     KEYBOARD_NULL13,
//     KEYBOARD_KEYPAD_ENTER,
//     KEYBOARD_RIGHT_CTRL,
//     KEYBOARD_NULL14,
//     KEYBOARD_NULL15,
//     KEYBOARD_MULTIMEDIA_MUTE,
//     KEYBOARD_MULTIMEDIA_CALC,
//     KEYBOARD_MULTIMEDIA_PLAY,
//     KEYBOARD_NULL16,
//     KEYBOARD_MULTIMEDIA_STOP,
//     KEYBOARD_NULL17,
//     KEYBOARD_NULL18,
//     KEYBOARD_NULL19,
//     KEYBOARD_NULL20,
//     KEYBOARD_NULL21,
//     KEYBOARD_MULTIMEDIA_VOL_DOWN,
//     KEYBOARD_NULL22,
//     KEYBOARD_MULTIMEDIA_VOL_UP,
//     KEYBOARD_NULL23,
//     KEYBOARD_WWWW_HOME,
//     KEYBOARD_NULL24,
//     KEYBOARD_NULL25,
//     KEYBOARD_KEYPAD_SLASH,
//     KEYBOARD_NULL26,
//     KEYBOARD_NULL27,
//     KEYBOARD_RIGHT_ALT,
//     KEYBOARD_NULL28,
//     KEYBOARD_NULL29,
//     KEYBOARD_NULL30,
//     KEYBOARD_NULL31,
//     KEYBOARD_NULL32,
//     KEYBOARD_NULL33,
//     KEYBOARD_HOME
//     // TODO: Put Release Key Scancodes
// };

bool shiftpressed;
bool altpressed;
bool ctrlpressed;

char get_keyboardkey(int scancode);
void keyboard_init(void);
char keyboard_get_char();
void keyboard_add_handler(void (*handler)(uint8_t scancode, bool released));
void keyboard_remove_handler();