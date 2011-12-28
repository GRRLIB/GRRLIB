/*------------------------------------------------------------------------------
Copyright (c) 2012 The GRRLIB Team

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
 * @file GRRLIB_texSetup.h
 * Inline functions for the basic manipulation of textures.
 */

#include <malloc.h>
#include <stdio.h>
#include <string.h>

/**
 * Create an empty texture.
 * @param w Width of the new texture to create.
 * @param h Height of the new texture to create.
 * @return A GRRLIB_texImg structure newly created.
 */
INLINE
GRRLIB_texImg*  GRRLIB_CreateEmptyTexture (const uint w, const uint h)
{
    GRRLIB_texImg *my_texture = (struct GRRLIB_texImg *)calloc(1, sizeof(GRRLIB_texImg));

    if(my_texture != NULL) {
        my_texture->data = memalign(32, h * w * 4);
        my_texture->w = w;
        my_texture->h = h;

        // Initialize the texture
        memset(my_texture->data, '\0', (h * w) << 2);

        GRRLIB_SetHandle(my_texture, 0, 0);
        GRRLIB_FlushTex(my_texture);
    }
    return my_texture;
}

/**
 * Write the contents of a texture in the data cache down to main memory.
 * For performance the CPU holds a data cache where modifications are stored before they get written down to main memory.
 * @param tex The texture to flush.
 */
INLINE
void  GRRLIB_FlushTex (GRRLIB_texImg *tex) {
    DCFlushRange(tex->data, tex->w * tex->h * 4);
}

/**
 * Free memory allocated for texture.
 * @param tex A GRRLIB_texImg structure.
 */
INLINE
void  GRRLIB_FreeTexture (GRRLIB_texImg *tex) {
    if(tex != NULL) {
        if (tex->data != NULL)  free(tex->data);
        free(tex);
        tex = NULL;
    }
}

/**
 * Clear a texture to transparent black.
 * @param tex Texture to clear.
 */
INLINE
void  GRRLIB_ClearTex(GRRLIB_texImg* tex) {
    bzero(tex->data, (tex->h * tex->w) << 2);
    GRRLIB_FlushTex(tex);
}
