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

#include <grrlib.h>
#include "grrlib/GRRLIB_ttf.h"
#include <wchar.h>

static FT_Library ftLibrary;        /**< A handle to a FreeType library instance. */

// Static function prototypes
static void DrawBitmap(FT_Bitmap *bitmap, int offset, int top, const u32 color);


/**
 * Initialize FreeType library.
 * @return int 0=OK; -1=Failed
 */
int GRRLIB_InitTTF () {
    if (FT_Init_FreeType(&ftLibrary)) {
        return -1;
    }
    return 0;
}

/**
 * Call this when your done with FreeType.
 */
void GRRLIB_ExitTTF (void) {
    FT_Done_FreeType(ftLibrary);
}

/**
 * Load a TTF from a buffer.
 * @param file_base Buffer with TTF data.
 * @param file_size Size of the TTF buffer.
 * @return A handle to a given TTF font object.
 * @see GRRLIB_FreeTTF
 */
GRRLIB_ttfFont* GRRLIB_LoadTTF (const u8* file_base, s32 file_size) {
    GRRLIB_ttfFont* myFont = (GRRLIB_ttfFont*)malloc(sizeof(GRRLIB_ttfFont));
    FT_New_Memory_Face(ftLibrary, file_base, file_size, 0, &myFont->face);
    myFont->kerning = FT_HAS_KERNING(myFont->face);
/*
    if (FT_Set_Pixel_Sizes(myFont->face, 0, fontSize)) {
        FT_Set_Pixel_Sizes(myFont->face, 0, 12);
    }
*/
    return myFont;
}

/**
 * Free memory allocated by TTF fonts.
 * @param myFont A TTF.
 */
void  GRRLIB_FreeTTF (GRRLIB_ttfFont *myFont) {
    if(myFont) {
        FT_Done_Face(myFont->face);
        free(myFont);
        myFont = NULL;
    }
}

/**
 * Print function for TTF font.
 * @param x Specifies the x-coordinate of the upper-left corner of the text.
 * @param y Specifies the y-coordinate of the upper-left corner of the text.
 * @param myFont A TTF.
 * @param string Text to draw.
 * @param fontSize Size of the font.
 * @param color Text color in RGBA format.
 */
void GRRLIB_PrintfTTF(int x, int y, GRRLIB_ttfFont *myFont, const char *string, unsigned int fontSize, const u32 color) {
    if(myFont == NULL || string == NULL)
        return;

    size_t length = strlen(string) + 1;
    wchar_t *utf32 = (wchar_t*)malloc(length * sizeof(wchar_t));
    if(utf32) {
        length = mbstowcs(utf32, string, length);
        if(length > 0) {
            utf32[length] = L'\0';
            GRRLIB_PrintfTTFW(x, y, myFont, utf32, fontSize, color);
        }
        free(utf32);
    }
}

/**
 * Print function for TTF font.
 * @author wplaat and DrTwox
 * @param x Specifies the x-coordinate of the upper-left corner of the text.
 * @param y Specifies the y-coordinate of the upper-left corner of the text.
 * @param myFont A TTF.
 * @param utf32 Text to draw.
 * @param fontSize Size of the font.
 * @param color Text color in RGBA format.
 */
void GRRLIB_PrintfTTFW(int x, int y, GRRLIB_ttfFont *myFont, const wchar_t *utf32, unsigned int fontSize, const u32 color) {
    if(myFont == NULL || utf32 == NULL)
        return;

    unsigned int loop;
    int penX = 0;
    int penY = fontSize;
    FT_GlyphSlot slot = myFont->face->glyph;
    FT_UInt glyphIndex = 0;
    FT_UInt previousGlyph = 0;

    if (FT_Set_Pixel_Sizes(myFont->face, 0, fontSize)) {
        FT_Set_Pixel_Sizes(myFont->face, 0, 12);
    }

    size_t length = wcslen(utf32);

    /* Loop over each character, until the
     * end of the string is reached, or until the pixel width is too wide */
    for (loop = 0; loop < length; ++loop) {
        glyphIndex = FT_Get_Char_Index(myFont->face, utf32[ loop ]);

        if (myFont->kerning && previousGlyph && glyphIndex) {
            FT_Vector delta;
            FT_Get_Kerning(myFont->face, previousGlyph, glyphIndex, FT_KERNING_DEFAULT, &delta);
            penX += delta.x >> 6;
        }
        if (FT_Load_Glyph(myFont->face, glyphIndex, FT_LOAD_RENDER)) {
            continue;
        }

        DrawBitmap(&slot->bitmap,
                   penX + slot->bitmap_left + x,
                   penY - slot->bitmap_top + y,
                   color);
        penX += slot->advance.x >> 6;
        previousGlyph = glyphIndex;
    }
}

/**
 * Draw a character on the screen.
 * @param bitmap Bitmap to draw.
 * @param offset x-coordinate offset.
 * @param top y-coordinate.
 * @param color character color in RGBA format.
 */
static void DrawBitmap(FT_Bitmap *bitmap, int offset, int top, const u32 color) {
    FT_Int i, j, p, q;
    FT_Int x_max = offset + bitmap->width;
    FT_Int y_max = top + bitmap->rows;
    u8 cR = R(color), cG = G(color), cB = B(color);

    for ( i = offset, p = 0; i < x_max; i++, p++ ) {
        for ( j = top, q = 0; j < y_max; j++, q++ ) {
            GX_Begin(GX_POINTS, GX_VTXFMT0, 1);
                GX_Position3f32(i, j, 0);
                GX_Color4u8(cR, cG, cB,
                            bitmap->buffer[ q * bitmap->width + p ]);
            GX_End();
        }
    }
}

/**
 * Get the width of a text in pixel.
 * @param myFont A TTF.
 * @param string The text to check.
 * @param fontSize The size of the font.
 * @return The width of a text in pixel.
 */
unsigned int GRRLIB_WidthTTF(GRRLIB_ttfFont *myFont, const char *string, unsigned int fontSize) {
    if(myFont == NULL || string == NULL) {
        return 0;
    }
    unsigned int penX;
    size_t length = strlen(string) + 1;
    wchar_t *utf32 = (wchar_t*)malloc(length * sizeof(wchar_t));
    length = mbstowcs(utf32, string, length);
    utf32[length] = L'\0';

    penX = GRRLIB_WidthTTFW(myFont, utf32, fontSize);

    free(utf32);

    return penX;
}

/**
 * Get the width of a text in pixel.
 * @param myFont A TTF.
 * @param utf32 The text to check.
 * @param fontSize The size of the font.
 * @return The width of a text in pixel.
 */
unsigned int GRRLIB_WidthTTFW(GRRLIB_ttfFont *myFont, const wchar_t *utf32, unsigned int fontSize) {
    if(myFont == NULL || utf32 == NULL) {
        return 0;
    }

    unsigned int loop;
    unsigned int penX = 0;
    FT_UInt glyphIndex;
    FT_UInt previousGlyph = 0;
    size_t length;

    if(FT_Set_Pixel_Sizes(myFont->face, 0, fontSize)) {
         FT_Set_Pixel_Sizes(myFont->face, 0, 12);
    }

    length = wcslen(utf32);

    for(loop = 0; loop < length; ++loop) {
        glyphIndex = FT_Get_Char_Index(myFont->face, utf32[ loop ]);

        if(myFont->kerning && previousGlyph && glyphIndex) {
            FT_Vector delta;
            FT_Get_Kerning(myFont->face, previousGlyph, glyphIndex, FT_KERNING_DEFAULT, &delta);
            penX += delta.x >> 6;
        }
        if(FT_Load_Glyph(myFont->face, glyphIndex, FT_LOAD_RENDER)) {
            continue;
        }

        penX += myFont->face->glyph->advance.x >> 6;
        previousGlyph = glyphIndex;
    }

    return penX;
}
