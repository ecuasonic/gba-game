#include "pal.h"
#include "types.h"
#include <stddef.h> // For NULL

/**
 * @brief - Copies src palette into dest (either sprite or bg palette).
 *
 * @param dest - SPRITE_PAL or BG_PAL.
 * @param src - Pointer to source palette, must be word aligned.
 * @param nbytes - Number of bytes to copy.
 */
void load_pal(u32 *dest, const u32 *src, u32 nbytes)
{
        if (dest == NULL || src == NULL)
                return;

        if (nbytes & 0x02)
                nbytes += 2;
        u32 words = nbytes / 4;
        while (words--)
                // word copy for ARM block-copy instructions.
                *dest++ = *src++;
}

/**
 * @brief - Dim palette.
 *
 * @param pal - Pointer to palette.
 * @param nbytes Number of bytes in palette.
 * @param dim - How much to subtract from each current 31-bit color.
 */
void dim_palette(u16 *pal, u32 nbytes, u32 dim)
{
        u32 npal = nbytes / 2;
        while (npal--) {
                DIM(pal++, dim);
        }
}
