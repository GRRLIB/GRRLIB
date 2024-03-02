/*------------------------------------------------------------------------------
Copyright (c) 2009-2023 The GRRLIB Team

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

    char tmp[1024];
    const f32 offset = tex->tilew * zoom;

    va_list argp;
    va_start(argp, text);
    const int size = vsnprintf(tmp, sizeof(tmp), text, argp);
    va_end(argp);

    for (int i = 0; i < size; i++) {
        GRRLIB_DrawTile(xpos+i*offset, ypos, tex, 0, zoom, zoom, color,
            tmp[i] - tex->tilestart);
    }
}

/**
 * Print formatted output with a ByteMap font.
 * This function could be slow, it should be used with GRRLIB_CompoStart and GRRLIB_CompoEnd.
 * @param xpos Specifies the x-coordinate of the upper-left corner of the text.
 * @param ypos Specifies the y-coordinate of the upper-left corner of the text.
 * @param bmf The ByteMap font to use.
 * @param text Text to draw.
 * @param ... Optional arguments.
 */
void  GRRLIB_PrintBMF (const f32 xpos, const f32 ypos,
                       const GRRLIB_bytemapFont *bmf,
                       const char *text, ...) {
    char  tmp[1024];
    f32   xoff = xpos;
    const GRRLIB_bytemapChar *pchar;

    va_list argp;
    va_start(argp, text);
    const u32 size = vsnprintf(tmp, sizeof(tmp), text, argp);
    va_end(argp);

    for (u32 i = 0; i < size; i++) {
        pchar = &bmf->charDef[(u8)tmp[i]];
        u8 *pdata = pchar->data;
        for (u8 y = 0; y<pchar->height; y++) {
            for (u8 x = 0; x<pchar->width; x++) {
                if (*pdata) {
                    GRRLIB_Plot(xoff + x + pchar->relx,
                                ypos + y + pchar->rely,
                                bmf->palette[*pdata]);
                }
                pdata++;
            }
        }
        xoff += pchar->kerning + bmf->tracking;
    }
}
