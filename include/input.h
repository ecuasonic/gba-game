#ifndef INPUT_H
#define INPUT_H
#include "types.h"
#include "memdef.h"

// ---------------------------------------------------------------------------
//                       BUTTON INPUT
// ---------------------------------------------------------------------------
#define KEY_MASK            0x03FF
#define REG_KEYINPUT        (*(vu32 *)(REG_BASE + 0x0130))
#define CHECK_KEY_DOWN(key) (~(REG_KEYINPUT) & (key))

extern u16 __key_prev, __key_curr;

INLINE u32 key_transit(u32 key) { return (__key_curr ^ __key_prev) & key; }
INLINE u32 key_held(u32 key) { return (__key_curr & __key_prev) & key; }
INLINE u32 key_hit(u32 key) { return (__key_curr & ~__key_prev) & key; }
INLINE u32 key_released(u32 key) { return (~__key_curr ^ __key_prev) & key; }

INLINE void key_poll(void)
{
        __key_prev = __key_curr;
        __key_curr = ~REG_KEYINPUT & KEY_MASK;
}

// tribool: 1 if {plus} is on, -1 if {minus} is on, 0 if {plus}=={minus}
INLINE int bit_tribool(u32 x, int plus, int minus)
{
        return (((x >> plus) & 1) - ((x >> minus) & 1));
}

#define KEY_A      (1 << 0)
#define KEY_B      (1 << 1)
#define KEY_SELECT (1 << 2)
#define KEY_START  (1 << 3)
#define KEY_RIGHT  (1 << 4)
#define KEY_LEFT   (1 << 5)
#define KEY_UP     (1 << 6)
#define KEY_DOWN   (1 << 7)
#define KEY_R      (1 << 8)
#define KEY_L      (1 << 9)

enum eKeyIndex {
        KI_A = 0,
        KI_B,
        KI_SELECT,
        KI_START,
        KI_RIGHT,
        KI_LEFT,
        KI_UP,
        KI_DOWN,
        KI_R,
        KI_L,
        KI_MAX
};

// --- TRISTATES ---
// You can make more (such as ones involving __key_prev)
INLINE int key_tri_horz(void)
{
        return bit_tribool(__key_curr, KI_RIGHT, KI_LEFT);
}
INLINE int key_tri_vert(void)
{
        return bit_tribool(__key_curr, KI_DOWN, KI_UP);
}
INLINE int key_tri_shoulder(void)
{
        return bit_tribool(__key_curr, KI_R, KI_L);
}
INLINE int key_tri_fire(void) { return bit_tribool(__key_curr, KI_A, KI_B); }

#endif // INPUT_H
