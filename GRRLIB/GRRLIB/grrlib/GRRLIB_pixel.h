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

/*
 * @file GRRLIB_pixel.h
 * Inline functions for manipulating pixels in textures.
 */

/**
 * Return the color value of a pixel from a GRRLIB_texImg.
 * @param x Specifies the x-coordinate of the pixel in the texture.
 * @param y Specifies the y-coordinate of the pixel in the texture.
 * @param tex The texture to get the color from.
 * @return The color of a pixel in RGBA format.
 */
INLINE
u32  GRRLIB_GetPixelFromtexImg (const int x, const int y,
                                const GRRLIB_texImg *tex) {
    register u32  offs;
    register u32  ar;
    register u8*  bp = (u8*)tex->data;

    offs = (((y&(~3))<<2)*tex->w) + ((x&(~3))<<4) + ((((y&3)<<2) + (x&3)) <<1);

    ar =                 (u32)(*((u16*)(bp+offs   )));
    return (ar<<24) | ( ((u32)(*((u16*)(bp+offs+32)))) <<8) | (ar>>8);  // Wii is big-endian
}

/**
 * Set the color value of a pixel to a GRRLIB_texImg.
 * @see GRRLIB_FlushTex
 * @param x Specifies the x-coordinate of the pixel in the texture.
 * @param y Specifies the y-coordinate of the pixel in the texture.
 * @param tex The texture to set the color to.
 * @param color The color of the pixel in RGBA format.
 */
INLINE
void  GRRLIB_SetPixelTotexImg (const int x, const int y,
                               GRRLIB_texImg *tex, const u32 color) {
    register u32  offs;
    register u8*  bp = (u8*)tex->data;

    offs = (((y&(~3))<<2)*tex->w) + ((x&(~3))<<4) + ((((y&3)<<2) + (x&3)) <<1);

    *((u16*)(bp+offs   )) = (u16)((color <<8) | (color >>24));
    *((u16*)(bp+offs+32)) = (u16) (color >>8);
}

/**
 * Reads a pixel directly from the FrontBuffer.
 * @param x The x-coordinate within the FB.
 * @param y The y-coordinate within the FB.
 */
INLINE
u32 GRRLIB_GetPixelFromFB (int x, int y) {
    GXColor peekColor;
    u32 MyColor;

    GX_PeekARGB(x, y, &peekColor);
    MyColor = RGBA(peekColor.r,peekColor.g,peekColor.b,peekColor.a);

    return (MyColor);
}

/**
 * Writes a pixel directly from the FrontBuffer.
 * @param x The x-coordinate within the FB.
 * @param y The y-coordinate within the FB.
 */
INLINE
void GRRLIB_SetPixelToFB (int x, int y, u32 pokeColor) {
    GXColor MyColor;

    MyColor.r=R(pokeColor);
    MyColor.g=G(pokeColor);
    MyColor.b=B(pokeColor);
    MyColor.a=A(pokeColor);

    GX_PokeARGB(x, y, MyColor);
}
