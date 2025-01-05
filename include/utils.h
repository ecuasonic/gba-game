#include "types.h"
#include "oam.h"

#define CBB(n)        (n)
#define SBB(n)        (n)
#define TILE(n)       (n)

#define OBJ(n)        (n)
#define BG(n)         (n)

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 160

// 0-239
#define SPRITE_X_MIN  100
#define SPRITE_X_MAX  139
#define SPRITE_X_MID  ((SPRITE_X_MIN + SPRITE_X_MAX) / 2)
// 0-159
#define SPRITE_Y_MIN  60
#define SPRITE_Y_MAX  99
#define SPRITE_Y_MID  ((SPRITE_Y_MIN + SPRITE_Y_MAX) / 2)

INLINE void show_menu(void);
INLINE void show_play(void);
INLINE void init_sprites(void);
INLINE void init_bg_pal(void);
INLINE void main_sprite_motion_buf(void);
INLINE void move_elements(void);

typedef struct {
        s16 x;
        s16 y;
} POINT;
typedef struct {
        const POINT bg_coord;
        OBJ_ATTR   *attr;
        /*u32 active;*/
} SPRITE;
typedef struct {
        SPRITE *berries;
        SPRITE *thorns;
        u16     b_len;
        u16     t_len;
} ELEMENTS;
INLINE void move_element(SPRITE *sprite);
