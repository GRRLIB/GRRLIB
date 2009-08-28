/*------------------------------------------------------------------------------
Copyright (c) 2009 The GRRLIB Team

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
#include <stdarg.h>
#include <stdio.h>

#include <grrlib.h>

/**
 * Print formatted output.
 * @param xpos Specifies the x-coordinate of the upper-left corner of the text.
 * @param ypos Specifies the y-coordinate of the upper-left corner of the text.
 * @param tex The texture containing the character set.
 * @param color Text color in RGBA format. The alpha channel is used to change the opacity of the text.
 * @param zoom This is a factor by which the text size will be increase or decrease.
 * @param text Text to draw.
 * @param ... Optional arguments.
 */
void  GRRLIB_Printf (const f32 xpos, const f32 ypos,
                     const GRRLIB_texImg *tex, const u32 color,
                     const f32 zoom, const char *text, ...) {
    if (tex == NULL || tex->data == NULL) {
        return;
    }

    int i, size;
    char tmp[1024];

    va_list argp;
    va_start(argp, text);
    size = vsprintf(tmp, text, argp);
    va_end(argp);

    for (i = 0; i < size; i++) {
        u8 c = tmp[i]-tex->tilestart;
        GRRLIB_DrawTile(xpos+i*tex->tilew*zoom, ypos, tex, 0, zoom, zoom, color, c);
    }
}

/**
 * Print formatted output with a ByteMap font.
 * @param xpos Specifies the x-coordinate of the upper-left corner of the text.
 * @param ypos Specifies the y-coordinate of the upper-left corner of the text.
 * @param bmf The ByteMap font to use.
 * @param zoom This is a factor by which the text size will be increase or decrease.
 * @param text Text to draw.
 * @param ... Optional arguments.
 */
void  GRRLIB_PrintBMF (const f32 xpos, const f32 ypos,
                       const GRRLIB_bytemapFont *bmf,
                       const f32 zoom, const char *text, ...) {
    uint  i, j, x, y, n, size;
    char  tmp[1024];
    f32   xoff = xpos;

    va_list argp;
    va_start(argp, text);
    size = vsprintf(tmp, text, argp);
    va_end(argp);

    GRRLIB_texImg *tex_BMfont = GRRLIB_CreateEmptyTexture(rmode->fbWidth, rmode->xfbHeight);

    for (i=0; i<size; i++) {
        for (j=0; j<bmf->nbChar; j++) {
            if (tmp[i] == bmf->charDef[j].character) {
                n=0;
                for (y=0; y<bmf->charDef[j].height; y++) {
                    for (x=0; x<bmf->charDef[j].width; x++) {
                        if (bmf->charDef[j].data[n]) {
                            GRRLIB_SetPixelTotexImg(xoff + x + bmf->charDef[j].relx, ypos + y + bmf->charDef[j].rely,
                                tex_BMfont, bmf->palette[bmf->charDef[j].data[n]]);
                        }
                        n++;
                    }
                }
                xoff += bmf->charDef[j].kerning + bmf->tracking;
                break;
            }
        }
    }

    GRRLIB_FlushTex( tex_BMfont );
    GRRLIB_DrawImg(0, 0, tex_BMfont, 0, 1, 1, 0xFFFFFFFF);
    free(tex_BMfont->data);
    free(tex_BMfont);
}
