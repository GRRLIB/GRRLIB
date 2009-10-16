/*------------------------------------------------------------------------------
Copyright (c) 2009 The GRRLIB Team

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
#include <stdio.h>

/**
 * Load a file to memory.
 * @param filename Name of the file to be loaded.
 * @param data Pointer-to-your-pointer. Ie. { u8 *data; load("file", &data); }.
 * It is your responsibility to free the memory allocated by this function.
 * @return int 0:EmptyFile, -1:FileNotFound, -2:OutOfMemory, -3:FileReadError,
 *             >0 -> FileLength.
 */
int  GRRLIB_LoadFile(const char* filename, unsigned char* *data) {
    int   len;
    FILE  *fd;

    // Open the file
    if ( !(fd = fopen(filename, "rb")) )  return -1 ;

    // Get file length
    fseek(fd, 0, SEEK_END);
    if ( !(len = ftell(fd)) )  {
        *data = NULL;
        return 0;
    }
    fseek(fd, 0, SEEK_SET);

    // Grab some memory in which to store the file
    if ( !(*data = malloc(len)) ) {
       fclose(fd);
       return -2;
    }

    if ( fread(*data, 1, len, fd) != len)  {
      fclose(fd);
      free(*data);  *data = NULL;
      return -3;
    }

    fclose(fd);
    return len;
}

/**
 * Load a texture from a file.
 * @param filename The JPEG or PNG filename to load.
 * @return A GRRLIB_texImg structure filled with image information.
 */
GRRLIB_texImg*  GRRLIB_LoadTextureFromFile(const char *filename) {
    GRRLIB_texImg  *tex;
    unsigned char  *data;

    // return NULL it load fails
    if (GRRLIB_LoadFile(filename, &data) <= 0)  return NULL ;

    // Convert to texture
    tex = GRRLIB_LoadTexture(data);

    // Free up the buffer
    free(data);

    return tex;
}

/**
 * Make a PNG screenshot.
 * libfat is required to use the function.
 * @param filename Name of the file to write.
 * @return bool true=everything worked, false=problems occurred.
 */
bool  GRRLIB_ScrShot(const char* filename) {
    IMGCTX  pngContext;
    int     ret = -1;

    if ( (pngContext = PNGU_SelectImageFromDevice(filename)) ) {
        ret = PNGU_EncodeFromYCbYCr( pngContext,
                                     rmode->fbWidth, rmode->efbHeight,
                                     xfb[fb], 0 );
        PNGU_ReleaseImageContext(pngContext);
    }
    return !ret;
}
