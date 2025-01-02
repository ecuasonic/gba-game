#include "oam.h"
#include "types.h"
#include "memreg.h"
#include "pal.h"
#include "video.h"
#include "input.h"

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
        //		Menu Rendering Setup
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

        // TODO:
        //	(1) Convert menu loading segment into function.
        //	(2) Create function that loads play-background (32x32 tiles).

        // =====================================================
        //		Sprite Rendering Setup
        // =====================================================

        // (1) Blank all sprites (they are active on startup).
        hide_sprites();

        // (2) Load object palbank into PALRAM.
        load_pal((u32 *)SPRITE_PAL, (const u32 *)spritePal, spritePalLen);

        // (3) Load sprite0 tiles into OVRAM.
        load_tiles4(CBB4, TILE0, (const TILE *)spriteTiles, spriteTilesLen);

        // (4) Load sprite0 attributes into OAM.
        // This line loads sprite into menu.
        /*oam_buf(0,*/
        /*        ATTR0_4BPP | ATTR0_SQUARE,*/
        /*        ATTR1_SIZE_8x8,*/
        /*        ATTR2_ID(0) | ATTR2_PALBANK(0));*/
        vid_vsync();
        update_entire_oam();

        // =====================================================
        //		Berries/Thorns Rendering Setup
        // =====================================================

        // TODO:
        //	(1) Create berry and thorn sprites.
        //	(2) If main sprite is not loaded, then don't move it.


        s32  obj0_x = 0, obj0_y = 0;
        u16 *main_attr1 = &oam_buffer[0].attr1;
        s32  horz = 0;
        while (1) {

                // =====================================================
                //		Sprite Movement
                // =====================================================

                // (1) Include controls for sprite.
                //	Also flip sprite horizontal if going left.
                key_poll();

                obj0_x += (horz = key_tri_horz());
                obj0_y += key_tri_vert();
                oam_buf_coord(0, obj0_x, obj0_y);

                if (horz == 1) {
                        BF_SET(main_attr1, 0, ATTR1_FLIP);
                } else if (horz == -1)
                        BF_SET(main_attr1, 1, ATTR1_FLIP);

                // (2) Vsync.
                vid_vsync();

                // (3) Update sprite attributes for coordinates.
                update_oam(0);

                // TODO:
                //	(1) Switch to play background on START press.
                //	(2) Switch back to menu background on SELECT press.
                //		Eventually evolve this is pause the game.
                //		Where you can select to restart or go to menu.
        }
}
