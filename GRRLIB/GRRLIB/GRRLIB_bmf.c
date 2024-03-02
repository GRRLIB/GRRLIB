/*------------------------------------------------------------------------------
Copyright (c) 2009-2022 The GRRLIB Team

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
 * File format version 1.1 is used, more information could be found at https://bmf.php5.cz/?page=format
 * @param my_bmf The ByteMap font buffer to load.
 * @return A GRRLIB_bytemapFont structure filled with BMF information.
 * @see GRRLIB_FreeBMF
 */
GRRLIB_bytemapFont*  GRRLIB_LoadBMF (const u8 my_bmf[]) {
    GRRLIB_bytemapFont *fontArray = (struct GRRLIB_bytemapFont *)malloc(sizeof(GRRLIB_bytemapFont));

    if (fontArray != NULL && my_bmf[0]==0xE1 && my_bmf[1]==0xE6 && my_bmf[2]==0xD5 && my_bmf[3]==0x1A) {
        u32 j = 1;
        fontArray->version = my_bmf[4];
        //u8 lineheight = my_bmf[5];
        //short int sizeover = my_bmf[6];
        //short int sizeunder = my_bmf[7];
        fontArray->tracking = my_bmf[8];
        //short int sizeinner = my_bmf[9];
        //u8 usedcolors = my_bmf[10];
        //u8 highestcolor = my_bmf[11];
        const u8 nbPalette = my_bmf[16];
        const short int numcolpal = 3 * nbPalette;
        fontArray->palette = (u32 *)calloc(nbPalette + 1, sizeof(u32));
        for (u32 i=0; i < numcolpal; i+=3) {
            fontArray->palette[j++] = ((((my_bmf[i+17]<<2)+3)<<24) | (((my_bmf[i+18]<<2)+3)<<16) | (((my_bmf[i+19]<<2)+3)<<8) | 0xFF);
        }
        j = my_bmf[17 + numcolpal];
        fontArray->name = (char *)calloc(j + 1, sizeof(char));
        memcpy(fontArray->name, &my_bmf[18 + numcolpal], j);
        j = 18 + numcolpal + j;
        fontArray->nbChar = (my_bmf[j] | my_bmf[j+1]<<8);
        memset(fontArray->charDef, 0, 256 * sizeof(GRRLIB_bytemapChar));
        j++;
        for (u32 i=0; i < fontArray->nbChar; i++) {
            const u8 c = my_bmf[++j];
            fontArray->charDef[c].width = my_bmf[++j];
            fontArray->charDef[c].height = my_bmf[++j];
            fontArray->charDef[c].relx = my_bmf[++j];
            fontArray->charDef[c].rely = my_bmf[++j];
            fontArray->charDef[c].kerning = my_bmf[++j];
            const u16 nbPixels = fontArray->charDef[c].width * fontArray->charDef[c].height;
            fontArray->charDef[c].data = (u8 *)malloc(nbPixels);
            if (nbPixels > 0 && fontArray->charDef[c].data != NULL) {
                memcpy(fontArray->charDef[c].data, &my_bmf[++j], nbPixels);
                j += (nbPixels - 1);
            }
        }
    }
    return fontArray;
}

/**
 * Free memory allocated by ByteMap fonts.
 * If \a bmf is a null pointer, the function does nothing.
 * @note This function does not change the value of \a bmf itself, hence it still points to the same (now invalid) location.
 * @param bmf A GRRLIB_bytemapFont structure.
 */
void  GRRLIB_FreeBMF (GRRLIB_bytemapFont *bmf) {
    if (bmf == NULL) {
        return;
    }

    for (u16 i = 0; i < 256; i++) {
        if (bmf->charDef[i].data != NULL) {
            free(bmf->charDef[i].data);
        }
    }
    free(bmf->palette);
    free(bmf->name);
    free(bmf);
}

/**
 * Initialize a tile set.
 * @param tex The texture to initialize.
 * @param tilew Width of the tile.
 * @param tileh Height of the tile.
 * @param tilestart Offset for starting position (Used in fonts).
 */
void  GRRLIB_InitTileSet (GRRLIB_texImg *tex,
                          const u32 tilew, const u32 tileh,
                          const u32 tilestart) {
    tex->tilew = tilew;
    tex->tileh = tileh;
    if (tilew != 0) { // Avoid division by zero
        tex->nbtilew = tex->w / tilew;
    }
    if (tileh != 0) { // Avoid division by zero
        tex->nbtileh = tex->h / tileh;
    }
    tex->tilestart = tilestart;
    tex->tiledtex = true;
    tex->ofnormaltexx = 1.0f / tex->nbtilew;
    tex->ofnormaltexy = 1.0f / tex->nbtileh;
    GRRLIB_SetHandle( tex, 0, 0 );
}
