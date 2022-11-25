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

/*
 * @file GRRLIB_clipping.h
 * Inline functions to control clipping.
 */

/**
 * Reset the clipping to normal.
 */
INLINE
void  GRRLIB_ClipReset (void) {
    GX_SetClipMode( GX_CLIP_ENABLE );
    GX_SetScissor( 0, 0, rmode->fbWidth, rmode->efbHeight );
}

/**
 * Clip the drawing area to an rectangle.
 * @param x The x-coordinate of the rectangle.
 * @param y The y-coordinate of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 */
INLINE
void  GRRLIB_ClipDrawing (const u32 x, const u32 y,
                          const u32 width, const u32 height) {
    GX_SetClipMode( GX_CLIP_ENABLE );
    GX_SetScissor( x, y, width, height );
}
