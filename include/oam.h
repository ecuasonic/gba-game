#ifndef OAM_H
#define OAM_H

#include "types.h"
#include "memdef.h"

// Regular sprite attributes (128):
typedef struct {
        // Y Coordinate, Object mode, Gfx mode, Mosaic effect, Color mode,
        // Sprite shape.
        u16 attr0;
        // X Coordinate, Affine index / Flipping flags, Sprite size.
        u16 attr1;
        // sprite id, priority, palbank.
        u16 attr2;
        u16 fill;
} ALIGN(4) OBJ_ATTR;

// Transformation data (32):
typedef struct {
        u16 fill0[3];
        s16 pa;
        u16 fill1[3];
        s16 pb;
        u16 fill2[3];
        s16 pc;
        u16 fill3[3];
        s16 pd;
} ALIGN(4) OBJ_AFFINE;

extern OBJ_ATTR oam_buffer[128];
#define oam_mem           ((OBJ_ATTR *)MEM_OAM)

// =============================================
//		Attribute 0
// =============================================

// Y (0-7):
//    Y coordinate of sprite on screen.
//    Marks the top of the sprite.
//    Up to 255 pixels.
//    0 - 255 pixels or 0-31 tiles.
#define ATTR0_Y_MASK      0x00FF
#define ATTR0_Y_SHIFT     0
#define ATTR0_Y(n)        ((n) & ATTR0_Y_MASK) // n can accidentally set other values

// OM (8-9):
//    (Affine) object mode.
//    Use to hide the sprite or govern affine mode.
//    If 00, then normal rendering.
//    If 01, then sprite is an affine sprite, using affine matrix specified by
//    attr1{9-D}. If 10, then disable rendering (hide the sprite). If 11, then
//    affine sprite using double rendering area.
#define ATTR0_MODE_MASK   0x0300
#define ATTR0_MODE_SHIFT  8
#define ATTR0_MODE(n)     ((n) << 8)
#define ATTR0_REG         ATTR0_MODE(0)
#define ATTR0_AFF         ATTR0_MODE(1)
#define ATTR0_HIDE        ATTR0_MODE(2)
#define ATTR0_AFF_DBL     ATTR0_MODE(3)

// GM (10-11):
//    Gfx mode.
//    Flags for special effects.
//    If 00, then normal rendering.
//    If 01, then enable alpha rendering.
//    If 10, then object is part of the object window.
//    11 is forbidden.
#define ATTR0_GFX_MASK    0x0C00
#define ATTR0_GFX_SHIFT   10
#define ATTR0_GFX(n)      ((n) << ATTR0_GFX_SHIFT)
#define ATTR0_BLEND       ATTR0_GFX(1)
#define ATTR0_WIN         ATTR0_GFX(2)

// Mos (12):
//    Enables mosaic effect.
#define ATTR0_MOSAIC      (1 << 12)

// CM (13):
//    Color mode.
//    If 0, 16 colors (4bpp).
//    If 1, 256 colors (8bpp).
#define ATTR0_4BPP        0
#define ATTR0_8BPP        (1 << 13)

// Sh (14-15):
//    Sprite shape.
//    This and the sprite's size (attr1{E-F}) determines the sprite's real size.
#define ATTR0_SHAPE_MASK  0xC000
#define ATTR0_SHAPE_SHIFT 14
#define ATTR0_SHAPE(n)    ((n) << ATTR0_SHAPE_SHIFT)
#define ATTR0_SQUARE      ATTR0_SHAPE(0)
#define ATTR0_WIDE        ATTR0_SHAPE(1)
#define ATTR0_TALL        ATTR0_SHAPE(2)

// attr0 Build Macro:
//    Y-coord, shape, bpp, object mode, mosaic, blend / window
#define ATTR0_BUILD(y, shape, bpp, mode, mos, bld, win)                    \
        (((y) & 255) | (((mode) & 3) << 8) | (((bld) & 1) << 10) |         \
         (((win) & 1) << 11) | (((mos) & 1) << 12) | (((bpp) & 8) << 10) | \
         (((shape) & 3) << 14))

// =============================================
//		Attribute 1
// =============================================

// X (0-8):
//    X coordinate of sprite on screen.
//    Marks left of the sprite.
#define ATTR1_X_MASK     0x01FF
#define ATTR1_X_SHIFT    0
#define ATTR1_X(n)       ((n) & ATTR1_X_MASK) // n can accidentally set other bits.

// AID (9-13):
//    Affine index.
//    Only if the affine flag (attr0{8}) is set.
//    Specifies the OAM_AFF_ENTY this sprite uses.
#define ATTR1_AFF_MASK   0x3E00
#define ATTR1_AFF_SHIFT  9
#define ATTR1_AFF(n)     ((n) << ATTR1_AFF_SHIFT)

// HF, VF (12-13):
//    Horizontal/vertical flipping flags.
//    Only if the affine flag (attr0{8}) is cleared.
//        Otherwise part of the affine index.
#define ATTR1_FLIP_MASK  0x3000
#define ATTR1_FLIP_SHIFT 12
#define ATTR1_FLIP(n)    ((n) << ATTR1_FLIP_SHIFT)
#define ATTR1_HFLIP      ATTR1_FLIP(1)
#define ATTR1_VFLIP      ATTR1_FLIP(2)
#define ATTR1_HVFLIP     ATTR1_FLIP(3)

// Sz (14-15):
//    Sprite size.
//    Used with shape bits (attr0{13-14}) to determine sprite's real size.
#define ATTR1_SIZE_MASK  0xC000
#define ATTR1_SIZE_SHIFT 14
#define ATTR1_SIZE(n)    ((n) << ATTR1_SIZE_SHIFT)
// Base sizes
#define ATTR1_SIZE_8     ATTR1_SIZE(0)
#define ATTR1_SIZE_16    ATTR1_SIZE(1)
#define ATTR1_SIZE_32    ATTR1_SIZE(2)
#define ATTR1_SIZE_64    ATTR1_SIZE(3)
// Square sizes
#define ATTR1_SIZE_8x8   ATTR1_SIZE(0)
#define ATTR1_SIZE_16x16 ATTR1_SIZE(1)
#define ATTR1_SIZE_32x32 ATTR1_SIZE(2)
#define ATTR1_SIZE_64x64 ATTR1_SIZE(3)
// Tall sizes
#define ATTR1_SIZE_8x16  ATTR1_SIZE(0)
#define ATTR1_SIZE_8x32  ATTR1_SIZE(1)
#define ATTR1_SIZE_16x32 ATTR1_SIZE(2)
#define ATTR1_SIZE_32x64 ATTR1_SIZE(3)
// Wide sizes
#define ATTR1_SIZE_16x8  ATTR1_SIZE(0)
#define ATTR1_SIZE_32x8  ATTR1_SIZE(1)
#define ATTR1_SIZE_32x16 ATTR1_SIZE(2)
#define ATTR1_SIZE_64x32 ATTR1_SIZE(3)

// attr1 Build Macros:
//    Regular sprites.
//    X-coord, size, flips.
#define ATTR1_BUILDR(x, size, hflip, vflip)                            \
        (((x) & 511) | (((hflip) & 1) << 12) | (((vflip) & 1) << 13) | \
         (((size) & 3) << 14))
//    Affine sprites.
//    X-coord, size, affine index.
#define ATTR1_BUILDA(x, size, affid)                                 \
        (((x) & 511) | (((affid) & 31) << 9) | (((size) & 3) << 14))

// =============================================
//		Attribute 2
// =============================================

// TID (0-9):
//    Tile index.
//    Specifies the base tile index of the sprite in OAM.
//    For bitmap modes (Modes 3-5), TID >= 512.
//    For tiled modes:
//        Sprite controller reads 32-bytes at a time, regardless of bitdepth.
//        If 4bpp, increment tile index by 1 (32 bytes).
//            0 -> 1 -> 2 -> ...
//        If 8bpp, increment tile index by 2 (64 bytes).
//            0 -> 2 -> 4 -> ...
#define ATTR2_ID_MASK       0x03FF
#define ATTR2_ID_SHIFT      0
#define ATTR2_ID(n)         ((n) << ATTR2_ID_SHIFT)

// Pr (10-11):
//    Priority.
//    Higher priorities are drawn first.
//        And therefore can be drawn over by later sprites and backgrounds.
//    Sprites cover backgrounds of the same priority.
//    For sprites of the same priority, the higher OBJ_ATTRs are drawn first.
#define ATTR2_PRIO_MASK     0x0C00
#define ATTR2_PRIO_SHIFT    10
#define ATTR2_PRIO(n)       ((n) << ATTR2_PRIO_SHIFT)

// PB (12-15):
//    Palette Bank.
//    Which subpalette to use when in 16-color mode.
//    No effect if color mode flag attr0{12} is set.
#define ATTR2_PALBANK_MASK  0xF000
#define ATTR2_PALBANK_SHIFT 12
#define ATTR2_PALBANK(n)    ((n) << ATTR2_PALBANK_SHIFT)

// attr2 Build Macros:
//    tile index, palette bank, sprite priority
#define ATTR2_BUILD(id, pbank, prio)                                     \
        (((id) & 0x3FF) | (((pbank) & 15) << 12) | (((prio) & 3) << 10))

// =====================================
//    bit field set and get routines:
// =====================================
// name = bit field macro name
// y = attr#
// x = value

// Place value into correct bit field position.
#define BF_PREP(x, name)    (((x) << name##_SHIFT) & name##_MASK)
// Shift value down to bit 0, then return.
#define BF_GET(y, name)     (((*y) & name##_MASK) >> name##_SHIFT)
// Place value in correct bit position and set y.
#define BF_SET(y, x, name)  (*y = ((*y) & ~name##_MASK) | BF_PREP(x, name))

// Get value in bit field position.
#define BF_PREP2(x, name)   ((x) & name##_MASK)
#define BF_GET2(y, name)    ((y) & name##_MASK)
// Get value in correct bit position and set y.
#define BF_SET2(y, x, name) (y = ((y) & ~name##_MASK) | BF_PREP2(x, name))

// ==================================================================
//			OAM_BUFFER
// ==================================================================
INLINE void hide_sprites(void)
{
        u32 *dest = (u32 *)oam_buffer;
        u32  count = 128;
        while (count--) {
                *dest++ = ATTR0_HIDE;
                *dest++ = 0;
        }
}
INLINE void oam_buf(u32 id, u16 attr0, u16 attr1, u16 attr2)
{
        OBJ_ATTR *dest = &oam_buffer[id];
        dest->attr0 = attr0;
        dest->attr1 = attr1;
        dest->attr2 = attr2;
}
INLINE void oam_buf_attr0(u32 id, u16 attr0) { oam_buffer[id].attr0 = attr0; }
INLINE void oam_buf_attr1(u32 id, u16 attr1) { oam_buffer[id].attr1 = attr1; }
INLINE void oam_buf_attr2(u32 id, u16 attr2) { oam_buffer[id].attr2 = attr2; }
INLINE void oam_buf_coord(u32 id, s32 x, s32 y)
{
        u16 *attr0 = &oam_buffer[id].attr0;
        u16 *attr1 = &oam_buffer[id].attr1;
        BF_SET(attr0, y, ATTR0_Y);
        BF_SET(attr1, x, ATTR1_X);
}
// ==================================================================
//			OAM_MEM
// ==================================================================
INLINE void update_oam(u32 id) { oam_mem[id] = oam_buffer[id]; }
INLINE void update_entire_oam(void)
{
        OBJ_ATTR *dest = oam_mem;
        OBJ_ATTR *src = oam_buffer;
        for (u32 i = 0; i < 128; i++)
                *dest++ = *src++;
}

#endif // OAM_H
