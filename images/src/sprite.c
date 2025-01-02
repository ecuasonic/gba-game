
//{{BLOCK(sprite)

//======================================================================
//
//	sprite, 8x8@4,
//	+ palette 15 entries, not compressed
//	+ 1 tiles not compressed
//	+ regular map (flat), not compressed, 1x1
//	Total size: 30 + 32 + 2 = 64
//
//	Time-stamp: 2024-12-31, 19:40:41
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================
#include "types.h"
#include "sprite.h"

const u16 spriteTiles[16] ALIGN(4) = {
        0x0000, 0x0000, 0xAC00, 0x0000, 0x6C70, 0x00FD, 0x6C70, 0x0066,
        0xCC70, 0x00CC, 0xC770, 0x00CC, 0x7700, 0x0077, 0x0700, 0x0070,
};

const u16 spritePal[16] ALIGN(4) = {
        0x0000, 0x0400, 0x1001, 0x2023, 0x2506, 0x3046, 0x3588, 0x3C27,
        0x41EA, 0x4469, 0x508B, 0x568F, 0x5C8D, 0x6F32, 0x77DA, 0xFFFF,
};

//}}BLOCK(sprite)
