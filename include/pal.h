#ifndef PAL_H
#define PAL_H

#include "types.h"

// Background palette: PALRAM to PALARM + 01FFh
// Sprite palette: PALRAM + 0200h to PALRAM + 03FFh

typedef u16 COLOR;
typedef u32 PALBANK[8];

#define BG_PAL      ((COLOR *)MEM_PAL)
#define BG_PALBANK  ((PALBANK *)MEM_PAL)
#define PALBANK_LEN (32)
#define OBJ_PAL     ((COLOR *)(MEM_PAL + 0x0200))
#define OBJ_PALBANK ((PALBANK *)(MEM_PAL + 0x0200))

void load_pal(u32 *dest, const u32 *src, u32 ncolors);
#define copy_pal_sprite(src, ncolors) copy_pal(SPRITE, src, ncolors)
#define copy_pal_bg(src, ncolors)     copy_pal(BG, src, ncolors)

#endif
