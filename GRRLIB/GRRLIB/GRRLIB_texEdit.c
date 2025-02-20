/*------------------------------------------------------------------------------
Copyright (c) 2009-2025 The GRRLIB Team

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

#include <malloc.h>
#include <pngu.h>
#include <stdio.h>
#include <jpeglib.h>
#include <string.h>

#include <grrlib.h>

#define TPL_HDR_VERSION_FIELD       0   /**< Version field. */
#define TPL_HDR_NTEXTURE_FIELD      4   /**< Texture field. */
#define TPL_HDR_HDRSIZE_FIELD       8   /**< Header size field. */
#define TPL_HDR_DESCR_FIELD         12  /**< Descriptor field. */

/**
 * Texture header.
 */
typedef struct _tplimgheader TPLImgHeader;

/**
 * Texture header.
 */
struct _tplimgheader {
    u16 height;     /**< Height of the texture in pixels. */
    u16 width;      /**< Width of the texture in pixels. */
    u32 fmt;        /**< Format of the texture. */
    void *data;     /**< Pointer to the texture data. */
    u32 wraps;      /**< Texture coordinate wrapping strategy in the S direction. */ 
    u32 wrapt;      /**< Texture coordinate wrapping strategy in the T direction. */
    u32 minfilter;  /**< Filter mode when the texel/pixel ratio is ≤ 1.0. */
    u32 magfilter;  /**< Filter mode when the texel/pixel ratio is > 1.0. */
    f32 lodbias;    /**< LOD bias. */
    u8 edgelod;     /**< Edge LOD. */
    u8 minlod;      /**< Minimum LOD value. */
    u8 maxlod;      /**< Maximum LOD value. */
    u8 unpacked;    /**< Internal flag. */
} ATTRIBUTE_PACKED;

/**
 * Texture palette header.
 */
typedef struct _tplpalheader TPLPalHeader;

/**
 * Texture palette header.
 */
struct _tplpalheader {
    u16 nitems;     /**< Number of palette entries. */
    u8 unpacked;    /**< Internal flag. */
    u8 pad;         /**< Padding. */
    u32 fmt;        /**< Format of the color lookup table (CLUT). */
    void *data;     /**< Pointer to the color lookup table (CLUT) data. */
} ATTRIBUTE_PACKED;

/**
 * Texture descriptor.
 */
typedef struct _tpldesc TPLDescHeader;

/**
 * Texture descriptor.
 */
struct _tpldesc {
    TPLImgHeader *imghead;  /**< Pointer to texture header structure. */
    TPLPalHeader *palhead;  /**< Pointer to color lookup table (CLUT) header. */
} ATTRIBUTE_PACKED;

/**
 * This structure contains information about the type, size, and layout of a file that containing a device-independent bitmap (DIB).
 */
typedef  struct tagBITMAPFILEHEADER {
    u16 bfType;             /**< Specifies the file type. It must be set to the signature word BM (0x4D42) to indicate bitmap. */
    u32 bfSize;             /**< Specifies the size, in bytes, of the bitmap file. */
    u16 bfReserved1;        /**< Reserved; set to zero. */
    u16 bfReserved2;        /**< Reserved; set to zero. */
    u32 bfOffBits;          /**< Specifies the offset, in bytes, from the BITMAPFILEHEADER structure to the bitmap bits. */
} BITMAPFILEHEADER;
/**
 * This structure contains information about the dimensions and color format of a device-independent bitmap (DIB).
 */
typedef  struct tagBITMAPINFOHEADER {
    u32 biSize;             /**< Specifies the size of the structure, in bytes. */
    u32 biWidth;            /**< Specifies the width of the bitmap, in pixels. */
    u32 biHeight;           /**< Specifies the height of the bitmap, in pixels. */
    u16 biPlanes;           /**< Specifies the number of planes for the target device. */
    u16 biBitCount;         /**< Specifies the number of bits per pixel. */
    u32 biCompression;      /**< Specifies the type of compression for a compressed bottom-up bitmap.*/
    u32 biSizeImage;        /**< Specifies the size, in bytes, of the image. */
    u32 biXPelsPerMeter;    /**< Specifies the horizontal resolution, in pixels per meter, of the target device for the bitmap. */
    u32 biYPelsPerMeter;    /**< Specifies the vertical resolution, in pixels per meter, of the target device for the bitmap. */
    u32 biClrUsed;          /**< Specifies the number of color indexes in the color table that are actually used by the bitmap. */
    u32 biClrImportant;     /**< Specifies the number of color indexes required for displaying the bitmap. */
} BITMAPINFOHEADER;
/**
 * The RGBQUAD structure describes a color consisting of relative intensities of
 * red, green, and blue. The bmiColors member of the BITMAPINFO structure
 * consists of an array of RGBQUAD structures.
 */
typedef  struct tagRGBQUAD {
    u8 rgbBlue;             /**< Specifies the intensity of blue in the color. */
    u8 rgbGreen;            /**< Specifies the intensity of green in the color. */
    u8 rgbRed;              /**< Specifies the intensity of red in the color. */
    u8 rgbReserved;         /**< Not used; must be set to zero. */
} RGBQUAD;

/**
 * Convert a raw BMP (RGB, no alpha) to 4x4RGBA.
 * @author DragonMinded
 * @param src Source.
 * @param dst Destination.
 * @param width Width.
 * @param height Height.
*/
static
void  RawTo4x4RGBA (const u8 *src, void *dst,
                    const u32 width, const u32 height) {
    u8 *p = (u8*)dst;

    for (u32 block = 0; block < height; block += 4) {
        for (u32 i = 0; i < width; i += 4) {
            // Alpha and Red
            for (u8 c = 0; c < 4; ++c) {
                for (u8 argb = 0; argb < 4; ++argb) {
                    // Alpha pixels
                    *p++ = 255;
                    // Red pixels
                    *p++ = src[((i + argb) + ((block + c) * width)) * 3];
                }
            }

            // Green and Blue
            for (u8 c = 0; c < 4; ++c) {
                for (u8 argb = 0; argb < 4; ++argb) {
                    // Green pixels
                    *p++ = src[(((i + argb) + ((block + c) * width)) * 3) + 1];
                    // Blue pixels
                    *p++ = src[(((i + argb) + ((block + c) * width)) * 3) + 2];
                }
            }
        }
    }
}

/**
 * Create an empty texture with a given format.
 * @param width Width of the new texture to create.
 * @param height Height of the new texture to create.
 * @param format Format of the new texture to create.
 * @return A GRRLIB_texImg structure newly created.
 */
GRRLIB_texImg*  GRRLIB_CreateEmptyTextureFmt (const u32 width, const u32 height, const u32 format)
{
    GRRLIB_texImg *my_texture = (struct GRRLIB_texImg *)calloc(1, sizeof(GRRLIB_texImg));

    if (my_texture != NULL) {
        const u32 buffsize = GX_GetTexBufferSize(width, height, format, 0, 0);

        my_texture->data = memalign(32, buffsize);
        my_texture->w = width;
        my_texture->h = height;
        my_texture->format = format;

        // Initialize the texture
        memset(my_texture->data, '\0', buffsize);

        GRRLIB_SetHandle(my_texture, 0, 0);
        GRRLIB_FlushTex(my_texture);
    }
    return my_texture;
}

/**
 * Create an empty texture in GX_TF_RGBA8 format.
 * @param width Width of the new texture to create.
 * @param height Height of the new texture to create.
 * @return A GRRLIB_texImg structure newly created.
 */
GRRLIB_texImg*  GRRLIB_CreateEmptyTexture (const u32 width, const u32 height)
{
    return GRRLIB_CreateEmptyTextureFmt(width, height, GX_TF_RGBA8);
}

/**
 * Set TPL data to a GRRLIB_texImg structure.
 * @param my_tpl The TPL buffer to set.
 * @param id ID of the TPL buffer to set.
 * @return A GRRLIB_texImg structure filled with image information.
 */
GRRLIB_texImg*  GRRLIB_LoadTextureTPL (const u8 *my_tpl, u32 id) {
    if(my_tpl == NULL || my_tpl[0] != 0x00 || my_tpl[1] != 0x20 || my_tpl[2] != 0xAF || my_tpl[3] != 0x30) {
        return NULL;
    }

    const u32 ntextures = *(u32*)(my_tpl + TPL_HDR_NTEXTURE_FIELD);
    if(id > ntextures) {
        return NULL;
    }

    GRRLIB_texImg *my_texture = calloc(1, sizeof(GRRLIB_texImg));
    if (my_texture == NULL) {
        return NULL;
    }

    const TPLDescHeader *deschead = (TPLDescHeader*)(my_tpl + TPL_HDR_DESCR_FIELD);

    u32 pos = (u32)deschead[id].imghead;
    const TPLImgHeader *imghead = (TPLImgHeader*)(my_tpl + pos);

    pos = (u32)imghead->data;
    my_texture->data = (u8*)(my_tpl + pos);
    my_texture->w = imghead->width;
    my_texture->h = imghead->height;
    my_texture->format = imghead->fmt;
    GRRLIB_SetHandle( my_texture, 0, 0 );
    GRRLIB_FlushTex( my_texture );

    return my_texture;
}

/**
 * Load a texture from a buffer.
 * @param my_img The JPEG, PNG or Bitmap buffer to load.
 * @return A GRRLIB_texImg structure filled with image information.
 */
GRRLIB_texImg*  GRRLIB_LoadTexture (const u8 *my_img) {
    if(my_img[0]==0x00 && my_img[1]==0x20 && my_img[2]==0xAF && my_img[3]==0x30)
        return GRRLIB_LoadTextureTPL(my_img, 0); // Load id 0 from TPL
    else if (my_img[0]==0xFF && my_img[1]==0xD8 && my_img[2]==0xFF)
        return (GRRLIB_LoadTextureJPG(my_img));
    else if (my_img[0]=='B' && my_img[1]=='M')
        return (GRRLIB_LoadTextureBMP(my_img));
    else
        return (GRRLIB_LoadTexturePNG(my_img));
}

/**
 * Load a texture from a buffer.
 * @param my_png the PNG buffer to load.
 * @return A GRRLIB_texImg structure filled with image information.
 *         If image size is not correct, the texture will be completely transparent.
 */
GRRLIB_texImg*  GRRLIB_LoadTexturePNG (const u8 *my_png) {
    PNGUPROP imgProp;
    IMGCTX ctx = PNGU_SelectImageFromBuffer(my_png);
    if (PNGU_GetImageProperties(ctx, &imgProp) != PNGU_OK) {
        return NULL;
    }

    GRRLIB_texImg *my_texture = calloc(1, sizeof(GRRLIB_texImg));
    if (my_texture == NULL) {
        return NULL;
    }

    int width = 0;
    int height = 0;
    my_texture->data = PNGU_DecodeTo4x4RGBA8(ctx, imgProp.imgWidth, imgProp.imgHeight, &width, &height);
    if (my_texture->data != NULL) {
        my_texture->w = width;
        my_texture->h = height;
        my_texture->format = GX_TF_RGBA8;
        GRRLIB_SetHandle( my_texture, 0, 0 );
        if (imgProp.imgWidth != width || imgProp.imgHeight != height) {
            // PNGU has resized the texture
            memset(my_texture->data, 0, (my_texture->h * my_texture->w) << 2);
        }
        GRRLIB_FlushTex( my_texture );
    }
    PNGU_ReleaseImageContext(ctx);

    return my_texture;
}

/**
 * Create an array of palette.
 * @param my_bmp Bitmap buffer to parse.
 * @param Size The number of palette to add.
 * @return An array of palette. Memory must be deleted.
 */
static RGBQUAD*  GRRLIB_CreatePalette (const u8 *my_bmp, u32 Size) {
    RGBQUAD *Palette = calloc(Size, sizeof(RGBQUAD));
    for(u32 n=0, i=0; n<Size; n++) {
        Palette[n].rgbBlue = my_bmp[i];
        Palette[n].rgbGreen = my_bmp[i+1];
        Palette[n].rgbRed = my_bmp[i+2];
        Palette[n].rgbReserved = 0;
        i += 4;
    }
    return Palette;
}

/**
 * Load a texture from a buffer.
 * It only works for the MS-Windows standard format uncompressed (1-bit, 4-bit, 8-bit, 24-bit and 32-bit).
 * @param my_bmp The Bitmap buffer to load.
 * @return A GRRLIB_texImg structure filled with image information.
 */
GRRLIB_texImg*  GRRLIB_LoadTextureBMP (const u8 *my_bmp) {
    GRRLIB_texImg *my_texture = calloc(1, sizeof(GRRLIB_texImg));

    if (my_texture == NULL) {
        return NULL;
    }

    BITMAPFILEHEADER MyBitmapFileHeader;
    BITMAPINFOHEADER MyBitmapHeader;

    // Fill file header structure
    MyBitmapFileHeader.bfType      = (my_bmp[0]  | my_bmp[1]<<8);
    MyBitmapFileHeader.bfSize      = (my_bmp[2]  | my_bmp[3]<<8  | my_bmp[4]<<16 | my_bmp[5]<<24);
    MyBitmapFileHeader.bfReserved1 = (my_bmp[6]  | my_bmp[7]<<8);
    MyBitmapFileHeader.bfReserved2 = (my_bmp[8]  | my_bmp[9]<<8);
    MyBitmapFileHeader.bfOffBits   = (my_bmp[10] | my_bmp[11]<<8 | my_bmp[12]<<16 | my_bmp[13]<<24);
    // Fill the bitmap structure
    MyBitmapHeader.biSize          = (my_bmp[14] | my_bmp[15]<<8 | my_bmp[16]<<16 | my_bmp[17]<<24);
    MyBitmapHeader.biWidth         = (my_bmp[18] | my_bmp[19]<<8 | my_bmp[20]<<16 | my_bmp[21]<<24);
    MyBitmapHeader.biHeight        = (my_bmp[22] | my_bmp[23]<<8 | my_bmp[24]<<16 | my_bmp[25]<<24);
    MyBitmapHeader.biPlanes        = (my_bmp[26] | my_bmp[27]<<8);
    MyBitmapHeader.biBitCount      = (my_bmp[28] | my_bmp[29]<<8);
    MyBitmapHeader.biCompression   = (my_bmp[30] | my_bmp[31]<<8 | my_bmp[32]<<16 | my_bmp[33]<<24);
    MyBitmapHeader.biSizeImage     = (my_bmp[34] | my_bmp[35]<<8 | my_bmp[36]<<16 | my_bmp[37]<<24);
    MyBitmapHeader.biXPelsPerMeter = (my_bmp[38] | my_bmp[39]<<8 | my_bmp[40]<<16 | my_bmp[41]<<24);
    MyBitmapHeader.biYPelsPerMeter = (my_bmp[42] | my_bmp[43]<<8 | my_bmp[44]<<16 | my_bmp[45]<<24);
    MyBitmapHeader.biClrUsed       = (my_bmp[46] | my_bmp[47]<<8 | my_bmp[48]<<16 | my_bmp[49]<<24);
    MyBitmapHeader.biClrImportant  = (my_bmp[50] | my_bmp[51]<<8 | my_bmp[52]<<16 | my_bmp[53]<<24);

    my_texture->data = memalign(32, MyBitmapHeader.biWidth * MyBitmapHeader.biHeight * 4);
    if (my_texture->data != NULL && MyBitmapFileHeader.bfType == 0x4D42) {
        u32 BufferSize;
        s32 i;
        RGBQUAD *Palette;
        my_texture->w = MyBitmapHeader.biWidth;
        my_texture->h = MyBitmapHeader.biHeight;
        my_texture->format = GX_TF_RGBA8;
        switch(MyBitmapHeader.biBitCount) {
            case 32:    // RGBA images
                i = 54;
                for(s32 y=MyBitmapHeader.biHeight-1; y>=0; y--) {
                    for(s32 x=0; x<MyBitmapHeader.biWidth; x++) {
                        GRRLIB_SetPixelTotexImg(x, y, my_texture,
                            RGBA(my_bmp[i+2], my_bmp[i+1], my_bmp[i], my_bmp[i+3]));
                        i += 4;
                    }
                }
                break;
            case 24:    // truecolor images
                BufferSize = (MyBitmapHeader.biWidth % 4);
                i = 54;
                for(s32 y=MyBitmapHeader.biHeight-1; y>=0; y--) {
                    for(s32 x=0; x<MyBitmapHeader.biWidth; x++) {
                        GRRLIB_SetPixelTotexImg(x, y, my_texture,
                            RGBA(my_bmp[i+2], my_bmp[i+1], my_bmp[i], 0xFF));
                        i += 3;
                    }
                    i += BufferSize;   // Padding
                }
                break;
            case 8:     // 256 color images
                BufferSize = (int) MyBitmapHeader.biWidth;
                while(BufferSize % 4) {
                    BufferSize++;
                }
                BufferSize -= MyBitmapHeader.biWidth;
                Palette = GRRLIB_CreatePalette(&my_bmp[54], 256);
                i = 1078; // 54 + (MyBitmapHeader.biBitCount * 4)
                for(s32 y=MyBitmapHeader.biHeight-1; y>=0; y--) {
                    for(s32 x=0; x<MyBitmapHeader.biWidth; x++) {
                        GRRLIB_SetPixelTotexImg(x, y, my_texture,
                            RGBA(Palette[my_bmp[i]].rgbRed,
                                    Palette[my_bmp[i]].rgbGreen,
                                    Palette[my_bmp[i]].rgbBlue,
                                    0xFF));
                        i++;
                    }
                    i += BufferSize;   // Padding
                }
                free(Palette);
                break;
            case 4:     // 16 color images
                BufferSize = (int)((MyBitmapHeader.biWidth*4) / 8.0);
                while(8*BufferSize < MyBitmapHeader.biWidth*4) {
                    BufferSize++;
                }
                while(BufferSize % 4) {
                    BufferSize++;
                }
                Palette = GRRLIB_CreatePalette(&my_bmp[54], 16);
                i = 118; // 54 + (MyBitmapHeader.biBitCount * 4)
                for(s32 y=MyBitmapHeader.biHeight-1; y>=0; y--) {
                    for(s32 x=0; x<MyBitmapHeader.biWidth; x++) {
                        u16 pal_ref = (my_bmp[i + (x / 2)] >> ((x % 2) ? 0 : 4)) & 0x0F;
                        GRRLIB_SetPixelTotexImg(x, y, my_texture,
                            RGBA(Palette[pal_ref].rgbRed,
                                    Palette[pal_ref].rgbGreen,
                                    Palette[pal_ref].rgbBlue,
                                    0xFF));
                    }
                    i += BufferSize;   // Padding
                }
                free(Palette);
                break;
            case 1:     // black & white images
                BufferSize = (int)(MyBitmapHeader.biWidth / 8.0);
                while(8*BufferSize < MyBitmapHeader.biWidth) {
                    BufferSize++;
                }
                while(BufferSize % 4) {
                    BufferSize++;
                }
                Palette = GRRLIB_CreatePalette(&my_bmp[54], 2);
                i = 62; // 54 + (MyBitmapHeader.biBitCount * 4)
                for(s32 y=MyBitmapHeader.biHeight-1; y>=0; y--) {
                    for(s32 x=0; x<MyBitmapHeader.biWidth; x++) {
                        u16 pal_ref = (my_bmp[i + (x / 8)] >> (-x%8+7)) & 0x01;
                        GRRLIB_SetPixelTotexImg(x, y, my_texture,
                            RGBA(Palette[pal_ref].rgbRed,
                                    Palette[pal_ref].rgbGreen,
                                    Palette[pal_ref].rgbBlue,
                                    0xFF));
                    }
                    i += BufferSize;   // Padding
                }
                free(Palette);
                break;
            default:
                GRRLIB_ClearTex(my_texture);
        }
        GRRLIB_SetHandle( my_texture, 0, 0 );
        GRRLIB_FlushTex( my_texture );
    }

    return my_texture;
}

/**
 * Load a texture from a buffer.
 * Take care to have the JPG finish with 0xFF 0xD9!
 * @param my_jpg The JPEG buffer to load.
 * @return A GRRLIB_texImg structure filled with image information.
 */
GRRLIB_texImg*  GRRLIB_LoadTextureJPG (const u8 *my_jpg) {
    int n = 0;

    if ((my_jpg[0]==0xFF) && (my_jpg[1]==0xD8) && (my_jpg[2]==0xFF)) {
        while(true) {
            if ((my_jpg[n]==0xFF) && (my_jpg[n+1]==0xD9))
                break;
            n++;
        }
        n+=2;
    }

    return GRRLIB_LoadTextureJPGEx(my_jpg, n);
}

/**
 * Load a texture from a buffer.
 * @author DrTwox
 * @param my_jpg The JPEG buffer to load.
 * @param my_size Size of the JPEG buffer to load.
 * @return A GRRLIB_texImg structure filled with image information.
 */
GRRLIB_texImg*  GRRLIB_LoadTextureJPGEx (const u8 *my_jpg, const u32 my_size) {
    GRRLIB_texImg *my_texture = calloc(1, sizeof(GRRLIB_texImg));

    if (my_texture == NULL) {
        return NULL;
    }

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    jpeg_create_decompress(&cinfo);
    cinfo.err = jpeg_std_error(&jerr);
    cinfo.progress = NULL;
    jpeg_mem_src(&cinfo, (const unsigned char *)my_jpg, my_size);
    jpeg_read_header(&cinfo, TRUE);
    if (cinfo.jpeg_color_space == JCS_GRAYSCALE) {
        cinfo.out_color_space = JCS_RGB;
    }
    jpeg_start_decompress(&cinfo);
    unsigned char *tempBuffer = malloc(cinfo.output_width * cinfo.output_height * cinfo.output_components);
    JSAMPROW row_pointer[1];
    row_pointer[0] = malloc(cinfo.output_width * cinfo.output_components);
    size_t location = 0;
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, row_pointer, 1);
        for (unsigned int i = 0; i < cinfo.image_width * cinfo.output_components; i++) {
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
    my_texture->format = GX_TF_RGBA8;
    GRRLIB_SetHandle( my_texture, 0, 0 );
    GRRLIB_FlushTex( my_texture );
    return my_texture;
}
