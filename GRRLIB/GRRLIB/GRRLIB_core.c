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

#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <ogc/conf.h>
#include <fat.h>

#define __GRRLIB_CORE__
#include <grrlib.h>

#define DEFAULT_FIFO_SIZE (256 * 1024) /**< GX fifo buffer size. */

GRRLIB_drawSettings  GRRLIB_Settings;
Mtx                  GXmodelView2D;

static void  *gp_fifo = NULL;

static bool  is_setup = false;  // To control entry and exit

/**
 * Initialize GRRLIB. Call this at the beginning your code.
 * @return int 0=OK; -1=NoMemory; -2=NoFilingSystem
 * @see GRRLIB_Exit
 */
int  GRRLIB_Init (void) {
    f32 yscale;
    u32 xfbHeight;
    Mtx44 perspective;

    // Ensure this function is only ever called once
    if (is_setup)  return 0 ;

    // Initialise the video subsystem
    VIDEO_Init();
    VIDEO_SetBlack(true);  // Disable video output during initialisation

    // Grab a pointer to the video mode attributes
    if ( !(rmode = VIDEO_GetPreferredMode(NULL)) )  return -1 ;

    // Video Mode Correction
    switch (rmode->viTVMode) {
        case VI_DEBUG_PAL:  // PAL 50hz 576i
            //rmode = &TVPal574IntDfScale;
            rmode = &TVPal528IntDf; //! BC ...this is still wrong, but "less bad" for now
            break;
    }

    // 16:9 and 4:3 Screen Adjustment
    if (CONF_GetAspectRatio() == CONF_ASPECT_16_9) {
        rmode->viWidth = 678;
        rmode->viXOrigin = (VI_MAX_WIDTH_NTSC - 678)/2;  //! This probably needs to consider PAL
    } else {    // 4:3
        rmode->viWidth = 672;
        rmode->viXOrigin = (VI_MAX_WIDTH_NTSC - 672)/2;
    }

    // --
    VIDEO_Configure(rmode);

    // Get some memory to use for a "double buffered" frame buffer
    if ( !(xfb[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode))) )  return -1 ;
    if ( !(xfb[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode))) )  return -1 ;

    VIDEO_SetNextFramebuffer(xfb[fb]);  // Choose a frame buffer to start with

    VIDEO_Flush();                      // flush the frame to the TV
    VIDEO_WaitVSync();                  // Wait for the TV to finish updating
    // If the TV image is interlaced it takes two passes to display the image
    if (rmode->viTVMode & VI_NON_INTERLACE)  VIDEO_WaitVSync() ;

    // The FIFO is the buffer the CPU uses to send commands to the GPU
    if ( !(gp_fifo = memalign(32, DEFAULT_FIFO_SIZE)) )  return -1 ;
    memset(gp_fifo, 0, DEFAULT_FIFO_SIZE);
    GX_Init(gp_fifo, DEFAULT_FIFO_SIZE);

    // Clear the background to opaque black and clears the z-buffer
    GX_SetCopyClear((GXColor){ 0, 0, 0, 0xff }, GX_MAX_Z24);

    // Other GX setup
    yscale    = GX_GetYScaleFactor(rmode->efbHeight, rmode->xfbHeight);
    xfbHeight = GX_SetDispCopyYScale(yscale);
    GX_SetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
    GX_SetDispCopyDst(rmode->fbWidth, xfbHeight);
    GX_SetCopyFilter(rmode->aa, rmode->sample_pattern, GX_TRUE, rmode->vfilter);
    GX_SetFieldMode(rmode->field_rendering, ((rmode->viHeight == 2 * rmode->xfbHeight) ? GX_ENABLE : GX_DISABLE));

    if (rmode->aa)  GX_SetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR) ;  // Set 16 bit RGB565
    else            GX_SetPixelFmt(GX_PF_RGB8_Z24  , GX_ZC_LINEAR) ;  // Set 24 bit Z24

    GX_SetDispCopyGamma(GX_GM_1_0);

    // Setup the vertex descriptor
    GX_ClearVtxDesc();      // clear all the vertex descriptors
    GX_InvVtxCache();       // Invalidate the vertex cache
    GX_InvalidateTexAll();  // Invalidate all textures

    // Tells the flipper to expect direct data
    GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);
    GX_SetVtxDesc(GX_VA_POS,  GX_DIRECT);
    GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);

    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS,  GX_POS_XYZ,  GX_F32, 0);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST,   GX_F32, 0);
    // Colour 0 is 8bit RGBA format
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GX_SetZMode(GX_FALSE, GX_LEQUAL, GX_TRUE);

    GX_SetNumChans(1);    // colour is the same as vertex colour
    GX_SetNumTexGens(1);  // One texture exists
    GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

    guMtxIdentity(GXmodelView2D);
    guMtxTransApply(GXmodelView2D, GXmodelView2D, 0.0F, 0.0F, -100.0F);
    GX_LoadPosMtxImm(GXmodelView2D, GX_PNMTX0);

    guOrtho(perspective, 0, rmode->efbHeight, 0, rmode->fbWidth, 0, 1000.0f);
    GX_LoadProjectionMtx(perspective, GX_ORTHOGRAPHIC);

    GX_SetViewport(0, 0, rmode->fbWidth, rmode->efbHeight, 0, 1);
    GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GX_SetAlphaUpdate(GX_TRUE);
    GX_SetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GX_SetColorUpdate(GX_ENABLE);
    GX_SetCullMode(GX_CULL_NONE);
    GRRLIB_ClipReset();

    // Default settings
    GRRLIB_Settings.antialias = true;
    GRRLIB_Settings.blend     = GRRLIB_BLEND_ALPHA;

    // Schedule cleanup for when program exits
    is_setup = true;
    atexit(GRRLIB_Exit);

    // Initialise the filing system
    if (!fatInitDefault())  return -2 ;

    VIDEO_SetBlack(false);  // Enable video output
    return 0;
}

/**
 * Call this before exiting your application.
 */
void  GRRLIB_Exit (void) {

    // Ensure this function is only ever called once
    // ...and only if the setup function has been called
    static  bool  done = false;
    if (done || !is_setup)  return ;
    else                    done = true ;

    // Allow write access to the full screen
    GX_SetClipMode( GX_CLIP_DISABLE );
    GX_SetScissor( 0, 0, rmode->fbWidth, rmode->efbHeight );

    // We empty both frame buffers on our way out
    // otherwise dead frames are sometimes seen when starting the next app
    GRRLIB_FillScreen( 0x000000FF );  GRRLIB_Render();
    GRRLIB_FillScreen( 0x000000FF );  GRRLIB_Render();

    // Shut down the GX engine
    GX_DrawDone();
    GX_AbortFrame();

    // Free up memory allocated for frame buffers & FIFOs
    if (xfb[0]  != NULL) {  free(MEM_K1_TO_K0(xfb[0]));  xfb[0]  = NULL;  }
    if (xfb[1]  != NULL) {  free(MEM_K1_TO_K0(xfb[1]));  xfb[1]  = NULL;  }
    if (gp_fifo != NULL) {  free(gp_fifo);               gp_fifo = NULL;  }
}
