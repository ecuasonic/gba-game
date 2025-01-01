
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

#ifndef GRIT_SPRITE_H
#define GRIT_SPRITE_H
#include "types.h"

#define spriteTilesLen 32
extern const u16 spriteTiles[16];

#define spritePalLen 32
extern const u16 spritePal[16];

#endif // GRIT_SPRITE_H

//}}BLOCK(sprite)
