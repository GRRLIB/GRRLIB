/*------------------------------------------------------------------------------
Copyright (c) 2009-2024 The GRRLIB Team

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
#include <wchar.h>
#include <ft2build.h>
#include FT_FREETYPE_H

static FT_Library ftLibrary; /**< A handle to a FreeType library instance. */

// Static function prototypes
static void DrawBitmap(FT_Bitmap *bitmap, int offset, int top, const u8 cR, const u8 cG, const u8 cB, const u8 cA);


/**
 * Initialize FreeType library.
 * @return int 0=OK; -1=Failed
 */
int GRRLIB_InitTTF (void) {
    if (FT_Init_FreeType(&ftLibrary) != 0) {
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
 * @param file_base Buffer with TTF data. You must not deallocate the memory before calling GRRLIB_FreeTTF.
 * @param file_size Size of the TTF buffer.
 * @return A handle to a given TTF font object or NULL if it fails to load the font.
 * @see GRRLIB_FreeTTF
 */
GRRLIB_ttfFont* GRRLIB_LoadTTF (const u8* file_base, s32 file_size) {
    FT_Face Face;
    if (FT_New_Memory_Face(ftLibrary, file_base, file_size, 0, &Face) != 0) {
        return NULL;
    }
    GRRLIB_ttfFont* myFont = (GRRLIB_ttfFont*)malloc(sizeof(GRRLIB_ttfFont));
    myFont->kerning = FT_HAS_KERNING(Face);
/*
    if (FT_Set_Pixel_Sizes(Face, 0, fontSize) != 0) {
        FT_Set_Pixel_Sizes(Face, 0, 12);
    }
*/
    myFont->face = Face;
    return myFont;
}

/**
 * Free memory allocated by TTF fonts.
 * If \a myFont is a null pointer, the function does nothing.
 * @note This function does not change the value of \a myFont itself, hence it still points to the same (now invalid) location.
 * @param myFont A TTF.
 */
void  GRRLIB_FreeTTF (GRRLIB_ttfFont *myFont) {
    if (myFont != NULL) {
        FT_Done_Face(myFont->face);
        free(myFont);
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
    if (myFont == NULL || string == NULL) {
        return;
    }

    FT_Face Face = (FT_Face)myFont->face;
    int penX = 0;
    int penY = fontSize;
    FT_GlyphSlot slot = Face->glyph;
    FT_UInt previousGlyph = 0;
    const u8 cR = R(color);
    const u8 cG = G(color);
    const u8 cB = B(color);
    const u8 cA = A(color);

    if (FT_Set_Pixel_Sizes(Face, 0, fontSize) != 0) {
        FT_Set_Pixel_Sizes(Face, 0, 12);
    }

    /* Loop over each character, until the
     * end of the string is reached, or until the pixel width is too wide */
    while(*string) {
        wchar_t wchar;
        if (((*(string + 0)) & 0x80) == 0x00) {
            // Single-byte character (ASCII)
            wchar = (wchar_t)*string;
            string += 1;
        }
        else if (((*(string + 0)) & 0xE0) == 0xC0 && ((*(string + 1)) & 0xC0) == 0x80) {
            // Two-byte character
            wchar = ((wchar_t)(*string & 0x1F) << 6) | ((wchar_t)(*(string + 1)) & 0x3F);
            string += 2;
        }
        else if (((*(string + 0)) & 0xF0) == 0xE0 && ((*(string + 1)) & 0xC0) == 0x80 && ((*(string + 2)) & 0xC0) == 0x80) {
            // Three-byte character
            wchar = ((wchar_t)(*string & 0x0F) << 12) | ((wchar_t)(*(string + 1) & 0x3F) << 6) | ((wchar_t)(*(string + 2)) & 0x3F);
            string += 3;
        }
        else if (((*(string + 0)) & 0xF8) == 0xF0 && ((*(string + 1)) & 0xC0) == 0x80 && ((*(string + 2)) & 0xC0) == 0x80 && ((*(string + 3)) & 0xC0) == 0x80) {
            // Four-byte character
            wchar = ((wchar_t)(*string & 0x07) << 18) | ((wchar_t)(*(string + 1) & 0x3F) << 12) | ((wchar_t)(*(string + 2) & 0x3F) << 6) | ((wchar_t)(*(string + 3)) & 0x3F);
            string += 4;
        }
        else {
            // Invalid UTF-8 sequence
            string++;
            continue;
        }

        const FT_UInt glyphIndex = FT_Get_Char_Index(myFont->face, wchar);

        if (myFont->kerning && previousGlyph && glyphIndex) {
            FT_Vector delta;
            FT_Get_Kerning(myFont->face, previousGlyph, glyphIndex, FT_KERNING_DEFAULT, &delta);
            penX += delta.x >> 6;
        }
        if (FT_Load_Glyph(myFont->face, glyphIndex, FT_LOAD_RENDER) != 0) {
            continue;
        }

        DrawBitmap(&slot->bitmap,
                penX + slot->bitmap_left + x,
                penY - slot->bitmap_top + y,
                cR, cG, cB, cA);
        penX += slot->advance.x >> 6;
        previousGlyph = glyphIndex;
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
    if (myFont == NULL || utf32 == NULL) {
        return;
    }

    FT_Face Face = (FT_Face)myFont->face;
    int penX = 0;
    int penY = fontSize;
    FT_GlyphSlot slot = Face->glyph;
    FT_UInt previousGlyph = 0;
    const u8 cR = R(color);
    const u8 cG = G(color);
    const u8 cB = B(color);
    const u8 cA = A(color);

    if (FT_Set_Pixel_Sizes(Face, 0, fontSize) != 0) {
        FT_Set_Pixel_Sizes(Face, 0, 12);
    }

    /* Loop over each character, until the
     * end of the string is reached, or until the pixel width is too wide */
    while(*utf32) {
        const FT_UInt glyphIndex = FT_Get_Char_Index(myFont->face, *utf32++);

        if (myFont->kerning && previousGlyph && glyphIndex) {
            FT_Vector delta;
            FT_Get_Kerning(myFont->face, previousGlyph, glyphIndex, FT_KERNING_DEFAULT, &delta);
            penX += delta.x >> 6;
        }
        if (FT_Load_Glyph(myFont->face, glyphIndex, FT_LOAD_RENDER) != 0) {
            continue;
        }

        DrawBitmap(&slot->bitmap,
                   penX + slot->bitmap_left + x,
                   penY - slot->bitmap_top + y,
                   cR, cG, cB, cA);
        penX += slot->advance.x >> 6;
        previousGlyph = glyphIndex;
    }
}

/**
 * Draw a character on the screen.
 * @param bitmap Bitmap to draw.
 * @param offset x-coordinate offset.
 * @param top y-coordinate.
 * @param cR Red component of the colour.
 * @param cG Green component of the colour.
 * @param cB Blue component of the colour.
 * @param cA Alpha component of the colour.
 */
static void DrawBitmap(FT_Bitmap *bitmap, int offset, int top, const u8 cR, const u8 cG, const u8 cB, const u8 cA) {
    const FT_Int x_max = offset + bitmap->width;
    const FT_Int y_max = top + bitmap->rows;

    for (FT_Int i = offset, p = 0; i < x_max; i++, p++ ) {
        for (FT_Int j = top, q = 0; j < y_max; j++, q++ ) {
            s16 alpha = bitmap->buffer[ q * bitmap->width + p ] - (0xFF - cA);
            if(alpha < 0) {
                alpha = 0;
            }
            GX_Begin(GX_POINTS, GX_VTXFMT0, 1);
                GX_Position3f32(i, j, 0);
                GX_Color4u8(cR, cG, cB, alpha);
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
u32 GRRLIB_WidthTTF(GRRLIB_ttfFont *myFont, const char *string, unsigned int fontSize) {
    if (myFont == NULL || string == NULL) {
        return 0;
    }

    FT_Face Face = (FT_Face)myFont->face;
    u32 penX = 0;
    FT_UInt previousGlyph = 0;

    if (FT_Set_Pixel_Sizes(myFont->face, 0, fontSize) != 0) {
         FT_Set_Pixel_Sizes(myFont->face, 0, 12);
    }

    while(*string) {
        wchar_t wchar;
        if (((*(string + 0)) & 0x80) == 0x00) {
            // Single-byte character (ASCII)
            wchar = (wchar_t)*string;
            string += 1;
        }
        else if (((*(string + 0)) & 0xE0) == 0xC0 && ((*(string + 1)) & 0xC0) == 0x80) {
            // Two-byte character
            wchar = ((wchar_t)(*string & 0x1F) << 6) | ((wchar_t)(*(string + 1)) & 0x3F);
            string += 2;
        }
        else if (((*(string + 0)) & 0xF0) == 0xE0 && ((*(string + 1)) & 0xC0) == 0x80 && ((*(string + 2)) & 0xC0) == 0x80) {
            // Three-byte character
            wchar = ((wchar_t)(*string & 0x0F) << 12) | ((wchar_t)(*(string + 1) & 0x3F) << 6) | ((wchar_t)(*(string + 2)) & 0x3F);
            string += 3;
        }
        else if (((*(string + 0)) & 0xF8) == 0xF0 && ((*(string + 1)) & 0xC0) == 0x80 && ((*(string + 2)) & 0xC0) == 0x80 && ((*(string + 3)) & 0xC0) == 0x80) {
            // Four-byte character
            wchar = ((wchar_t)(*string & 0x07) << 18) | ((wchar_t)(*(string + 1) & 0x3F) << 12) | ((wchar_t)(*(string + 2) & 0x3F) << 6) | ((wchar_t)(*(string + 3)) & 0x3F);
            string += 4;
        }
        else {
            // Invalid UTF-8 sequence
            string++;
            continue;
        }

        const FT_UInt glyphIndex = FT_Get_Char_Index(myFont->face, wchar);

        if (myFont->kerning && previousGlyph && glyphIndex) {
            FT_Vector delta;
            FT_Get_Kerning(Face, previousGlyph, glyphIndex, FT_KERNING_DEFAULT, &delta);
            penX += delta.x >> 6;
        }
        if (FT_Load_Glyph(Face, glyphIndex, FT_LOAD_RENDER) != 0) {
            continue;
        }

        penX += Face->glyph->advance.x >> 6;
        previousGlyph = glyphIndex;
    }

    return penX;
}

/**
 * Get the width of a text in pixel.
 * @param myFont A TTF.
 * @param utf32 The text to check.
 * @param fontSize The size of the font.
 * @return The width of a text in pixel.
 */
u32 GRRLIB_WidthTTFW(GRRLIB_ttfFont *myFont, const wchar_t *utf32, unsigned int fontSize) {
    if (myFont == NULL || utf32 == NULL) {
        return 0;
    }

    FT_Face Face = (FT_Face)myFont->face;
    u32 penX = 0;
    FT_UInt glyphIndex;
    FT_UInt previousGlyph = 0;

    if (FT_Set_Pixel_Sizes(myFont->face, 0, fontSize) != 0) {
         FT_Set_Pixel_Sizes(myFont->face, 0, 12);
    }

    while(*utf32) {
        glyphIndex = FT_Get_Char_Index(myFont->face, *utf32++);

        if (myFont->kerning && previousGlyph && glyphIndex) {
            FT_Vector delta;
            FT_Get_Kerning(Face, previousGlyph, glyphIndex, FT_KERNING_DEFAULT, &delta);
            penX += delta.x >> 6;
        }
        if (FT_Load_Glyph(Face, glyphIndex, FT_LOAD_RENDER) != 0) {
            continue;
        }

        penX += Face->glyph->advance.x >> 6;
        previousGlyph = glyphIndex;
    }

    return penX;
}
