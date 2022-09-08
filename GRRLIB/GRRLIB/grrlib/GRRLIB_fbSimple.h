/*------------------------------------------------------------------------------
Copyright (c) 2009-2017 The GRRLIB Team

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
 * @file GRRLIB_fbSimple.h
 * Inline functions for primitive point and line drawing.
 */

/**
 * Clear screen with a specific color.
 * @param color The color to use to fill the screen.
 */
INLINE
void  GRRLIB_FillScreen (const u32 color) {
    GRRLIB_Rectangle(-40.0f, -40.0f, rmode->fbWidth + 80.0f, rmode->xfbHeight + 80.0f, color, true);
}

/**
 * Draw a dot.
 * @param x Specifies the x-coordinate of the dot.
 * @param y Specifies the y-coordinate of the dot.
 * @param color The color of the dot in RGBA format.
 * @author Jespa
 */
INLINE
void  GRRLIB_Plot (const f32 x,  const f32 y, const u32 color) {
    GX_Begin(GX_POINTS, GX_VTXFMT0, 1);
        GX_Position3f32(x, y, 0.0f);
        GX_Color1u32(color);
    GX_End();
}

/**
 * Draw a line.
 * @param x1 Starting point for line for the x coordinate.
 * @param y1 Starting point for line for the y coordinate.
 * @param x2 Ending point for line for the x coordinate.
 * @param y2 Ending point for line for the x coordinate.
 * @param color Line color in RGBA format.
 * @author JESPA
 */
INLINE
void  GRRLIB_Line (const f32 x1, const f32 y1,
                   const f32 x2, const f32 y2, const u32 color) {
    GX_Begin(GX_LINES, GX_VTXFMT0, 2);
        GX_Position3f32(x1, y1, 0.0f);
        GX_Color1u32(color);
        GX_Position3f32(x2, y2, 0.0f);
        GX_Color1u32(color);
    GX_End();
}

/**
 * Draw a rectangle.
 * @param x Specifies the x-coordinate of the upper-left corner of the rectangle.
 * @param y Specifies the y-coordinate of the upper-left corner of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color The color of the rectangle in RGBA format.
 * @param filled Set to @c true to fill the rectangle.
 */
INLINE
void  GRRLIB_Rectangle (const f32 x,      const f32 y,
                        const f32 width,  const f32 height,
                        const u32 color, const bool filled) {
    const f32 x2 = x + width;
    const f32 y2 = y + height;

    if (filled == true) {
        GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
            GX_Position3f32(x, y, 0.0f);
            GX_Color1u32(color);
            GX_Position3f32(x2, y, 0.0f);
            GX_Color1u32(color);
            GX_Position3f32(x2, y2, 0.0f);
            GX_Color1u32(color);
            GX_Position3f32(x, y2, 0.0f);
            GX_Color1u32(color);
        GX_End();
    }
    else {
        GX_Begin(GX_LINESTRIP, GX_VTXFMT0, 5);
            GX_Position3f32(x, y, 0.0f);
            GX_Color1u32(color);
            GX_Position3f32(x2, y, 0.0f);
            GX_Color1u32(color);
            GX_Position3f32(x2, y2, 0.0f);
            GX_Color1u32(color);
            GX_Position3f32(x, y2, 0.0f);
            GX_Color1u32(color);
            GX_Position3f32(x, y, 0.0f);
            GX_Color1u32(color);
        GX_End();
    }
}
