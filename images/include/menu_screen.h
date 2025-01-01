
//{{BLOCK(menu_screen)

//======================================================================
//
//	menu_screen, 240x160@4,
//	+ palette 15 entries, not compressed
//	+ 600 tiles not compressed
//	+ regular map (flat), not compressed, 1x1
//	Metatiled by 30x20
//	Total size: 30 + 19200 + 1200 + 2 = 20432
//
//	Time-stamp: 2024-12-31, 14:36:06
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MENU_SCREEN_H
#define GRIT_MENU_SCREEN_H
#include "types.h"

#define menu_screenTilesLen 19200
extern const u16 menu_screenTiles[9600];

#define menu_screenMetaTilesLen 1280
extern const u16 menu_screenMetaTiles[1024];

#define menu_screenPalLen 32
extern const u16 menu_screenPal[16];

#endif // GRIT_MENU_SCREEN_H

//}}BLOCK(menu_screen)
