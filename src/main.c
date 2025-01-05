#include "input.h"
#include "memreg.h"
#include "oam.h"
#include "pal.h"
#include "types.h"
#include "video.h"

#include "grass.h"
#include "menu_screen.h"
#include "sprite.h"
#include "berries.h"
#include "utils.h"

static ELEMENTS elements;

static enum STATE { MENU = 0, PAUSE, PLAY, LOSE } state;
static s32 main_obj_x, main_obj_y;

int NORETURN main(void)
{
        REG_DISPCNT = DCNT_MODE0 | DCNT_OBJ_1D;

        // =====================================================
        //		Menu Rendering Setup
        // =====================================================

        init_bg_pal();
        show_menu();

        // =====================================================
        //		Sprite Rendering Setup
        // =====================================================

        init_sprites();
        SPRITE berry1 = {
                .bg_coord = { 0, 0 },
                  .attr = &oam_buffer[OBJ(1)]
        };
        SPRITE berry2 = {
                .bg_coord = { 0, 240 },
                  .attr = &oam_buffer[OBJ(2)]
        };
        SPRITE berry3 = {
                .bg_coord = { 240, 0 },
                  .attr = &oam_buffer[OBJ(3)]
        };
        SPRITE berry4 = {
                .bg_coord = { 240, 240 },
                  .attr = &oam_buffer[OBJ(4)]
        };

restart:
        main_obj_x = SPRITE_X_MID - 4;
        main_obj_y = SPRITE_Y_MID - 4;
        bg_offset_buf[BG(0)].x = 0;
        bg_offset_buf[BG(0)].y = 0;
        update_bg_offset(BG(0));
        show_menu();

        state = MENU;
        while (1) {
                // (1) Key processing
                key_poll();
                switch (state) {
                case MENU:
                        if (key_hit(KEY_START)) {
                                state = PLAY;
                                // Activate main sprite and restore coordinates.
                                oam_buf(OBJ(0),
                                        ATTR0_4BPP | ATTR0_SQUARE,
                                        ATTR1_SIZE_8x8,
                                        ATTR2_ID(0) | ATTR2_PALBANK(0));
                                oam_buf_coord(OBJ(0), main_obj_x, main_obj_y);
                                vid_vsync();
                                update_oam(OBJ(0));

                                // Activate berry and restore coordinates.
                                // TODO: Render berries all at once, in
                                // designated areas based on berry sprite
                                // attributes and background/screen coordinates.
                                // The objects must start hidden then appear
                                // after they move on screen.
                                oam_buf(OBJ(1),
                                        ATTR0_Y(50),
                                        ATTR1_SIZE_16 | ATTR1_VFLIP |
                                                ATTR1_X(50),
                                        ATTR2_ID(1) | ATTR2_PALBANK(1));
                                oam_buf(OBJ(2),
                                        ATTR0_Y(50),
                                        ATTR1_SIZE_16 | ATTR1_VFLIP |
                                                ATTR1_X(50),
                                        ATTR2_ID(1) | ATTR2_PALBANK(1));
                                oam_buf(OBJ(3),
                                        ATTR0_Y(50),
                                        ATTR1_SIZE_16 | ATTR1_VFLIP |
                                                ATTR1_X(50),
                                        ATTR2_ID(1) | ATTR2_PALBANK(1));
                                oam_buf(OBJ(4),
                                        ATTR0_Y(50),
                                        ATTR1_SIZE_16 | ATTR1_VFLIP |
                                                ATTR1_X(50),
                                        ATTR2_ID(1) | ATTR2_PALBANK(1));
                                // TODO: Copy all berry sprites to oam.
                                // Maybe change update_oam() functionality to
                                // take a range.
                                vid_vsync();
                                update_oam(OBJ(1));
                                update_oam(OBJ(2));
                                update_oam(OBJ(3));
                                update_oam(OBJ(4));
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
                        main_sprite_motion_buf();

                        // (2) Moves element according to screen positioning
                        // within bg.
                        move_element(&berry1);
                        move_element(&berry2);
                        move_element(&berry3);
                        move_element(&berry4);

                        // (3) Vsync before copying into important areas.
                        vid_vsync();

                        // (4) Update sprite attributes for coordinates.
                        update_oam(OBJ(0));
                        update_oam(OBJ(1));
                        update_oam(OBJ(2));
                        update_oam(OBJ(3));
                        update_oam(OBJ(4));

                        // (5) Update background coordinates.
                        update_bg_offset(BG(0));

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
        load_tiles4(CBB(0),
                    TILE(0),
                    (const TILE *)menu_screenTiles,
                    menu_screenTilesLen);

        // (2) Load tilemap into SBB30.
        load_tilemap(SBB(30),
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
        load_tiles4(CBB(0), TILE(0), (const TILE *)grassTiles, grassTilesLen);

        // (2) Load tilemap into SBB30.
        load_tilemap(
                SBB(30), (const u32 *)grassMetaTiles, grassMetaTilesLen, 16);

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
        dim_palette((u16 *)BG_PALBANK[1], PALBANK_LEN, 10);
}

/**
 * @brief - Hide all sprites and load sprite palettes and tiles.
 */
INLINE void init_sprites(void)
{
        // TODO:
        //      (1) Make main sprite larger.
        //	(2) Create thorn sprites.

        // (1) Blank all sprites (they are active on startup).
        hide_sprites();

        // (2) Load object palbank into PALRAM.
        load_pal((u32 *)OBJ_PALBANK[0], (const u32 *)spritePal, spritePalLen);
        load_pal((u32 *)OBJ_PALBANK[1], (const u32 *)berriesPal, berriesPalLen);

        // (3) Load sprite0 tiles into OVRAM.
        load_tiles4(CBB(4), TILE(0), (const TILE *)spriteTiles, spriteTilesLen);
        load_tiles4(
                CBB(4), TILE(1), (const TILE *)berriesTiles, berriesTilesLen);

        // (4) Update the oam.
        vid_vsync();
        update_entire_oam();
}

INLINE void move_sprite(s32 horz, s32 vert);
INLINE void move_bg(u32 id, s32 horz, s32 vert);
/**
 * @brief Move sprite and/or background based on key_poll() current keys and
 * postion of the sprite on the screen.
 *
 * @param id - Background to move if need be.
 */
INLINE void main_sprite_motion_buf(void)
{
        // Read current keys from poll_keys().
        s32 vert = key_tri_vert(), horz = key_tri_horz();
        if ((horz == -1 && main_obj_x > SPRITE_X_MIN) ||
            (horz == 1 && main_obj_x < SPRITE_X_MAX))
                move_sprite(horz, 0);
        else
                move_bg(0, horz, 0);

        if ((vert == -1 && main_obj_y > SPRITE_Y_MIN) ||
            (vert == 1 && main_obj_y < SPRITE_Y_MAX))
                move_sprite(0, vert);
        else
                move_bg(0, 0, vert);
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
        // s9.

        bg_offset_buf[id].x += horz;
        bg_offset_buf[id].y += vert;
}

INLINE s16 s16_to_u8(s16 x)
{
        return (x < 0) ? (0x100 - (-x & 0xFF)) & 0xFF : x & 0xFF;
}

#define u8(n)      ((n) & 0xFF)
#define wrapped(n) ((n) - 256)
/**
 * @brief - Move element if in screen.
 * Accounts for background is wrapped.
 * Handles only 32x32 tiled bgs for now.
 *
 * @param sprite - Pointer to sprite struct.
 */
INLINE void move_element(SPRITE *sprite)
{
        // FIX: If sprite is along [240, 255], then it never gets moved.
        //
        // (1) Get sprite bg coordinates.
        // ysb = 245;
        s16 xsb = sprite->bg_coord.x;
        s16 ysb = sprite->bg_coord.y;

        // (2) Get bg coordinates.
        // y = 230
        BG_POINT sc = bg_offset_buf[BG(0)];
        BG_POINT u8_sc = {
                .x = s16_to_u8(sc.x),
                .y = s16_to_u8(sc.y),
        };

        // (3) Get sprite size.
        u16 size = 16;

        // (4) If sprite on screen, update screen coordinates.
        // Point to attr buffer.
        OBJ_ATTR *attr = sprite->attr;

        // Wrap sprite coordinates if on outer edge
        if (u8(ysb + size) == wrapped(ysb) + size)
                ysb = wrapped(ysb);
        if (u8(xsb + size) == wrapped(xsb) + size)
                xsb = wrapped(xsb);

        // y-coord
        // (Case 1): Bottom sprite side is below top unwrapped bg side.
        if (u8(ysb + size) >= u8_sc.y) {
                // Top sprite side is above bottom unwrapped bg side.
                if (ysb <= u8_sc.y + SCREEN_HEIGHT)
                        BF_SET(&attr->attr0, ysb - u8_sc.y, ATTR0_Y);
                // (Case 2): Bottom sprite side is above top wrapped bg side
                // AND top sprite side is above bottom wrapped bg side.
        } else if (ysb <= wrapped(u8_sc.y) + SCREEN_HEIGHT) {
                BF_SET(&attr->attr0, ysb - wrapped(u8_sc.y), ATTR0_Y);
        }

        // x-coord
        // (Case 1): Right sprite side is ahead of left unwrapped screen
        // side.
        if (u8(xsb + size) >= u8_sc.x) {
                // Left sprite side is behind of right unwrapped screen
                // side.
                if (xsb <= u8_sc.x + SCREEN_WIDTH)
                        BF_SET(&attr->attr1, xsb - u8_sc.x, ATTR1_X);
                // (Case 2): Right sprite side is behind of left wrapped
                // bg side AND left sprite side is ahead of right
                // wrapped bg side.
        } else if (xsb <= wrapped(u8_sc.x) + SCREEN_WIDTH) {
                BF_SET(&attr->attr1, xsb - wrapped(u8_sc.x), ATTR1_X);
        }
}

// Has to move everything that is not the main sprite, including berries
// and thorns. Should the every other sprite be put in a struct that
// contains berries and thorn arrays?
INLINE void move_elements(void)
{
        for (u32 i = 0; i < elements.b_len; i++)
                move_element(&elements.berries[i]);

        for (u32 i = 0; i < elements.t_len; i++)
                move_element(&elements.thorns[i]);
}
