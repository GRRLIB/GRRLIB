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
    u32  offs;
    u32  ar;
    u8*  bp = (u8*)tex->data;

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
    u32  offs;
    u8*  bp = (u8*)tex->data;

    offs = (((y&(~3))<<2)*tex->w) + ((x&(~3))<<4) + ((((y&3)<<2) + (x&3)) <<1);

    *((u16*)(bp+offs   )) = (u16)((color <<8) | (color >>24));
    *((u16*)(bp+offs+32)) = (u16) (color >>8);
}

/**
 * Reads a pixel directly from the FrontBuffer.
 * @param x The x-coordinate within the FB.
 * @param y The y-coordinate within the FB.
 * @return The color of a pixel in RGBA format.
 */
INLINE
u32 GRRLIB_GetPixelFromFB (int x, int y) {
    u32 regval,val;

    regval = 0xc8000000|(_SHIFTL(x,2,10));
    regval = (regval&~0x3FF000)|(_SHIFTL(y,12,10));
    val = *(u32*)regval;

    return RGBA(_SHIFTR(val,16,8), _SHIFTR(val,8,8), val&0xff, _SHIFTR(val,24,8));
}

/**
 * Writes a pixel directly from the FrontBuffer.
 * @param x The x-coordinate within the FB.
 * @param y The y-coordinate within the FB.
 * @param pokeColor The color of the pixel in RGBA format.
 */
INLINE
void GRRLIB_SetPixelToFB (int x, int y, u32 pokeColor) {
    u32 regval;

    regval = 0xc8000000|(_SHIFTL(x,2,10));
    regval = (regval&~0x3FF000)|(_SHIFTL(y,12,10));
    *(u32*)regval = _SHIFTL(A(pokeColor),24,8) | _SHIFTL(R(pokeColor),16,8) | _SHIFTL(G(pokeColor),8,8) | (B(pokeColor)&0xff);
}
