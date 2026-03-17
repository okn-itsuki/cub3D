#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H



#define PLAYER_MOVE_SPEED 3.0
#define PLAYER_ROT_SPEED 2.0
#define FOV_HALF_TAN 0.66

// linux,mac対応のゴリ押しkeycode
# if defined(__linux__)
#  define PLATFORM_LINUX 1
# elif defined(__APPLE__) && defined(__MACH__)
#  define PLATFORM_MACOS 1
# endif


# if defined(PLATFORM_LINUX)

#  include <X11/keysym.h>

#  define KEY_ESC       XK_Escape
#  define KEY_LEFT      XK_Left
#  define KEY_RIGHT     XK_Right
#  define KEY_UP        XK_Up
#  define KEY_DOWN      XK_Down
#  define KEY_W         XK_w
#  define KEY_A         XK_a
#  define KEY_S         XK_s
#  define KEY_D         XK_d

# elif defined(PLATFORM_MACOS)

#  define KEY_ESC       53
#  define KEY_LEFT      123
#  define KEY_RIGHT     124
#  define KEY_DOWN      125
#  define KEY_UP        126
#  define KEY_A         0
#  define KEY_S         1
#  define KEY_D         2
#  define KEY_W         13

# endif

#endif
