#include "oam.h"
#include "types.h"
#include "memreg.h"
#include "pal.h"
#include "video.h"

#include "menu_screen.h"
#include "sprite.h"

#define CBB0  0
#define CBB4  4
#define TILE0 0
#define SBB30 30

int NORETURN main(void)
{
        REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ;
        REG_BGCNT[0] = BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_SIZE_32x32;

        // =====================================================
        //		Menu Screen Setup
        // =====================================================

        // (1) Load background palette into PALRAM.
        load_pal((u32 *)BG_PAL, (const u32 *)menu_screenPal, menu_screenPalLen);

        // (2) Load background tiles into CBB0.
        load_tiles4(CBB0,
                    TILE0,
                    (const TILE *)menu_screenTiles,
                    menu_screenTilesLen);

        // (3) Load tilemap into SBB30.
        load_tilemap(SBB30,
                     (const u32 *)menu_screenMetaTiles,
                     menu_screenMetaTilesLen);

        // =====================================================
        //		Sprite Setup
        // =====================================================

        // (1) Blank all sprites (they are active on startup).
        hide_obj_oam_buf();
        update_entire_oam();
        // (2) Load sprite0 tiles into OVRAM.
        load_tiles4(CBB4, TILE0, (const TILE *)spriteTiles, spriteTilesLen);
        // (3) Load object palbank into PALRAM.
        load_pal((u32 *)SPRITE_PAL, (const u32 *)spritePal, spritePalLen);
        // (4) Load sprite0 attributes into OAM.
        update_obj_oam_buf(0,
                           ATTR0_4BPP | ATTR0_SQUARE,
                           ATTR1_SIZE_8x8,
                           ATTR2_ID(0) | ATTR2_PALBANK(0));
        update_oam(0);

        while (1)
                ;
}
