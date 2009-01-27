/*===========================================
        GRRLIB (GX version) 3.0.5 alpha
        Code     : NoNameNo
        Additional Code : Crayon
        GX hints : RedShade
===========================================*/
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdarg.h>
#include <string.h>
#include "../libpng/pngu/pngu.h"
#include "../libjpeg/jpeglib.h"
#include "GRRLIB.h"

#define DEFAULT_FIFO_SIZE (256 * 1024)

u32 fb = 0;
static void *xfb[2] = { NULL, NULL};
GXRModeObj *rmode;
void *gp_fifo = NULL;

/**
 * Clear screen with a specific color.
 * @param color the color to use to fill the screen.
 */
inline void GRRLIB_FillScreen(u32 color) {
    GRRLIB_Rectangle(-40, -40, 680, 520, color, 1);
}

/**
 * Draw a dot.
 * @param x specifies the x-coordinate of the dot. 
 * @param y specifies the y-coordinate of the dot. 
 * @param color the color of the dot.
 */
inline void GRRLIB_Plot(f32 x, f32 y, u32 color) {
    Vector  v[] = {{x,y,0.0f}};

    GRRLIB_NPlot(v, color, 1);
}

/**
 *
 * @param v
 * @param color
 * @param n
 */
void GRRLIB_NPlot(Vector v[], u32 color, long n) {
    GRRLIB_GXEngine(v, color, n, GX_POINTS);
}

/**
 * Draw a line.
 * @param x1 start point for line for the x coordinate.
 * @param y1 start point for line for the y coordinate.
 * @param x2 end point for line for the x coordinate.
 * @param y2 end point for line for the x coordinate.
 * @param color line color.
 */
inline void GRRLIB_Line(f32 x1, f32 y1, f32 x2, f32 y2, u32 color) {
    Vector  v[] = {{x1,y1,0.0f}, {x2,y2,0.0f}};

    GRRLIB_NGone(v, color, 2);
}

/**
 * Draw a rectangle.
 * @param x specifies the x-coordinate of the upper-left corner of the rectangle.
 * @param y specifies the y-coordinate of the upper-left corner of the rectangle.
 * @param width the width of the rectangle.
 * @param height the height of the rectangle.
 * @param color the color of the rectangle.
 * @param filled true to fill the rectangle with a color.
 */
inline void GRRLIB_Rectangle(f32 x, f32 y, f32 width, f32 height, u32 color, u8 filled) {
    f32 x2 = x+width;
    f32 y2 = y+height;
    Vector  v[] = {{x,y,0.0f}, {x2,y,0.0f}, {x2,y2,0.0f}, {x,y2,0.0f}, {x,y,0.0f}};

    if(!filled) {
        GRRLIB_NGone(v, color, 5);
    }
    else{
        GRRLIB_NGoneFilled(v, color, 4);
    }
}

/**
 *
 * @param v
 * @param color
 * @param n
 */
void GRRLIB_NGone(Vector v[], u32 color, long n) {
    GRRLIB_GXEngine(v, color, n, GX_LINESTRIP);
}

/**
 *
 * @param v
 * @param color
 * @param n
 */
void GRRLIB_NGoneFilled(Vector v[], u32 color, long n) {
    GRRLIB_GXEngine(v, color, n, GX_TRIANGLEFAN);
}

/**
 *
 * @param tex
 * @param tilew
 * @param tileh
 * @param tilestart
 */
void GRRLIB_InitTileSet(struct GRRLIB_texImg *tex, unsigned int tilew, unsigned int tileh, unsigned int tilestart) {
    tex->tilew = tilew;
    tex->tileh = tileh;
    tex->nbtilew = tex->w / tilew;
    tex->nbtileh = tex->h / tileh;
    tex->tilestart = tilestart;
}

/**
 * Load a texture from a buffer.
 * @param my_png the PNG buffer to load.
 * @return A GRRLIB_texImg structure filled with PNG informations.
 */
GRRLIB_texImg GRRLIB_LoadTexturePNG(const unsigned char my_png[]) {
    PNGUPROP imgProp;
    IMGCTX ctx;
    GRRLIB_texImg my_texture;

    ctx = PNGU_SelectImageFromBuffer(my_png);
    PNGU_GetImageProperties (ctx, &imgProp);
    my_texture.data = memalign (32, imgProp.imgWidth * imgProp.imgHeight * 4);
    PNGU_DecodeTo4x4RGBA8 (ctx, imgProp.imgWidth, imgProp.imgHeight, my_texture.data, 255);
    PNGU_ReleaseImageContext (ctx);
    DCFlushRange (my_texture.data, imgProp.imgWidth * imgProp.imgHeight * 4);
    my_texture.w = imgProp.imgWidth;
    my_texture.h = imgProp.imgHeight;
    return my_texture;
}

/**
 * Convert a raw bmp (RGB, no alpha) to 4x4RGBA.
 * @author DrTwox
 * @param src
 * @param dst
 * @param width
 * @param height
*/
static void RawTo4x4RGBA(const unsigned char *src, void *dst, const unsigned int width, const unsigned int height) {
    unsigned int block;
    unsigned int i;
    unsigned int c;
    unsigned int ar;
    unsigned int gb;
    unsigned char *p = (unsigned char*)dst;

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
 * Take Care to have a JPG finnishing by 0xFF 0xD9 !!!!
 * @author DrTwox
 * @param my_jpg the JPEG buffer to load.
 * @return A GRRLIB_texImg structure filled with PNG informations.
 */
GRRLIB_texImg GRRLIB_LoadTextureJPG(const unsigned char my_jpg[]) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    GRRLIB_texImg my_texture;
    int n = 0;
    unsigned int i;

    if((my_jpg[0]==0xff) && (my_jpg[1]==0xd8) && (my_jpg[2]==0xff)) {
        while(1) {
            if((my_jpg[n]==0xff) && (my_jpg[n+1]==0xd9))
                break;
            n++;
        }
        n+=2;
    }

    /* Init the JPEG decompressor */
    jpeg_create_decompress(&cinfo);

    /* Use the standard error handler */
    cinfo.err = jpeg_std_error(&jerr);

    /* Don't use a progress handler */
    cinfo.progress = NULL;

    /* Set the source buffer */
    jpeg_memory_src(&cinfo, my_jpg, n);

    /* Read the default header information */
    jpeg_read_header(&cinfo, TRUE);

    /* Get ready to decompress */
    jpeg_start_decompress(&cinfo);

    /* Create a buffer to hold the final image */
    unsigned char *tempBuffer = (unsigned char*) malloc(cinfo.output_width * cinfo.output_height * cinfo.num_components);

    /* Decompress the JPEG into tempBuffer, one row at a time */
    JSAMPROW row_pointer[1];
    row_pointer[0] = (unsigned char*) malloc(cinfo.output_width * cinfo.num_components);
    size_t location = 0;
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, row_pointer, 1);
        for (i = 0; i < cinfo.image_width * cinfo.num_components; i++) {
            /* Put the decoded scanline into the tempBuffer */
            tempBuffer[ location++ ] = row_pointer[0][i];
        }
    }

    /* Create a buffer to hold the final texture */
    my_texture.data = memalign(32, cinfo.output_width * cinfo.output_height * 4);
    RawTo4x4RGBA(tempBuffer, my_texture.data, cinfo.output_width, cinfo.output_height);
    DCFlushRange(my_texture.data, cinfo.output_width * cinfo.output_height * 4);

    /* Done - do cleanup and release memory */
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    free(row_pointer[0]);
    free(tempBuffer);

    my_texture.w = cinfo.output_width;
    my_texture.h = cinfo.output_height;

    return my_texture;
}

/**
 * Draw a texture.
 * @param xpos specifies the x-coordinate of the upper-left corner.
 * @param ypos specifies the y-coordinate of the upper-left corner.
 * @param tex texture to draw.
 * @param degrees angle of rotation.
 * @param scaleX
 * @param scaleY
 * @param color
 */
inline void GRRLIB_DrawImg(f32 xpos, f32 ypos, GRRLIB_texImg tex, float degrees, float scaleX, f32 scaleY, u32 color ) {
    GXTexObj texObj;
    u16 width, height;
    Mtx m, m1, m2, mv;

    GX_InitTexObj(&texObj, tex.data, tex.w, tex.h, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
    GX_LoadTexObj(&texObj, GX_TEXMAP0);

    GX_SetTevOp (GX_TEVSTAGE0, GX_MODULATE);
    GX_SetVtxDesc (GX_VA_TEX0, GX_DIRECT);

    width = tex.w * 0.5;
    height = tex.h * 0.5;
    guMtxIdentity (m1);
    guMtxScaleApply(m1, m1, scaleX, scaleY, 1.0);
    Vector axis = (Vector) {0, 0, 1 };
    guMtxRotAxisDeg (m2, &axis, degrees);
    guMtxConcat(m2, m1, m);

    guMtxTransApply(m, m, xpos+width, ypos+height, 0);
    guMtxConcat (GXmodelView2D, m, mv);
    GX_LoadPosMtxImm (mv, GX_PNMTX0);

    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
    GX_Position3f32(-width, -height, 0);
    GX_Color1u32(color);
    GX_TexCoord2f32(0, 0);

    GX_Position3f32(width, -height, 0);
    GX_Color1u32(color);
    GX_TexCoord2f32(1, 0);

    GX_Position3f32(width, height, 0);
    GX_Color1u32(color);
    GX_TexCoord2f32(1, 1);

    GX_Position3f32(-width, height, 0);
    GX_Color1u32(color);
    GX_TexCoord2f32(0, 1);
    GX_End();
    GX_LoadPosMtxImm (GXmodelView2D, GX_PNMTX0);

    GX_SetTevOp (GX_TEVSTAGE0, GX_PASSCLR);
    GX_SetVtxDesc (GX_VA_TEX0, GX_NONE);
}

/**
 * Draw a tile.
 * @param xpos specifies the x-coordinate of the upper-left corner.
 * @param ypos specifies the y-coordinate of the upper-left corner.
 * @param tex texture to draw.
 * @param degrees angle of rotation.
 * @param scaleX
 * @param scaleY
 * @param color
 * @param frame
 */
inline void GRRLIB_DrawTile(f32 xpos, f32 ypos, GRRLIB_texImg tex, float degrees, float scaleX, f32 scaleY, u32 color, int frame) {
    GXTexObj texObj;
    f32 width, height;
    Mtx m, m1, m2, mv;

    // Frame Correction by spiffen
    f32 FRAME_CORR = 0.001f;
    f32 s1 = (((frame%tex.nbtilew))/(f32)tex.nbtilew)+(FRAME_CORR/tex.w);
    f32 s2 = (((frame%tex.nbtilew)+1)/(f32)tex.nbtilew)-(FRAME_CORR/tex.w);
    f32 t1 = (((int)(frame/tex.nbtilew))/(f32)tex.nbtileh)+(FRAME_CORR/tex.h);
    f32 t2 = (((int)(frame/tex.nbtilew)+1)/(f32)tex.nbtileh)-(FRAME_CORR/tex.h);

    GX_InitTexObj(&texObj, tex.data, tex.tilew*tex.nbtilew, tex.tileh*tex.nbtileh, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
    GX_LoadTexObj(&texObj, GX_TEXMAP0);

    GX_SetTevOp (GX_TEVSTAGE0, GX_MODULATE);
    GX_SetVtxDesc (GX_VA_TEX0, GX_DIRECT);

    width = tex.tilew * 0.5f;
    height = tex.tileh * 0.5f;
    guMtxIdentity (m1);
    guMtxScaleApply(m1, m1, scaleX, scaleY, 1.0f);
    Vector axis = (Vector) {0, 0, 1 };
    guMtxRotAxisDeg (m2, &axis, degrees);
    guMtxConcat(m2, m1, m);
    guMtxTransApply(m, m, xpos+width, ypos+height, 0);
    guMtxConcat (GXmodelView2D, m, mv);
    GX_LoadPosMtxImm (mv, GX_PNMTX0);
    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
    GX_Position3f32(-width, -height, 0);
    GX_Color1u32(color);
    GX_TexCoord2f32(s1, t1);

    GX_Position3f32(width, -height,  0);
    GX_Color1u32(color);
    GX_TexCoord2f32(s2, t1);

    GX_Position3f32(width, height,  0);
    GX_Color1u32(color);
    GX_TexCoord2f32(s2, t2);

    GX_Position3f32(-width, height,  0);
    GX_Color1u32(color);
    GX_TexCoord2f32(s1, t2);
    GX_End();
    GX_LoadPosMtxImm (GXmodelView2D, GX_PNMTX0);

    GX_SetTevOp (GX_TEVSTAGE0, GX_PASSCLR);
    GX_SetVtxDesc (GX_VA_TEX0, GX_NONE);
}

/**
 * Print formatted output.
 * @param xpos
 * @param ypos
 * @param tex
 * @param color
 * @param zoom
 * @param ... Optional arguments.
 */
void GRRLIB_Printf(f32 xpos, f32 ypos, GRRLIB_texImg tex, u32 color, f32 zoom, const char *text, ...) {
    int i, size;
    char tmp[1024];

    va_list argp;
    va_start(argp, text);
    size = vsprintf(tmp, text, argp);
    va_end(argp);

    for(i=0; i<size; i++) {
        u8 c = tmp[i]-tex.tilestart;
        GRRLIB_DrawTile(xpos+i*tex.tilew*zoom, ypos, tex, 0, zoom, zoom, color, c);
    }
}

/**
 * Determines whether the specified point lies within the specified rectangle.
 * @param hotx specifies the x-coordinate of the upper-left corner of the rectangle.
 * @param hoty specifies the y-coordinate of the upper-left corner of the rectangle.
 * @param hotw the width of the rectangle.
 * @param hoth the height of the rectangle.
 * @param wpadx specifies the x-coordinate of the point.
 * @param wpady specifies the y-coordinate of the point.
 * @return If the specified point lies within the rectangle, the return value is true otherwise it's false.
 */
bool GRRLIB_PtInRect(int hotx, int hoty, int hotw, int hoth, int wpadx, int wpady) {
    return(((wpadx>=hotx) & (wpadx<=(hotx+hotw))) & ((wpady>=hoty) & (wpady<=(hoty+hoth))));
}

/**
 * Determines whether a specified rectangle lies within another rectangle.
 */
bool GRRLIB_RectInRect(int rect1x, int rect1y, int rect1w, int rect1h, int rect2x, int rect2y, int rect2w, int rect2h) {
    return ((rect1x >= rect2x) && (rect1y >= rect2y) &&
        (rect1x+rect1w <= rect2x+rect2w) && (rect1y+rect1h <= rect2y+rect2h));
}

/**
 * Determines whether a part of a specified rectangle lies on another rectangle.
 */
bool GRRLIB_RectOnRect(int rect1x, int rect1y, int rect1w, int rect1h, int rect2x, int rect2y, int rect2w, int rect2h) {
    return (GRRLIB_PtInRect(rect1x, rect1y, rect1w, rect1h, rect2x, rect2y) ||
        GRRLIB_PtInRect(rect1x, rect1y, rect1w, rect1h, rect2x+rect2w, rect2y) ||
        GRRLIB_PtInRect(rect1x, rect1y, rect1w, rect1h, rect2x+rect2w, rect2y+rect2h) ||
        GRRLIB_PtInRect(rect1x, rect1y, rect1w, rect1h, rect2x, rect2y+rect2h));
}

/**
 *
 * @param v
 * @param color
 * @param fmt
 */
void GRRLIB_GXEngine(Vector v[], u32 color, long n, u8 fmt) {
    int i;

    GX_Begin(fmt, GX_VTXFMT0, n);
    for(i=0; i<n; i++) {
        GX_Position3f32(v[i].x, v[i].y,  v[i].z);
        GX_Color1u32(color);
    }
    GX_End();
}

/**
 * Initialize GRRLIB. Call this at the beginning your code.
 * @see GRRLIB_Exit
 */
void GRRLIB_Init() {
    f32 yscale;
    u32 xfbHeight;
    Mtx44 perspective;

    VIDEO_Init();
    rmode = VIDEO_GetPreferredMode(NULL);
    if(rmode == NULL)
        return;
    VIDEO_Configure (rmode);
    xfb[0] = (u32 *)MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    xfb[1] = (u32 *)MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    if(xfb[0] == NULL || xfb[1] == NULL)
        return;

    VIDEO_SetNextFramebuffer(xfb[fb]);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if(rmode->viTVMode&VI_NON_INTERLACE)
        VIDEO_WaitVSync();

    gp_fifo = (u8 *) memalign(32, DEFAULT_FIFO_SIZE);
    if(gp_fifo == NULL)
        return;
    memset(gp_fifo, 0, DEFAULT_FIFO_SIZE);
    GX_Init(gp_fifo, DEFAULT_FIFO_SIZE);

    // clears the bg to color and clears the z buffer
    GXColor background = { 0, 0, 0, 0xff };
    GX_SetCopyClear (background, GX_MAX_Z24);

    // other gx setup
    yscale = GX_GetYScaleFactor(rmode->efbHeight, rmode->xfbHeight);
    xfbHeight = GX_SetDispCopyYScale(yscale);
    GX_SetScissor(0, 0, rmode->fbWidth, rmode->efbHeight);
    GX_SetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
    GX_SetDispCopyDst(rmode->fbWidth, xfbHeight);
    GX_SetCopyFilter(rmode->aa, rmode->sample_pattern, GX_TRUE, rmode->vfilter);
    GX_SetFieldMode(rmode->field_rendering, ((rmode->viHeight==2*rmode->xfbHeight)?GX_ENABLE:GX_DISABLE));

    if (rmode->aa)
        GX_SetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
    else
        GX_SetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);

    GX_SetDispCopyGamma(GX_GM_1_0);


    // setup the vertex descriptor
    // tells the flipper to expect direct data
    GX_ClearVtxDesc();
    GX_InvVtxCache ();
    GX_InvalidateTexAll();

    GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);
    GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);


    GX_SetVtxAttrFmt (GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GX_SetVtxAttrFmt (GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GX_SetZMode (GX_FALSE, GX_LEQUAL, GX_TRUE);

    GX_SetNumChans(1);
    GX_SetNumTexGens(1);
    GX_SetTevOp (GX_TEVSTAGE0, GX_PASSCLR);
    GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

    guMtxIdentity(GXmodelView2D);
    guMtxTransApply (GXmodelView2D, GXmodelView2D, 0.0F, 0.0F, -50.0F);
    GX_LoadPosMtxImm(GXmodelView2D, GX_PNMTX0);

    guOrtho(perspective,0, 479, 0, 639, 0, 300.0F);
    GX_LoadProjectionMtx(perspective, GX_ORTHOGRAPHIC);

    GX_SetViewport(0, 0, rmode->fbWidth, rmode->efbHeight, 0, 1);
    GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GX_SetAlphaUpdate(GX_TRUE);

    GX_SetCullMode(GX_CULL_NONE);
}

/**
 * Call this function after drawing.
 */
void GRRLIB_Render() {
    GX_DrawDone ();

    fb ^= 1;        // flip framebuffer
    GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GX_SetColorUpdate(GX_TRUE);
    GX_CopyDisp(xfb[fb], GX_TRUE);
    VIDEO_SetNextFramebuffer(xfb[fb]);
    VIDEO_Flush();
    VIDEO_WaitVSync();
}

/**
 * Call this before exiting your application.
 */
void GRRLIB_Exit() {
    GX_Flush();
    GX_AbortFrame();

    if(xfb[0] != NULL) {
        free(MEM_K1_TO_K0(xfb[0]));
        xfb[0] = NULL;
    }
    if(xfb[1] != NULL) {
        free(MEM_K1_TO_K0(xfb[1]));
        xfb[1] = NULL;
    }
    if(gp_fifo != NULL) {
        free(gp_fifo);
        gp_fifo = NULL;
    }
}
