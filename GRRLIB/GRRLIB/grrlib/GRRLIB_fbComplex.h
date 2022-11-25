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
 * @file GRRLIB_fbComplex.h
 * Inline functions for complex (N-point) shape drawing.
 */

/**
 * Draw an array of points.
 * @param v Array containing the points.
 * @param color The color of the points in RGBA format.
 * @param n Number of points in the vector array. The maximum is 65536.
 */
INLINE
void  GRRLIB_NPlot (const guVector v[], const u32 color[], const u16 n) {
    GRRLIB_GXEngine(v, color, n, GX_POINTS);
}

/**
 * Draw a polygon.
 * @param v The vector containing the coordinates of the polygon.
 * @param color The color of the filled polygon in RGBA format.
 * @param n Number of points in the vector. The maximum is 65536.
 */
INLINE
void  GRRLIB_NGone (const guVector v[], const u32 color[], const u16 n) {
    GRRLIB_GXEngine(v, color, n, GX_LINESTRIP);
}

/**
 * Draw a filled polygon.
 * @param v The vector containing the coordinates of the polygon.
 * @param color The color of the filled polygon in RGBA format.
 * @param n Number of points in the vector. The maximum is 65536.
 */
INLINE
void  GRRLIB_NGoneFilled (const guVector v[], const u32 color[], const u16 n) {
    GRRLIB_GXEngine(v, color, n, GX_TRIANGLEFAN);
}
