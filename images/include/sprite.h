
//{{BLOCK(sprite)

//======================================================================
//
//	sprite, 16x16@4,
//	+ palette 15 entries, not compressed
//	+ 4 tiles Metatiled by 2x2 not compressed
//	Total size: 30 + 128 = 158
//
//	Time-stamp: 2025-01-04, 20:07:11
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_SPRITE_H
#define GRIT_SPRITE_H
#include "types.h"

#define spriteTilesLen 128
extern const u16 spriteTiles[64];

#define spritePalLen 30
extern const u16 spritePal[16];

#endif // GRIT_SPRITE_H

//}}BLOCK(sprite)
