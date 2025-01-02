#include "input.h"
#include "memreg.h"
#include "oam.h"
#include "pal.h"
#include "types.h"
#include "video.h"

#include "grass.h"
#include "menu_screen.h"
#include "sprite.h"

#define CBB0  0
#define CBB4  4
#define TILE0 0
#define SBB30 30

INLINE void show_menu(void);
INLINE void show_play(void);
INLINE void init_sprites(void);
INLINE void init_bg_pal(void);
INLINE void move_sprite(void);
INLINE void move_bg(u32 id);

static enum STATE { MENU = 0, PAUSE, PLAY, LOSE } state = MENU;
static s32 main_obj_x, main_obj_y;

int NORETURN main(void)
{
        REG_DISPCNT = DCNT_MODE0;

        // =====================================================
        //		Menu Rendering Setup
        // =====================================================

        // (1) Load background palette into PALRAM.
        init_bg_pal();

        // (2) Show menu to screen.
        show_menu();

        // =====================================================
        //		Sprite Rendering Setup
        // =====================================================

        init_sprites();

restart:
        main_obj_x = main_obj_y = 0;
        show_menu();

        state = MENU;
        while (1) {
                // (1) Key processing
                key_poll();
                switch (state) {
                case MENU:
                        if (key_hit(KEY_START)) {
                                state = PLAY;
                                show_play();
                                // Activate sprite.
                                oam_buf(0,
                                        ATTR0_4BPP | ATTR0_SQUARE,
                                        ATTR1_SIZE_8x8,
                                        ATTR2_ID(0) | ATTR2_PALBANK(0));
                        }
                        break;
                case PAUSE:
                        break;
                case PLAY:
                        if (key_hit(KEY_SELECT))
                                // Eventually this goes to pause, which
                                // activates a second background that displays
                                // options such as restart and resume. Along
                                // with dimming main background palette.
                                goto restart;
                        // =====================================================
                        //		Sprite Movement
                        // =====================================================

                        // (1) Calculate controls for sprite.
                        move_sprite();

                        // (2) Calculate background coordinates, based on main_x
                        // and main_y.
                        move_bg(0);

                        // (3) Vsync.
                        vid_vsync();

                        // (4) Update sprite attributes for coordinates.
                        update_oam(0);

                        // (5) Update background coordinates.


                        break;
                case LOSE:
                        break;
                default:
                        break;
                }
        }
}

/**
 * @brief - Show menu background.
 */
INLINE void show_menu(void)
{
        // (1) Load background tiles into CBB0.
        load_tiles4(CBB0,
                    TILE0,
                    (const TILE *)menu_screenTiles,
                    menu_screenTilesLen);

        // (2) Load tilemap into SBB30.
        load_tilemap(SBB30,
                     (const u32 *)menu_screenMetaTiles,
                     menu_screenMetaTilesLen,
                     1);

        // (3) Set registers to show BG0.
        BF_SET(&REG_DISPCNT, 0x01, DCNT_LAYER);
        BF_SET(&REG_BGCNT[0], 0, BG_CBB);
        BF_SET(&REG_BGCNT[0], 30, BG_SBB);
}

/**
 * @brief - Show play background.
 */
INLINE void show_play(void)
{
        // (1) Load background tiles into CBB0.
        load_tiles4(CBB0, TILE0, (const TILE *)grassTiles, grassTilesLen);

        // (2) Load tilemap into SBB30.
        load_tilemap(SBB30, (const u32 *)grassMetaTiles, grassMetaTilesLen, 16);

        // (3) Set registers to show BG0.
        BF_SET(&REG_DISPCNT, 0x11, DCNT_LAYER);
        BF_SET(&REG_BGCNT[0], 0, BG_CBB);
        BF_SET(&REG_BGCNT[0], 30, BG_SBB);
}

/**
 * @brief - Load background palettes/palbanks into palram.
 */
INLINE void init_bg_pal(void)
{
        // (1) Load background palettes into PALRAM.
        load_pal((u32 *)BG_PALBANK[0],
                 (const u32 *)menu_screenPal,
                 menu_screenPalLen);
        load_pal((u32 *)BG_PALBANK[1], (const u32 *)grassPal, grassPalLen);
        dim_palette((u16 *)BG_PALBANK[1], PALBANK_LEN, 18);
}

/**
 * @brief - Hide all sprites and load sprite palettes and tiles.
 */
INLINE void init_sprites(void)
{
        // TODO:
        //	(1) Create berry and thorn sprites.
        //	(2) If main sprite is not loaded, then don't move it.

        // (1) Blank all sprites (they are active on startup).
        hide_sprites();

        // (2) Load object palbank into PALRAM.
        load_pal((u32 *)OBJ_PALBANK[0], (const u32 *)spritePal, spritePalLen);

        // (3) Load sprite0 tiles into OVRAM.
        load_tiles4(CBB4, TILE0, (const TILE *)spriteTiles, spriteTilesLen);

        // (4) Update the oam.
        vid_vsync();
        update_entire_oam();
}

/**
 * @brief Calculates next sprite position based on previous saved keys
 * (poll_keys()).
 * Places the new position into main_obj_{x,y}.
 * Stores new position into oam_buffer, not oam_mem.
 * Flips sprite horizontally depending on horizontal direction.
 */
INLINE void move_sprite(void)
{
        static s32  horz;
        static u16 *main_attr1 = &oam_buffer[0].attr1;

        main_obj_x += (horz = key_tri_horz());
        main_obj_y += key_tri_vert();
        oam_buf_coord(0, main_obj_x, main_obj_y);
        if (horz == 1) {
                BF_SET(main_attr1, 0, ATTR1_FLIP);
        } else if (horz == -1)
                BF_SET(main_attr1, 1, ATTR1_FLIP);
}

INLINE void move_bg(u32 id)
{
        // This is for data processing.
        // Write into bg register buffer for particular background.
        // Then create another function that copies buffer to background.
        //
        // TODO:
        // (1) Get sprite coordinates through main_obj_{x,y}.
        // (2) If either coordinates is within 2 tiles from bg border, then move
        // background.
}
