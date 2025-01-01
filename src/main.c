#include "types.h"
#include "memreg.h"
#include "pal.h"
#include "video.h"
#include "menu_screen.h"

#define CBB0  0
#define TILE0 0
#define SBB30 30

int NORETURN main(void)
{
        REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ;
        REG_BGCNT[0] = BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_SIZE_32x32;

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

        while (1)
                ;
}
