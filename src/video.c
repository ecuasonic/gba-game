#include "video.h"
#include "types.h"
#include <stddef.h> // For NULL

/**
 * @brief - Copy s-tiles or d-tiles from src to dest at ccb, starting tile.
 * .
 * @param cbb - Charblock to store tiles.
 * @param starting_tile - Tile in charblock where copying starts.
 * @param bitdepth - Either 4BPP or 8BPP.
 * @param src - Source tile array, must be word aligned.
 * @param nbytes - Number of bytes to copy.
 */
void load_tiles4(u32 cbb, u32 starting_tile, const TILE *src, u32 nbytes)
{
        if (src == NULL)
                return;
        if (cbb > 5)
                cbb = 5;

        // We could struct copy or word copy.
        // Here, we did word copy, but we could try the other one later.
        TILE *dest = &tile_mem[cbb][starting_tile];

        // Good since s-tiles == 8 words == 32 bits.
        u32 ntiles = nbytes / 32;
        while (ntiles--)
                *dest++ = *src++;
}

/**
 * @brief - Copy screenblock entries from src to dest at sbb.
 * Internally handles if last src element falls on word boundary (SRC_ENTRY == 2
 * bytes).
 * .
 * @param sbb - Screenblock to store screenblock entries.
 * @param src - Source tilemap, must be word aligned.
 * @param nbytes - Number of bytes to copy.
 */
void load_tilemap(u32 sbb, const u32 *src, u32 nbytes)
{
        if (src == NULL)
                return;
        if (sbb > 31)
                sbb = 31;

        u32 *dest = (u32 *)&se_mem[sbb][0];

        // Good since each screenblock is 2 bytes, so nbytes can either be
        // multiple of 2 or 4.
        if (nbytes & 0x2)
                nbytes += 2;
        u32 nwords = nbytes / 4;
        while (nwords--)
                *dest++ = *src++;
}
