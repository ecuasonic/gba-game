#ifndef VIDEO_H
#define VIDEO_H

#include "types.h"
#include "memdef.h"
#include "memreg.h"

#define vid_mem ((vu16 *)MEM_VRAM)

/**
 * @brief Buffer for BG offset register.
 * Used for processing, then copying into actual buffer (update_bg_offset()).
 */
extern BG_POINT bg_offset_buf[4];

/**
 * @brief Copy BG offset register buffer into actual register.
 *
 * @param id Background to update BG offset register.
 */
INLINE void update_bg_offset(u32 id) { REG_BG_OFS[id] = bg_offset_buf[id]; }

// ---------------------------------------------------------------------------
//                       VBLANK
// ---------------------------------------------------------------------------
#define REG_VCOUNT (*(vu16 *)(REG_BASE + 0x0006))

/**
 * @brief Wait until the start of the next VBlank.
 */
void vid_vsync(void);

// =======================================================
//			CHARBLOCKS
// =======================================================
// Charblock is an array of tile structs.
// 4 background charblocks.
// 2 sprite charblocks.
// 16KB each.

// 8x8 s-tile - 4bpp
// 32 bytes.
typedef struct {
        u32 data[8];
} TILE, TILE4;

// 8x8 d-tile - 8bpp
// 64 bytes.
typedef struct {
        u32 data[16];
} TILE8;

// Charblock - 16 KB
typedef TILE  CHARBLOCK[512];
typedef TILE8 CHARBLOCK8[256];

#define tile_mem  ((CHARBLOCK *)MEM_VRAM)
#define tile8_mem ((CHARBLOCK8 *)MEM_VRAM)

// Copy a tile from data to sprite-mem, tile 12.
// tile_mem[4][12] = *(TILE *)spriteData;

void load_tiles4(u32 cbb, u32 starting_tile, const TILE *src, u32 bytes);
void load_tiles8(u32 cbb, u32 starting_tile, const TILE8 *src, u32 nbytes);

// =======================================================
//			SCREENBLOCKS
// =======================================================
// Screenblock is an array of tile indices (screenblock entries)
// 32 Screenblocks.
// 2 KB each.

// Screenblock entry.
typedef u16 SCR_ENTRY;

// Screenblock struct.
typedef SCR_ENTRY SCREENBLOCK[1024];

// Screen-entry mapping: se_mem[SSB][SE] == SCR_ENTRY
#define se_mem ((SCREENBLOCK *)MEM_VRAM)

void load_tilemap(u32 sbb, const u32 *tilemap, u32 nse, u32 times);

// =======================================================
//			SCREENBLOCK ENTRY
// =======================================================
// Stores tile index, tile flipping flags, and palbank for tile.
// 2 bytes each.

// TID (0-9):
//    Tile-index of the SE.
//    1024 tile indices.
//    If s-tiles (4bpp):
//        512 tiles/charblock.
//        2 charblocks available.
//    If d-tiles (8bpp):
//        256 tiles/charblock.
//        4 charblocks available.
//    If indexed tile is in ovram (charblocks 4,5), nothing is shown.
#define SE_ID_MASK       0x03FF
#define SE_ID_SHIFT      0
#define SE_ID(n)         ((n) << SE_ID_SHIFT)

// HF (10):
//    Horizontal flags.
// VF (11):
//    Vertical flags.
#define SE_FLIP_MASK     0x0400
#define SE_FLIP_SHIFT    10
#define SE_FLIP(n)       ((n) << SE_FLIP_SHIFT)
#define SE_HFLIP         SE_FLIP(1)
#define SE_VFLIP         SE_FLIP(2)
#define SE_HVFLIP        SE_FLIP(3)

// PB (12-15):
//    Palette bank to use when in 16-color mode.
//    Has no effect for 256-color bgs (REG_BGxCNT{6} is set).
#define SE_PALBANK_MASK  0xF000
#define SE_PALBANK_SHIFT 12
#define SE_PALBANK(n)    ((n) << PALBANK_SHIFT)

#endif
