
//{{BLOCK(grass)

//======================================================================
//
//	grass, 16x16@4,
//	+ palette 15 entries, not compressed
//	+ 4 tiles not compressed
//	+ regular map (flat), not compressed, 1x1
//	Metatiled by 2x2
//	Total size: 30 + 128 + 8 + 2 = 168
//
//	Time-stamp: 2025-01-02, 14:06:07
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_GRASS_H
#define GRIT_GRASS_H
#include "types.h"

#define grassTilesLen 128
extern const u16 grassTiles[64];

#define grassMetaTilesLen 128
extern const u16 grassMetaTiles[64];

#define grassPalLen 32
extern const u16 grassPal[16];

#endif // GRIT_GRASS_H

//}}BLOCK(grass)
