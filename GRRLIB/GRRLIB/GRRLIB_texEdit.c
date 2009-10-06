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

#include <jpeglib.h>
#include <malloc.h>
#include <pngu.h>
#include <stdio.h>
#include <string.h>

#include <grrlib.h>

/**
 * Convert a raw BMP (RGB, no alpha) to 4x4RGBA.
 * @author DragonMinded
 * @param src
 * @param dst
 * @param width
 * @param height
*/
static
void  RawTo4x4RGBA (const u8 *src, void *dst,
                    const uint width, const uint height) {
    uint  block;
    uint  i;
    uint  c;
    uint  ar;
    uint  gb;

    u8    *p = (u8*)dst;

    for (block = 0; block < height; block += 4) {
        for (i = 0; i < width; i += 4) {
            /* Alpha and Red */
            for (c = 0; c < 4; ++c) {
                for (ar = 0; ar < 4; ++ar) {
                    /* Alpha pixels */
                    *p++ = 255;
                    /* Red pixels */
                    *p++ = src[((i + ar) + ((block + c) * width)) * 3];
                }
            }

            /* Green and Blue */
            for (c = 0; c < 4; ++c) {
                for (gb = 0; gb < 4; ++gb) {
                    /* Green pixels */
                    *p++ = src[(((i + gb) + ((block + c) * width)) * 3) + 1];
                    /* Blue pixels */
                    *p++ = src[(((i + gb) + ((block + c) * width)) * 3) + 2];
                }
            }
        } /* i */
    } /* block */
}

/**
 * Load a texture from a buffer.
 * @param my_img The JPEG or PNG buffer to load.
 * @return A GRRLIB_texImg structure filled with image information.
 */
GRRLIB_texImg*  GRRLIB_LoadTexture (const u8 my_img[]) {
    // Check for jpeg signature
    if ((my_img[0]==0xFF) && (my_img[1]==0xD8) && (my_img[2]==0xFF))
        return (GRRLIB_LoadTextureJPG(my_img));
    else
        return (GRRLIB_LoadTexturePNG(my_img));
}

/**
 * Load a texture from a buffer.
 * @param my_png the PNG buffer to load.
 * @return A GRRLIB_texImg structure filled with image information.
 */
GRRLIB_texImg*  GRRLIB_LoadTexturePNG (const u8 *my_png) {
    PNGUPROP imgProp;
    IMGCTX ctx;
    GRRLIB_texImg *my_texture = calloc(1, sizeof(GRRLIB_texImg));

    if(my_texture != NULL) {
        ctx = PNGU_SelectImageFromBuffer(my_png);
        PNGU_GetImageProperties(ctx, &imgProp);
        my_texture->data = memalign(32, imgProp.imgWidth * imgProp.imgHeight * 4);
        if(my_texture->data != NULL) {
            PNGU_DecodeTo4x4RGBA8(ctx, imgProp.imgWidth, imgProp.imgHeight, my_texture->data, 255);
            PNGU_ReleaseImageContext(ctx);
            my_texture->w = imgProp.imgWidth;
            my_texture->h = imgProp.imgHeight;
            GRRLIB_SetHandle( my_texture, 0, 0 );
            GRRLIB_FlushTex( my_texture );
        }
    }
    return my_texture;
}

/**
 * Load a texture from a buffer.
 * Take care to have the JPG finnish with 0xFF 0xD9!!
 * @author DrTwox
 * @param my_jpg The JPEG buffer to load.
 * @return A GRRLIB_texImg structure filled with image information.
 */
GRRLIB_texImg*  GRRLIB_LoadTextureJPG (const u8 *my_jpg) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    GRRLIB_texImg *my_texture = calloc(1, sizeof(GRRLIB_texImg));
    int n = 0;
    unsigned int i;

    if(my_texture == NULL)
        return NULL;

    if ((my_jpg[0]==0xFF) && (my_jpg[1]==0xD8) && (my_jpg[2]==0xFF)) {
        while(true) {
            if ((my_jpg[n]==0xFF) && (my_jpg[n+1]==0xD9))
                break;
            n++;
        }
        n+=2;
    }

    jpeg_create_decompress(&cinfo);
    cinfo.err = jpeg_std_error(&jerr);
    cinfo.progress = NULL;
    jpeg_memory_src(&cinfo, my_jpg, n);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);
    unsigned char *tempBuffer = malloc(cinfo.output_width * cinfo.output_height * cinfo.num_components);
    JSAMPROW row_pointer[1];
    row_pointer[0] = malloc(cinfo.output_width * cinfo.num_components);
    size_t location = 0;
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, row_pointer, 1);
        for (i = 0; i < cinfo.image_width * cinfo.num_components; i++) {
            /* Put the decoded scanline into the tempBuffer */
            tempBuffer[ location++ ] = row_pointer[0][i];
        }
    }

    /* Create a buffer to hold the final texture */
    my_texture->data = memalign(32, cinfo.output_width * cinfo.output_height * 4);
    RawTo4x4RGBA(tempBuffer, my_texture->data, cinfo.output_width, cinfo.output_height);

    /* Done - Do cleanup and release allocated memory */
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    free(row_pointer[0]);
    free(tempBuffer);

    my_texture->w = cinfo.output_width;
    my_texture->h = cinfo.output_height;
    GRRLIB_SetHandle( my_texture, 0, 0 );
    GRRLIB_FlushTex( my_texture );
    return my_texture;
}

/**
 * Compose a layer/sprite to a canvas/textured-image.
 * Currently only performs "a-over-b (normal) alpha compositing" (opacity)
 * Ie. Light source is behind the eye, not behind the canvas!
 * @author BlueChip
 * @param xoff   : The x-offset within the canvas (negative values allowed)
 * @param yoff   : The y-offset within the canvas (negative values allowed)
 * @param layer  : The layer/sprite to draw
 * @param canvas : The canvas/textured-image on which to draw
 * @param mode   : Currently unused - will be composition mode
 */
void  GRRLIB_Compose( int xoff, int yoff, GRRLIB_texImg* layer,
                      GRRLIB_texImg* canvas, GRRLIB_ComposeMode mode )
{
    int    x, y;                        // x & y on layer
    int    cnv_x, cnv_y;                // x & y on canvas

    float  cnv_a, lyr_a, alpha;         // Alpha of canvas & layer & result
    u32    cnv_c, lyr_c;                // Colour of pixel from canvas & layer
    u32    new_r, new_g, new_b, new_a;  // R, G, B & A values of result

    // Loop through the layer, one pixel at a time
    for (y = 0;  y < layer->h;  y++)  {
        cnv_y  = y + yoff;               // y coord of canvas pixel to edit
        if (cnv_y < 0)  continue ;       // not on the canvas yet
        if (cnv_y >= canvas->h)  break;  // off the bottom of the canvas

        for (x = 0;  x < layer->w;  x++)  {
            cnv_x  = x + xoff;               // x coord of canvas pixel to edit
            if (cnv_x < 0)  continue ;       // not on the canvas yet
            if (cnv_x >= canvas->h)  break;  // off the right of the canvas

            // Grab the working pixels from the canvas and layer
            cnv_c = GRRLIB_GetPixelFromtexImg(cnv_x, cnv_y, canvas);
            lyr_c = GRRLIB_GetPixelFromtexImg(x, y,         layer);

            // Calculate alpha value as 0.0 to 1.0 in 255th's
            cnv_a = A(cnv_c) /255.0;
            lyr_a = A(lyr_c) /255.0;

            // Perform desired composition
            switch (mode) {
                default:
                case GRRLIB_COMPOSE_NORMAL :
                    // Perform "a-over-b (normal) alpha compositing" (opacity)
                    // http://en.wikipedia.org/wiki/Alpha_compositing
                    new_a = (u32)( A(lyr_c) + (A(cnv_c) *(1.0 -lyr_a)) );
                    alpha =  new_a /255.0;
                    new_r = ( (R(lyr_c) *lyr_a) + (R(cnv_c) *cnv_a *(1 -lyr_a)) ) /alpha;
                    new_g = ( (G(lyr_c) *lyr_a) + (G(cnv_c) *cnv_a *(1 -lyr_a)) ) /alpha;
                    new_b = ( (B(lyr_c) *lyr_a) + (B(cnv_c) *cnv_a *(1 -lyr_a)) ) /alpha;
                    break;
            }

            // Replace the old canvas pixel with the new one
            GRRLIB_SetPixelTotexImg( cnv_x, cnv_y, canvas,
                                     RGBA(new_r, new_g, new_b, new_a) );
        }//for x
    }// for y

    GRRLIB_FlushTex(canvas);
}
