/*------------------------------------------------------------------------------
Copyright (c) 2010 The GRRLIB Team

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
#  error Do not include GRRLIB__lib.h directly, include only GRRLIB.h
#endif

#ifndef __GRRLIB_FNLIB_H__
#define __GRRLIB_FNLIB_H__

//==============================================================================
// Prototypes for library contained functions
//==============================================================================

//------------------------------------------------------------------------------
// GRRLIB_bmf.c - BitMapFont functions
GRRLIB_bytemapFont*  GRRLIB_LoadBMF (const u8 my_bmf[] );
void                 GRRLIB_FreeBMF (const GRRLIB_bytemapFont *bmf);

void  GRRLIB_InitTileSet  (GRRLIB_texImg *tex,
                           const uint tilew, const uint tileh,
                           const uint tilestart);

//------------------------------------------------------------------------------
// GRRLIB_bmfx.c - Bitmap f/x
void  GRRLIB_BMFX_FlipH     (const GRRLIB_texImg *texsrc,
                             GRRLIB_texImg *texdest);

void  GRRLIB_BMFX_FlipV     (const GRRLIB_texImg *texsrc,
                             GRRLIB_texImg *texdest);

void  GRRLIB_BMFX_Grayscale (const GRRLIB_texImg *texsrc,
                             GRRLIB_texImg *texdest);

void  GRRLIB_BMFX_Sepia     (const GRRLIB_texImg *texsrc,
                             GRRLIB_texImg *texdest);

void  GRRLIB_BMFX_Invert    (const GRRLIB_texImg *texsrc,
                             GRRLIB_texImg *texdest);

void  GRRLIB_BMFX_Blur      (const GRRLIB_texImg *texsrc,
                             GRRLIB_texImg *texdest, const u32 factor);

void  GRRLIB_BMFX_Scatter   (const GRRLIB_texImg *texsrc,
                             GRRLIB_texImg *texdest, const u32 factor);

void  GRRLIB_BMFX_Pixelate  (const GRRLIB_texImg *texsrc,
                             GRRLIB_texImg *texdest, const u32 factor);

//------------------------------------------------------------------------------
// GRRLIB_core.c - GRRLIB core functions
int   GRRLIB_Init (void);
void  GRRLIB_Exit (void);

//------------------------------------------------------------------------------
// GRRLIB_fbAdvanced.c - Render to framebuffer: Advanced primitives
void  GRRLIB_Circle (const f32 x,  const f32 y,  const f32 radius,
                     const u32 color, const u8 filled);

//------------------------------------------------------------------------------
// GRRLIB_fileIO - File I/O (SD Card)
int             GRRLIB_LoadFile            (const char* filename,
                                            unsigned char* *data);
GRRLIB_texImg*  GRRLIB_LoadTextureFromFile (const char* filename);
bool            GRRLIB_ScrShot             (const char* filename);

//------------------------------------------------------------------------------
// GRRLIB_print.c - Will someome please tell me what these are :)
void  GRRLIB_Printf   (const f32 xpos, const f32 ypos,
                       const GRRLIB_texImg *tex, const u32 color,
                       const f32 zoom, const char *text, ...);

void  GRRLIB_PrintBMF (const f32 xpos, const f32 ypos,
                       const GRRLIB_bytemapFont *bmf,
                       const char *text, ...);

//------------------------------------------------------------------------------
// GRRLIB_render.c - Rendering functions
void  GRRLIB_DrawImg  (const f32 xpos, const f32 ypos, const GRRLIB_texImg *tex,
                       const f32 degrees, const f32 scaleX, const f32 scaleY,
                       const u32 color);

void  GRRLIB_DrawImgQuad  (const guVector pos[4], GRRLIB_texImg *tex,
                           const u32 color);

void  GRRLIB_DrawTile (const f32 xpos, const f32 ypos, const GRRLIB_texImg *tex,
                       const f32 degrees, const f32 scaleX, const f32 scaleY,
                       const u32 color, const int frame);

void  GRRLIB_DrawPart (const f32 xpos, const f32 ypos, const f32 partx, const f32 party,
                       const f32 partw, const f32 parth, const GRRLIB_texImg *tex,
                       const f32 degrees, const f32 scaleX, const f32 scaleY,
                       const u32 color);

void  GRRLIB_DrawTileQuad (const guVector pos[4], GRRLIB_texImg *tex, const u32 color, const int frame);

void  GRRLIB_Render  (void);

//------------------------------------------------------------------------------
// GRRLIB_snapshot.c - Create a texture containing a snapshot of a part of the framebuffer
void  GRRLIB_Screen2Texture (int posx, int posy, GRRLIB_texImg *tex, bool clear);
void GRRLIB_CompoStart (void);
void GRRLIB_CompoEnd(int posx, int posy, GRRLIB_texImg *tex);

//------------------------------------------------------------------------------
// GRRLIB_texEdit.c - Modifying the content of a texture
GRRLIB_texImg*  GRRLIB_LoadTexture    (const u8 *my_img);
GRRLIB_texImg*  GRRLIB_LoadTexturePNG (const u8 *my_png);
GRRLIB_texImg*  GRRLIB_LoadTextureJPG (const u8 *my_jpg);
GRRLIB_texImg*  GRRLIB_LoadTextureJPGEx (const u8 *my_jpg, const int);
GRRLIB_texImg*  GRRLIB_LoadTextureBMP (const u8 *my_bmp);

//------------------------------------------------------------------------------
// GRRLIB_gecko.c - USB_Gecko output facilities
bool GRRLIB_GeckoInit();
void GRRLIB_GeckoPrintf (const char *text, ...);

//------------------------------------------------------------------------------
// GRRLIB_3D.c - 3D functions for GRRLIB
void GRRLIB_SetBackgroundColour(u8 r, u8 g, u8 b, u8 a);
void GRRLIB_Camera3dSettings(f32 posx, f32 posy, f32 posz, f32 upx, f32 upy, f32 upz, f32 lookx, f32 looky, f32 lookz);
void GRRLIB_3dMode(f32 minDist, f32 maxDist, f32 fov, bool colormode, bool texturemode, bool normalmode);
void GRRLIB_2dMode();
void GRRLIB_ObjectView(f32 posx, f32 posy, f32 posz, f32 angx, f32 angy, f32 angz,  f32 scalx, f32 scaly, f32 scalz);
void GRRLIB_SetTexture(GRRLIB_texImg *tex, bool rep);
void GRRLIB_InitLight(u8 id, guVector lpos, u32 lcol);
void GRRLIB_LightOff(void);
void GRRLIB_LightSwitch(u8 id, u32 ambcol, u32 matcol, u8 colsrc);
void GRRLIB_DrawTorus(f32 r, f32 R, int nsides, int rings, bool filled);
void GRRLIB_DrawSphere(f32 r, int lats, int longs, bool filled);
void GRRLIB_DrawCube(f32 size, bool filled);
void GRRLIB_DrawCylinder(f32 r, f32 h, int d, bool filled);

//------------------------------------------------------------------------------
// GRRLIB_Freetype.c - FreeType function for GRRLIB
GRRLIB_ttfFont* GRRLIB_LoadTTF(const u8* file_base, s32 file_size);
void GRRLIB_FreeTTF(GRRLIB_ttfFont *myFont);
void GRRLIB_PrintfTTF(int x, int y, GRRLIB_ttfFont *myFont, const char *string, unsigned int fontSize, const u32 color);
void GRRLIB_PrintfTTFW(int x, int y, GRRLIB_ttfFont *myFont, const wchar_t *string, unsigned int fontSize, const u32 color);
unsigned int GRRLIB_WidthTTF(GRRLIB_ttfFont *myFont, const char *, unsigned int);
unsigned int GRRLIB_WidthTTFW(GRRLIB_ttfFont *myFont, const wchar_t *, unsigned int);

#endif // __GRRLIB_FNLIB_H__
