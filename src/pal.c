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
