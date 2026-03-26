#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H


#define BITS_PER_BYTE 8

#define MIN_PERP_WALL_DIST   1e-4
#define CAMERA_X_MIN         (-1.0)
#define CAMERA_X_SCALE       2.0
#define RAY_DELTA_INF        1e30
#define MAP_WALL_CELL        '1'

#define PLAYER_MOVE_SPEED 3.0
#define PLAYER_ROT_SPEED 3.0
#define FOV_HALF_TAN 0.6
#define COLLISION_MARGIN 0.2


#define EVENT_KEY_PRESS      2 
#define EVENT_KEY_RELEASE    3 
#define EVENT_DESTROY        17
#define MASK_KEY_PRESS       (1L << 0)
#define MASK_KEY_RELEASE     (1L << 1)


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
