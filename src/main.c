#include "input.h"
#include "memreg.h"
#include "oam.h"
#include "pal.h"
#include "types.h"
#include "video.h"

#include "grass.h"
#include "menu_screen.h"
#include "sprite.h"

#define CBB0         0
#define CBB4         4
#define TILE0        0
#define SBB30        30

// 0-239
#define SPRITE_X_MIN 100
#define SPRITE_X_MAX 139
#define SPRITE_X_MID ((SPRITE_X_MIN + SPRITE_X_MAX) / 2)
// 0-159
#define SPRITE_Y_MIN 60
#define SPRITE_Y_MAX 99
#define SPRITE_Y_MID ((SPRITE_Y_MIN + SPRITE_Y_MAX) / 2)

INLINE void show_menu(void);
INLINE void show_play(void);
INLINE void init_sprites(void);
INLINE void init_bg_pal(void);
INLINE void motion_buf(u32 id);
INLINE void move_sprite(s32 horz, s32 vert);
INLINE void move_bg(u32 id, s32 horz, s32 vert);

static enum STATE { MENU = 0, PAUSE, PLAY, LOSE } state;
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
        main_obj_x = SPRITE_X_MID - 4;
        main_obj_y = SPRITE_Y_MID - 4;
        ((u32 *)REG_BG_OFS)[0] = 0;
        show_menu();

        state = MENU;
        while (1) {
                // (1) Key processing
                key_poll();
                switch (state) {
                case MENU:
                        if (key_hit(KEY_START)) {
                                state = PLAY;
                                // Activate sprite and restore coordinates.
                                oam_buf(0,
                                        ATTR0_4BPP | ATTR0_SQUARE,
                                        ATTR1_SIZE_8x8,
                                        ATTR2_ID(0) | ATTR2_PALBANK(0));
                                oam_buf_coord(0, main_obj_x, main_obj_y);
                                vid_vsync();
                                update_oam(0);
                                vid_vsync();
                                show_play();
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

                        // (1) Move sprite and background based on key_poll().
                        motion_buf(0);

                        // (2) Vsync before copying to important areas.
                        vid_vsync();

                        // (3) Update sprite attributes for coordinates.
                        update_oam(0);

                        // (4) Update background coordinates.
                        update_bg_offset(0);

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
 * @brief Move sprite and/or background based on key_poll() current keys and
 * postion of the sprite on the screen.
 *
 * @param id - Background to move if need be.
 */
INLINE void motion_buf(u32 id)
{
        // Read current keys from poll_keys().
        s32 vert = key_tri_vert(), horz = key_tri_horz();
        if ((horz == -1 && main_obj_x > SPRITE_X_MIN) ||
            (horz == 1 && main_obj_x < SPRITE_X_MAX))
                move_sprite(horz, 0);
        else
                move_bg(id, horz, 0);

        if ((vert == -1 && main_obj_y > SPRITE_Y_MIN) ||
            (vert == 1 && main_obj_y < SPRITE_Y_MAX))
                move_sprite(0, vert);
        else
                move_bg(id, 0, vert);
}

/**
 * @brief Calculates next sprite position based on previous saved keys
 * (poll_keys()).
 * Places the new position into main_obj_{x,y}.
 * Stores new position into oam_buffer, not oam_mem.
 * Flips sprite horizontally depending on horizontal direction.
 *
 * @param horz - Sprite horizontal offset.
 * @param vert - Sprite vertical offset.
 */
INLINE void move_sprite(s32 horz, s32 vert)
{
        static u16 *main_attr1 = &oam_buffer[0].attr1;

        // Only move sprite if not move_bg;
        main_obj_x += horz;
        main_obj_y += vert;
        oam_buf_coord(0, main_obj_x, main_obj_y);
        if (horz == 1) {
                BF_SET(main_attr1, 0, ATTR1_FLIP);
        } else if (horz == -1)
                BF_SET(main_attr1, 1, ATTR1_FLIP);
}

/**
 * @brief Calcuates next bg position based on previous saved keys (poll_keys()).
 *
 * @param id - Background to move.
 * @param horz - BG horizontal offset.
 * @param vert - BG vertical offset.
 */
INLINE void move_bg(u32 id, s32 horz, s32 vert)
{
        // This is for data processing.
        // Write into bg register buffer for particular background.
        // Then create another function that copies buffer to background.

        bg_offset_buf[id].x += horz;
        bg_offset_buf[id].y += vert;
}
