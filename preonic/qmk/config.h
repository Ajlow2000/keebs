#pragma once

#define TAPPING_TOGGLE 2
#define TAPPING_TERM 200
#define TAPPING_TERM_PER_KEY

#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD
#define CAPS_WORD_INVERT_ON_SHIFT

// Activate Command with Left Ctrl + Right Ctrl.
#define IS_COMMAND() (get_mods() == MOD_MASK_CTRL)

// Mouse key speed and acceleration.
#undef MOUSEKEY_DELAY
#define MOUSEKEY_DELAY          0
#undef MOUSEKEY_INTERVAL
#define MOUSEKEY_INTERVAL       16
#undef MOUSEKEY_WHEEL_DELAY
#define MOUSEKEY_WHEEL_DELAY    0
#undef MOUSEKEY_MAX_SPEED
#define MOUSEKEY_MAX_SPEED      6
#undef MOUSEKEY_TIME_TO_MAX
#define MOUSEKEY_TIME_TO_MAX    64
