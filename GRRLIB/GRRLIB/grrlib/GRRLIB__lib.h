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

/**
 * @file GRRLIB__lib.h
 * GRRLIB library function prototypes.
 */

#ifndef __GRRLIB_H__
#  error Do not include GRRLIB_fnLib.h directly, include only GRRLIB.h
#endif

#ifndef __GRRLIB_FNLIB_H__
#define __GRRLIB_FNLIB_H__

//==============================================================================
// Prototypes for library contained functions
//==============================================================================

//------------------------------------------------------------------------------
// GRRLIB_bmf.c - BitMapFont functions
GRRLIB_bytemapFont*  GRRLIB_LoadBMF (const u8 my_bmf[] ) ;
void                 GRRLIB_FreeBMF (const GRRLIB_bytemapFont *bmf) ;

void  GRRLIB_InitTileSet  (GRRLIB_texImg *tex,
                           const uint tilew, const uint tileh,
                           const uint tilestart) ;

//------------------------------------------------------------------------------
// GRRLIB_bmfx.c - Bitmap f/x
void  GRRLIB_BMFX_FlipH     (const GRRLIB_texImg *texsrc,
                             GRRLIB_texImg *texdest) ;

void  GRRLIB_BMFX_FlipV     (const GRRLIB_texImg *texsrc,
                             GRRLIB_texImg *texdest) ;

void  GRRLIB_BMFX_Grayscale (const GRRLIB_texImg *texsrc,
                             GRRLIB_texImg *texdest) ;

void  GRRLIB_BMFX_Sepia     (const GRRLIB_texImg *texsrc,
                             GRRLIB_texImg *texdest) ;

void  GRRLIB_BMFX_Invert    (const GRRLIB_texImg *texsrc,
                             GRRLIB_texImg *texdest) ;

void  GRRLIB_BMFX_Blur      (const GRRLIB_texImg *texsrc,
                             GRRLIB_texImg *texdest, const u32 factor) ;

void  GRRLIB_BMFX_Scatter   (const GRRLIB_texImg *texsrc,
                             GRRLIB_texImg *texdest, const u32 factor) ;

void  GRRLIB_BMFX_Pixelate  (const GRRLIB_texImg *texsrc,
                             GRRLIB_texImg *texdest, const u32 factor) ;

//------------------------------------------------------------------------------
// GRRLIB_core.c - GRRLIB core functions
int   GRRLIB_Init (void) ;
void  GRRLIB_Exit (void) ;

//------------------------------------------------------------------------------
// GRRLIB_fbAdvanced.c - Render to framebuffer: Advanced primitives
void  GRRLIB_Circle (const f32 x,  const f32 y,  const f32 radius,
                     const u32 color, const u8 filled) ;

//------------------------------------------------------------------------------
// GRRLIB_fileIO - File I/O (SD Card)
int             GRRLIB_LoadFile            (const char* filename,
                                            unsigned char* *data) ;
GRRLIB_texImg*  GRRLIB_LoadTextureFromFile (const char* filename) ;
bool            GRRLIB_ScrShot             (const char* filename) ;

//------------------------------------------------------------------------------
//! GRRLIB_print.c - Will someome please tell me what these are :)
void  GRRLIB_Printf   (const f32 xpos, const f32 ypos,
                       const GRRLIB_texImg *tex,
                       const char *text, ...) ;

void  GRRLIB_PrintBMF (const f32 xpos, const f32 ypos,
                       const GRRLIB_bytemapFont *bmf,
                       const f32 zoom, const char *text, ...) ;

//------------------------------------------------------------------------------
// GRRLIB_render.c - Rendering functions
void  GRRLIB_DrawImg  (const f32 xpos, const f32 ypos, const GRRLIB_texImg *tex) ;

void  GRRLIB_DrawImgQuad  (const guVector pos[4], GRRLIB_texImg *tex) ;

void  GRRLIB_DrawTile (const f32 xpos, const f32 ypos, const GRRLIB_texImg *tex,
                       const int frame) ;

void  GRRLIB_DrawPart (const f32 xpos, const f32 ypos, const f32 partx, const f32 party,
                       const f32 partw, const f32 parth, const GRRLIB_texImg *tex);

void  GRRLIB_DrawTileQuad (const guVector pos[4], GRRLIB_texImg *tex, const int frame) ;

void  GRRLIB_Render  (void) ;

//------------------------------------------------------------------------------
// GRRLIB_snapshot.c - Create a texture containing a snapshot of a part of the framebuffer
void  GRRLIB_Screen2Texture (int posx, int posy, GRRLIB_texImg *tex, bool clear) ;
void GRRLIB_CompoStart (void);
void GRRLIB_CompoEnd(int posx, int posy, GRRLIB_texImg *tex);
//------------------------------------------------------------------------------
// GRRLIB_texEdit.c - Modifying the content of a texture

GRRLIB_texImg*  GRRLIB_LoadTexture    (const u8 *my_img) ;
GRRLIB_texImg*  GRRLIB_LoadTexturePNG (const u8 *my_png) ;
GRRLIB_texImg*  GRRLIB_LoadTextureJPG (const u8 *my_jpg) ;
GRRLIB_texImg*  GRRLIB_LoadTextureBMP (const u8 *my_bmp) ;

#endif // __GRRLIB_FNLIB_H__
