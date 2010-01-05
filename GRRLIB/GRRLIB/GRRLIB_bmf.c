/*------------------------------------------------------------------------------
Copyright (c) 2010 The GRRLIB Team

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

#include <malloc.h>
#include <string.h>

#include <grrlib.h>

/**
 * Load a ByteMap font structure from a buffer.
 * @param my_bmf The ByteMap font buffer to load.
 * @return A GRRLIB_bytemapFont structure filled with BMF information.
 * @see GRRLIB_FreeBMF
 */
GRRLIB_bytemapFont*  GRRLIB_LoadBMF (const u8 my_bmf[] ) {
    GRRLIB_bytemapFont *fontArray = (struct GRRLIB_bytemapFont *)malloc(sizeof(GRRLIB_bytemapFont));
    u32 i, j = 1;
    u8 lineheight, usedcolors, highestcolor, nbPalette;
    short int sizeover, sizeunder, sizeinner, numcolpal;
    u16 nbPixels;

    if (fontArray != NULL && my_bmf[0]==0xE1 && my_bmf[1]==0xE6 && my_bmf[2]==0xD5 && my_bmf[3]==0x1A) {
        fontArray->version = my_bmf[4];
        lineheight = my_bmf[5];
        sizeover = my_bmf[6];
        sizeunder = my_bmf[7];
        fontArray->tracking = my_bmf[8];
        sizeinner = my_bmf[9];
        usedcolors = my_bmf[10];
        highestcolor = my_bmf[11];
        nbPalette = my_bmf[16];
        numcolpal = 3 * nbPalette;
        fontArray->palette = (u32 *)calloc(nbPalette + 1, sizeof(u32));
        for (i=0; i < numcolpal; i+=3) {
            fontArray->palette[j++] = ((((my_bmf[i+17]<<2)+3)<<24) | (((my_bmf[i+18]<<2)+3)<<16) | (((my_bmf[i+19]<<2)+3)<<8) | 0xFF);
        }
        j = my_bmf[17 + numcolpal];
        fontArray->name = (char *)calloc(j + 1, sizeof(char));
        memcpy(fontArray->name, &my_bmf[18 + numcolpal], j);
        j = 18 + numcolpal + j;
        fontArray->nbChar = (my_bmf[j] | my_bmf[j+1]<<8);
        fontArray->charDef = (GRRLIB_bytemapChar *)calloc(fontArray->nbChar, sizeof(GRRLIB_bytemapChar));
        j++;
        for (i=0; i < fontArray->nbChar; i++) {
            fontArray->charDef[i].character = my_bmf[++j];
            fontArray->charDef[i].width = my_bmf[++j];
            fontArray->charDef[i].height = my_bmf[++j];
            fontArray->charDef[i].relx = my_bmf[++j];
            fontArray->charDef[i].rely = my_bmf[++j];
            fontArray->charDef[i].kerning = my_bmf[++j];
            nbPixels = fontArray->charDef[i].width * fontArray->charDef[i].height;
            fontArray->charDef[i].data = (u8 *)malloc(nbPixels);
            if (nbPixels && fontArray->charDef[i].data) {
                memcpy(fontArray->charDef[i].data, &my_bmf[++j], nbPixels);
                j += (nbPixels - 1);
            }
        }
    }
    return fontArray;
}

/**
 * Free memory allocated by ByteMap fonts.
 * @param bmf A GRRLIB_bytemapFont structure.
 */
void  GRRLIB_FreeBMF (const GRRLIB_bytemapFont *bmf) {
    u16 i;

    for (i=0; i<bmf->nbChar; i++) {
        free(bmf->charDef[i].data);
    }
    free(bmf->charDef);
    free(bmf->palette);
    free(bmf->name);
}

/**
 * Initialize a tile set.
 * @param tex The texture to initialize.
 * @param tilew Width of the tile.
 * @param tileh Height of the tile.
 * @param tilestart Offset for starting position (Used in fonts).
 */
void  GRRLIB_InitTileSet (GRRLIB_texImg *tex,
                          const uint tilew, const uint tileh,
                          const uint tilestart) {
    tex->tilew = tilew;
    tex->tileh = tileh;
    if (tilew) // Avoid division by zero
        tex->nbtilew = tex->w / tilew;
    if (tileh) // Avoid division by zero
        tex->nbtileh = tex->h / tileh;
    tex->tilestart = tilestart;
    tex->tiledtex = true;
    tex->ofnormaltexx = 1.0F / tex->nbtilew;
    tex->ofnormaltexy = 1.0F / tex->nbtileh;
    GRRLIB_SetHandle( tex, 0, 0 );
}
