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

#define CBB(n)        (n)
#define SBB(n)        (n)
#define TILE(n)       (n)

#define OBJ(n)        (n)
#define BG(n)         (n)

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 160

// 0-239
#define SPRITE_X_MIN  100
#define SPRITE_X_MAX  139
#define SPRITE_X_MID  ((SPRITE_X_MIN + SPRITE_X_MAX) / 2)
// 0-159
#define SPRITE_Y_MIN  60
#define SPRITE_Y_MAX  99
#define SPRITE_Y_MID  ((SPRITE_Y_MIN + SPRITE_Y_MAX) / 2)

INLINE void show_menu(void);
INLINE void show_play(void);
INLINE void init_sprites(void);
INLINE void init_bg_pal(void);
INLINE void main_sprite_motion_buf(u32 id);
INLINE void move_sprite(s32 horz, s32 vert);
INLINE void move_bg(u32 id, s32 horz, s32 vert);
INLINE void move_elements(void);

typedef struct {
        u32 x;
        u32 y;
} POINT;
typedef struct {
        const POINT bg_coord;
        OBJ_ATTR   *attr;
        /*u32 active;*/
} SPRITE;
typedef struct {
        SPRITE *berries;
        SPRITE *thorns;
        u32     b_len;
        u32     t_len;
} ELEMENTS;
INLINE void move_element(SPRITE *sprite);

static ELEMENTS elements;

static enum STATE { MENU = 0, PAUSE, PLAY, LOSE } state;
static s32   main_obj_x, main_obj_y;
int NORETURN main(void)
{
        REG_DISPCNT = DCNT_MODE0 | DCNT_OBJ_1D;

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
        SPRITE berry = {
                .bg_coord = { 60, 60 },
                  .attr = &oam_buffer[OBJ(1)]
        };

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
                                oam_buf(OBJ(1),
                                        ATTR0_4BPP | ATTR0_SQUARE,
                                        ATTR1_SIZE_16x16 | ATTR1_VFLIP,
                                        ATTR2_ID(1) | ATTR2_PALBANK(1));
                                oam_buf_coord(OBJ(1), main_obj_x, main_obj_y);
                                // TODO: Copy all berry sprites to oam.
                                // Maybe change update_oam() functionality to
                                // take a range.
                                vid_vsync();
                                update_oam(OBJ(1));
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
                        main_sprite_motion_buf(BG(0));

                        // TODO: Create function that checks for each berry
                        // sprite and see whether to unhide/hide and whether to
                        // change the berry coordinates when the background
                        // coordinates change.
                        move_element(&berry);

                        // (2) Vsync before copying into important areas.
                        vid_vsync();

                        // (3) Update sprite attributes for coordinates.
                        update_oam(OBJ(0));
                        update_oam(OBJ(1));

                        // (4) Update background coordinates.
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
        dim_palette((u16 *)BG_PALBANK[1], PALBANK_LEN, 18);
}

/**
 * @brief - Hide all sprites and load sprite palettes and tiles.
 */
INLINE void init_sprites(void)
{
        // TODO:
        //	(1) Create thorn sprites.

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

/**
 * @brief Move sprite and/or background based on key_poll() current keys and
 * postion of the sprite on the screen.
 *
 * @param id - Background to move if need be.
 */
INLINE void main_sprite_motion_buf(u32 id)
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

/**
 * @brief - Move element if in screen. Hide and don't move if not.
 *
 * @param sprite - Pointer to sprite struct.
 */
INLINE void move_element(SPRITE *sprite)
{
        // TODO:
        // (1) Get bg coordinates.
        BG_POINT bg = bg_offset_buf[0];
        // (2) Get sprite bg coordinates.
        s16 xsb = (s16)(sprite->bg_coord.x);
        s16 ysb = (s16)(sprite->bg_coord.y);
        // (3) Get sprite size.
        s16 size = 16;

        // (4) Perform comparision. If sprite is in screen, then unhide and
        // update screen coordinates.
        // TODO: Consider the case where the screen (xb, yb) = (0, 0) goes up
        // and left.
        OBJ_ATTR *attr = sprite->attr; // pointer to buffer
        u32       hidden = BF_GET(&attr->attr0, ATTR0_MODE);
        if (((xsb + size >= bg.x) || (xsb <= bg.x + SCREEN_WIDTH)) &&
            ((ysb + size >= bg.y) || (ysb <= bg.y + SCREEN_HEIGHT))) {
                // here we need to update the coordinate of the sprite.
                BF_SET(&attr->attr0, ysb - bg.y, ATTR0_Y);
                BF_SET(&attr->attr1, xsb - bg.x, ATTR1_X);
                if (hidden) {
                        BF_SET(&attr->attr0, 2, ATTR0_MODE);
                }
        } else if (!hidden) {
                BF_SET(&attr->attr0, 0, ATTR0_MODE);
        }
}

// Has to move everything that is not the main sprite, including berries and
// thorns. Should the every other sprite be put in a struct that contains
// berries and thorn arrays?
INLINE void move_elements(void)
{
        for (u32 i = 0; i < elements.b_len; i++) {
                move_element(&elements.berries[i]);
        }
        for (u32 i = 0; i < elements.t_len; i++) {
                move_element(&elements.thorns[i]);
        }
}
