#ifndef REG_IO_H
#define REG_IO_H
#include "types.h"
#include "memdef.h"

// ---------------------------------------------------------------------------
//                       REG_DISPCNT
// ---------------------------------------------------------------------------
// Display Control
#define REG_DISPCNT      (*(vu16 *)(REG_BASE + 0x0000))

// Mode (0-2):
//    Sets video mode.
//    0, 1, 2 are tiled modes.
//    3, 4, 5 are bitmap modes.
#define DCNT_MODE_MASK   0x0003
#define DCNT_MODE_SHIFT  0
#define DCNT_MODE(n)     ((n) << DCNT_MODE_SHIFT)

#define DCNT_MODE0       DCNT_MODE(0)
#define DCNT_MODE1       DCNT_MODE(1)
#define DCNT_MODE2       DCNT_MODE(2)
#define DCNT_MODE3       DCNT_MODE(3)
#define DCNT_MODE4       DCNT_MODE(4)
#define DCNT_MODE5       DCNT_MODE(5)

// GB (3):
//    Is set if cartridge is a GBC game.
//    Read-only.
//    Must be 0 on GBA.
#define DCNT_GB          (1 << 3)

// PS (4):
//    Page select.
//    For bitmaps modes 4 and 5.
//    Selects the frame buffer page to display.
#define DCNT_PAGE        (1 << 4)

// HB (5):
//    Allows access to OAM in an HBlank.
//    OAM is normally locked in VDraw.
//    Will reduce the amount of sprite pixels rendered per line.
#define DCNT_OAM_HBL     (1 << 5)

// OM (6):
//    Object/Sprite mapping mode.
//    Tile memory can be seen as a 32x32 matrix of tiles.
//    Determines object (sprite) tile addressing.
//    When sprites are multiple tiles high:
//        OM == 0:
//            2D mapping.
//            Next row of tiles lies beneath the previous.
//        OM == 1:
//            1D mapping.
//            Next row of tiles lies right next to it.
#define DCNT_OBJ_1D      (1 << 6)

// FB (7):
//    Force a blank screen.
//    Display turned off.
//    Useful for updating video memory.
#define DCNT_BLANK       (1 << 7)

// BG0-BG3 (8-11):
//    Enable certain background rendering.
#define DCNT_LAYER_MASK  0x1F00
#define DCNT_LAYER_SHIFT 8
#define DCNT_LAYER(n)    ((n) << DCNT_LAYER_SHIFT)

#define DCNT_BG0         DCNT_LAYER(0x1)
#define DCNT_BG1         DCNT_LAYER(0x2)
#define DCNT_BG2         DCNT_LAYER(0x4)
#define DCNT_BG3         DCNT_LAYER(0x8)

// Obj (12):
//    Enable Object (sprite) rendering.
#define DCNT_OBJ         DCNT_LAYER(0x10)

// W0-W1 (13-14):
//    Enables Window0 or Window1 (rectangular display mask region).
#define DCNT_WIN0        (1 << 13)
#define DCNT_WIN1        (1 << 14)

// OW (15):
//    Enables the Object Window (mask region based on sprites).
#define DCNT_WINOBJ      (1 << 15)

// ---------------------------------------------------------------------------
//				REG_BGxCNT
// ---------------------------------------------------------------------------
// BG control register array: REG_BGCNT[x] is REG_BGxCNT
#define REG_BGCNT        ((vu16 *)(REG_BASE + 0x0008))

// Pr (0-1):
//    Priority.
//    Determines drawing order of backgrounds.
#define BG_PRIO_MASK     0x0003
#define BG_PRIO_SHIFT    0
#define BG_PRIO(n)       ((n) << 0)
#define BG_PRIO0         BG_PRIO(0)
#define BG_PRIO1         BG_PRIO(1)
#define BG_PRIO2         BG_PRIO(2)
#define BG_PRIO3         BG_PRIO(3)

// CBB (2-3):
//    Character Base Block.
//    Select charblock from which tilemap in SBB indexes into.
#define BG_CBB_MASK      0x000C
#define BG_CBB_SHIFT     2
#define BG_CBB(n)        ((n) << BG_CBB_SHIFT)
#define BG_CBB0          BG_CBB(0) // 0600:0000h - 0600:3FFFh (16 KB).
#define BG_CBB1          BG_CBB(1) // 0600:4000h - 0600:7FFFh (16 KB).
#define BG_CBB2          BG_CBB(2) // 0600:8000h - 0600:BFFFh (16 KB).
#define BG_CBB3          BG_CBB(3) // 0600:C000h - 0600:FFFFh (16 KB).

// Mos (6):
//    Mosaic Flag.
//    Enables mosaic effects.
#define BG_MOSAIC        (1 << 6)

// CM (7):
//    Color Mode.
//    If cleared, 16 colors (4bpp).
//    If set, 256 colors (8bpp).
#define BG_4BPP          0
#define BG_8BPP          (1 << 7)

// SBB (8-12):
//    Screen Base Block.
//    Starting screenblock that contains tilemap of tiles for background to
//    show. Total number of screenblocks depends on (Sz) bits. Values: 0-31.
#define BG_SBB_MASK      0x1F00
#define BG_SBB_SHIFT     8
#define BG_SBB(n)        (n << BG_SBB_SHIFT)

// Wr (13):
//    Affine Wrapping Flap.
//    If set, affine background wrap around at their edges.
//    Has no effect on regular backgrounds as they wrap around by default.
#define BG_WRAP          (1 << 13)

// Sz (14-15):
//    Background size and dimensions with screenblock units.
//    Screenblock = 32x32 tiles indices.
// Regular BG sizes:
//    Mode 0, 1, 2.
#define BG_SIZE_MASK     0xC000
#define BG_SIZE_SHIFT    14
#define BG_SIZE(n)       ((n) << BG_SIZE_SHIFT)
#define BG_SIZE_32x32    BG_SIZE(0) // 32x32 tiles (0)
#define BG_SIZE_64x32    BG_SIZE(1) // 64x32 tiles (0 | 1)
#define BG_SIZE_32x64    BG_SIZE(2) // 32x64 tiles (0 / 1)
#define BG_SIZE_64x64    BG_SIZE(3) // 64x64 tiles ((0 | 1) / (2 | 3) )
// Affine BG sizes:
//    Modes 1, 2.
#define BG_AFF_16x16     BG_SIZE(0) // 16x16 tiles
#define BG_AFF_32x32     BG_SIZE(1) // 32x32 tiles
#define BG_AFF_64x64     BG_SIZE(2) // 64x64 tiles
#define BG_AFF_128x128   BG_SIZE(3) // 128x128 tiles

#endif // REG_IO_H
