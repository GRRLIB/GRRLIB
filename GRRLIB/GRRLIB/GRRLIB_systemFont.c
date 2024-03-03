/*------------------------------------------------------------------------------
Copyright (c) 2009-2026 The GRRLIB Team

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

#include "grrlib/GRRLIB_private.h"
#include <grrlib.h>

// Static function prototypes
static void GRRLIB_PrintfCharSystemFont(f32 x, f32 y, u32 color, s32 texX, s32 texY, unsigned int fontSize, void *image);

extern GRRLIB_drawSettings GRRLIB_Settings;

static sys_fontheader *fontdata = NULL;
static GXTexObj fonttex;

/**
 * Initialize the system font.
 * @return 0 on success, or -1 if the font could not be initialized.
 */
int GRRLIB_InitSystemFont (void) {
    if (fontdata != NULL) {
        return 0;
    }

    switch(SYS_GetFontEncoding())
    {
        case 0:
            fontdata = memalign(32, SYS_FONTSIZE_ANSI);
            break;
        case 1:
            fontdata = memalign(32, SYS_FONTSIZE_SJIS);
            break;
        default:
            return -1;
    }
    if (fontdata == NULL) {
        return -1;
    }

    if (SYS_InitFont(fontdata) == 0) {
        free(fontdata);
        fontdata = NULL;
        return -1;
    }

    return 0;
}

/**
 * Free the memory allocated for the system font.
 */
void GRRLIB_ExitSystemFont (void) {
    if (fontdata != NULL) {
        free(fontdata);
        fontdata = NULL;
    }
}

/**
 * Draw a single character using the system font.
 * @param x The x-coordinate of the character's position.
 * @param y The y-coordinate of the character's position.
 * @param color The color of the character in RGBA format.
 * @param texX The x-coordinate of the character in the font texture.
 * @param texY The y-coordinate of the character in the font texture.
 * @param fontSize The size of the character in pixels.
 * @param image Pointer to the font texture image.
 */
static void GRRLIB_PrintfCharSystemFont (f32 x, f32 y, u32 color, s32 texX, s32 texY, unsigned int fontSize, void *image) {
    const f32 x2 = x + (f32)fontdata->cell_width * fontSize / fontdata->cell_height;
    const f32 y2 = y + fontSize;
    const f32 s1 = (f32)texX / fontdata->sheet_width;
    const f32 t1 = (f32)texY / fontdata->sheet_height;
    const f32 s2 = s1 + (f32)fontdata->cell_width / fontdata->sheet_width;
    const f32 t2 = t1 + (f32)fontdata->cell_height / fontdata->sheet_height;

    GX_InitTexObj(&fonttex, image,
                  fontdata->sheet_width, fontdata->sheet_height,
                  fontdata->sheet_format, GX_CLAMP, GX_CLAMP, GX_FALSE);
    if (GRRLIB_Settings.antialias == false) {
        GX_InitTexObjLOD(&fonttex, GX_NEAR, GX_NEAR,
                         0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
        GX_SetCopyFilter(GX_FALSE, rmode->sample_pattern, GX_FALSE, rmode->vfilter);
    }
    else {
        GX_SetCopyFilter(rmode->aa, rmode->sample_pattern, GX_TRUE, rmode->vfilter);
    }

    GX_LoadTexObj(&fonttex,     GX_TEXMAP0);
    GX_SetTevOp  (GX_TEVSTAGE0, GX_MODULATE);
    GX_SetVtxDesc(GX_VA_TEX0,   GX_DIRECT);

    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
        GX_Position3f32(x, y, 0.0f);
        GX_Color1u32(color);
        GX_TexCoord2f32(s1, t1);

        GX_Position3f32(x2, y, 0.0f);
        GX_Color1u32(color);
        GX_TexCoord2f32(s2, t1);

        GX_Position3f32(x2, y2, 0.0f);
        GX_Color1u32(color);
        GX_TexCoord2f32(s2, t2);

        GX_Position3f32(x, y2, 0.0f);
        GX_Color1u32(color);
        GX_TexCoord2f32(s1, t2);
    GX_End();

    GX_SetTevOp  (GX_TEVSTAGE0, GX_PASSCLR);
    GX_SetVtxDesc(GX_VA_TEX0,   GX_NONE);
}

/**
 * Print text using the system font.
 * @param x Specifies the x-coordinate of the upper-left corner of the text.
 * @param y Specifies the y-coordinate of the upper-left corner of the text.
 * @param string Text to draw.
 * @param fontSize Height of the text in pixels.
 * @param color Text color in RGBA format.
 */
void GRRLIB_PrintfSystemFont (int x, int y, const char *string, unsigned int fontSize, const u32 color) {
    if (string == NULL || fontSize == 0 || fontdata == NULL) {
        return;
    }

    f32 penX = x;
    for (const char* c = string; *c != '\0'; c++) {
        void *image;
        s32 texX, texY, width;
        SYS_GetFontTexture(*c, &image, &texX, &texY, &width);
        if (image != NULL) {
            GRRLIB_PrintfCharSystemFont(penX, y, color, texX, texY, fontSize, image);
            penX += (f32)width * fontSize / fontdata->cell_height;
        }
    }
}
