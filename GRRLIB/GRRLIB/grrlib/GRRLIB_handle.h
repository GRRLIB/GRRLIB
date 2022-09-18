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
 * @file GRRLIB_handle.h
 * Inline functions for manipulating texture handles.
 */

/**
 * Set a texture's X and Y handles.
 * For example, it could be used for the rotation of a texture.
 * @param tex The texture to set the handle on.
 * @param x The x-coordinate of the handle.
 * @param y The y-coordinate of the handle.
 */
INLINE
void  GRRLIB_SetHandle (GRRLIB_texImg *tex, const int x, const int y) {
    if (tex->tiledtex) {
        tex->handlex = -(((int)tex->tilew)/2) + x;
        tex->handley = -(((int)tex->tileh)/2) + y;
    } else {
        tex->handlex = -(((int)tex->w)/2) + x;
        tex->handley = -(((int)tex->h)/2) + y;
    }
}

/**
 * Center a texture's handles.
 * For example, it could be used for the rotation of a texture.
 * @param tex The texture to center.
 * @param enabled
 */
INLINE
void  GRRLIB_SetMidHandle (GRRLIB_texImg *tex, const bool enabled) {
    if (enabled == true) {
        if (tex->tiledtex) {
            tex->offsetx = (((int)tex->tilew)/2);
            tex->offsety = (((int)tex->tileh)/2);
        } else {
            tex->offsetx = (((int)tex->w)/2);
            tex->offsety = (((int)tex->h)/2);
        }
        GRRLIB_SetHandle(tex, tex->offsetx, tex->offsety);
    } else {
        GRRLIB_SetHandle(tex, 0, 0);
        tex->offsetx = 0;
        tex->offsety = 0;
    }
}
