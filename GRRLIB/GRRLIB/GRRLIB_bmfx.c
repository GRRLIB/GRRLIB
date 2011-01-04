/*------------------------------------------------------------------------------
Copyright (c) 2011 The GRRLIB Team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
------------------------------------------------------------------------------*/

#include <stdlib.h>

#include <grrlib.h>

/**
 * Flip texture horizontal.
 * @see GRRLIB_FlushTex
 * @param texsrc The texture source.
 * @param texdest The texture destination.
 */
void  GRRLIB_BMFX_FlipH (const GRRLIB_texImg *texsrc, GRRLIB_texImg *texdest) {
    unsigned int x, y, txtWidth = texsrc->w - 1;

    for (y = 0; y < texsrc->h; y++) {
        for (x = 0; x < texsrc->w; x++) {
            GRRLIB_SetPixelTotexImg(txtWidth - x, y, texdest,
                GRRLIB_GetPixelFromtexImg(x, y, texsrc));
        }
    }
}

/**
 * Flip texture vertical.
 * @see GRRLIB_FlushTex
 * @param texsrc The texture source.
 * @param texdest The texture destination.
 */
void  GRRLIB_BMFX_FlipV (const GRRLIB_texImg *texsrc, GRRLIB_texImg *texdest) {
    unsigned int x, y, texHeight = texsrc->h - 1;

    for (y = 0; y < texsrc->h; y++) {
        for (x = 0; x < texsrc->w; x++) {
            GRRLIB_SetPixelTotexImg(x, texHeight - y, texdest,
                GRRLIB_GetPixelFromtexImg(x, y, texsrc));
        }
    }
}

/**
 * Change a texture to gray scale.
 * @see GRRLIB_FlushTex
 * @param texsrc The texture source.
 * @param texdest The texture grayscaled destination.
 */
void  GRRLIB_BMFX_Grayscale (const GRRLIB_texImg *texsrc,
                             GRRLIB_texImg *texdest) {
    unsigned int x, y;
    u8 gray;
    u32 color;

    for (y = 0; y < texsrc->h; y++) {
        for (x = 0; x < texsrc->w; x++) {
            color = GRRLIB_GetPixelFromtexImg(x, y, texsrc);

            gray = ((R(color)* 77 +
                     G(color)*150 +
                     B(color)* 28 ) / 255);

            GRRLIB_SetPixelTotexImg(x, y, texdest,
                ((gray << 24) | (gray << 16) | (gray << 8) | A(color)));
        }
    }
    GRRLIB_SetHandle(texdest, 0, 0);
}

/**
 * Change a texture to sepia (old photo style).
 * @see GRRLIB_FlushTex
 * @param texsrc The texture source.
 * @param texdest The texture destination.
 * @author elisherer
 */
void  GRRLIB_BMFX_Sepia (const GRRLIB_texImg *texsrc, GRRLIB_texImg *texdest) {
    unsigned int  x, y;
    u16           sr, sg, sb;
    u32           color;

    for (y = 0; y < texsrc->h; y++) {
        for (x = 0; x < texsrc->w; x++) {
            color = GRRLIB_GetPixelFromtexImg(x, y, texsrc);
            sr = R(color)*0.393 + G(color)*0.769 + B(color)*0.189;
            sg = R(color)*0.349 + G(color)*0.686 + B(color)*0.168;
            sb = R(color)*0.272 + G(color)*0.534 + B(color)*0.131;
            if (sr>255) sr=255;  if (sg>255) sg=255;  if (sb>255) sb=255;
            GRRLIB_SetPixelTotexImg(x, y, texdest,
                                    RGBA(sr,sg,sb,A(color)));
        }
    }
    GRRLIB_SetHandle(texdest, 0, 0);
}
/**
 * Invert colors of the texture.
 * @see GRRLIB_FlushTex
 * @param texsrc The texture source.
 * @param texdest The texture destination.
 */
void  GRRLIB_BMFX_Invert (const GRRLIB_texImg *texsrc, GRRLIB_texImg *texdest) {
    unsigned int x, y;
    u32 color;

    for (y = 0; y < texsrc->h; y++) {
        for (x = 0; x < texsrc->w; x++) {
            color = GRRLIB_GetPixelFromtexImg(x, y, texsrc);
            GRRLIB_SetPixelTotexImg(x, y, texdest,
                ((0xFFFFFF - (color >> 8 & 0xFFFFFF)) << 8)  | (color & 0xFF));
        }
    }
}

/**
 * A texture effect (Blur).
 * @see GRRLIB_FlushTex
 * @param texsrc The texture source.
 * @param texdest The texture destination.
 * @param factor The blur factor.
 */
void  GRRLIB_BMFX_Blur (const GRRLIB_texImg *texsrc,
                              GRRLIB_texImg *texdest, const u32 factor) {
    int numba = (1+(factor<<1))*(1+(factor<<1));
    u32 x, y;
    s32 k, l;
    int tmp;
    int newr, newg, newb, newa;
    u32 colours[numba];
    u32 thiscol;

    for (x = 0; x < texsrc->w; x++) {
        for (y = 0; y < texsrc->h; y++) {
            newr = 0;
            newg = 0;
            newb = 0;
            newa = 0;

            tmp = 0;
            thiscol = GRRLIB_GetPixelFromtexImg(x, y, texsrc);

            for (k = x - factor; k <= x + factor; k++) {
                for (l = y - factor; l <= y + factor; l++) {
                    if (k < 0 || k >= texsrc->w || l < 0 || l >= texsrc->h) {
                        colours[tmp] = thiscol;
                    }
                    else {
                        colours[tmp] = GRRLIB_GetPixelFromtexImg(k, l, texsrc);
                    }
                    tmp++;
                }
            }

            for (tmp = 0; tmp < numba; tmp++) {
                newr += (colours[tmp] >> 24) & 0xFF;
                newg += (colours[tmp] >> 16) & 0xFF;
                newb += (colours[tmp] >> 8) & 0xFF;
                newa += colours[tmp] & 0xFF;
            }

            newr /= numba;
            newg /= numba;
            newb /= numba;
            newa /= numba;

            GRRLIB_SetPixelTotexImg(x, y, texdest, (newr<<24) | (newg<<16) | (newb<<8) | newa);
        }
    }
}

/**
 * A texture effect (Scatter).
 * @see GRRLIB_FlushTex
 * @param texsrc The texture source.
 * @param texdest The texture destination.
 * @param factor The factor level of the effect.
 */
void  GRRLIB_BMFX_Scatter (const GRRLIB_texImg *texsrc,
                                 GRRLIB_texImg *texdest, const u32 factor) {
    unsigned int x, y;
    u32 val1, val2;
    u32 val3, val4;
    int factorx2 = factor*2;

    for (y = 0; y < texsrc->h; y++) {
        for (x = 0; x < texsrc->w; x++) {
            val1 = x + (int) (factorx2 * (rand() / (RAND_MAX + 1.0))) - factor;
            val2 = y + (int) (factorx2 * (rand() / (RAND_MAX + 1.0))) - factor;

            if ((val1 >= texsrc->w) || (val2 >= texsrc->h)) {
            }
            else {
                val3 = GRRLIB_GetPixelFromtexImg(x, y, texsrc);
                val4 = GRRLIB_GetPixelFromtexImg(val1, val2, texsrc);
                GRRLIB_SetPixelTotexImg(x, y, texdest, val4);
                GRRLIB_SetPixelTotexImg(val1, val2, texdest, val3);
            }
        }
    }
}

/**
 * A texture effect (Pixelate).
 * @see GRRLIB_FlushTex
 * @param texsrc The texture source.
 * @param texdest The texture destination.
 * @param factor The factor level of the effect.
 */
void  GRRLIB_BMFX_Pixelate (const GRRLIB_texImg *texsrc,
                                  GRRLIB_texImg *texdest, const u32 factor) {
    unsigned int x, y;
    unsigned int xx, yy;
    u32 rgb;

    for (x = 0; x < texsrc->w - 1 - factor; x += factor) {
        for (y = 0; y < texsrc->h - 1 - factor; y +=factor) {
            rgb = GRRLIB_GetPixelFromtexImg(x, y, texsrc);
            for (xx = x; xx < x + factor; xx++) {
                for (yy = y; yy < y + factor; yy++) {
                    GRRLIB_SetPixelTotexImg(xx, yy, texdest, rgb);
                }
            }
        }
    }
}
