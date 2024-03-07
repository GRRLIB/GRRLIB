/*------------------------------------------------------------------------------
Copyright (c) 2009-2024 The GRRLIB Team

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
 * Do not include GRRLIB__lib.h directly, include only GRRLIB.h.
 */
/**
 * @addtogroup AllFunc
 * @{
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
void                 GRRLIB_FreeBMF (GRRLIB_bytemapFont *bmf);

void  GRRLIB_InitTileSet  (GRRLIB_texImg *tex,
                           const u32 tilew, const u32 tileh,
                           const u32 tilestart);

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
void  GRRLIB_Ellipse (const f32 x, const f32 y, const f32 radiusX,
                      const f32 radiusY, const u32 color, const u8 filled);
void  GRRLIB_Circle (const f32 x, const f32 y, const f32 radius,
                     const u32 color, const u8 filled);

//------------------------------------------------------------------------------
// GRRLIB_fileIO - File I/O (SD Card)
int              GRRLIB_LoadFile            (const char* filename, u8* *data);
GRRLIB_texImg*   GRRLIB_LoadTextureFromFile (const char* filename);
GRRLIB_ttfFont*  GRRLIB_LoadTTFFromFile     (const char* filename);
bool             GRRLIB_ScrShot             (const char* filename);

//------------------------------------------------------------------------------
// GRRLIB_print.c - Will someone please tell me what these are :)
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
void  GRRLIB_Screen2Texture (u16 posx, u16 posy, GRRLIB_texImg *tex, bool clear);
void  GRRLIB_CompoStart (void);
void  GRRLIB_CompoEnd(u16 posx, u16 posy, GRRLIB_texImg *tex);

//------------------------------------------------------------------------------
// GRRLIB_texEdit.c - Modifying the content of a texture
GRRLIB_texImg*  GRRLIB_CreateEmptyTexture (const u32 width, const u32 height);
GRRLIB_texImg*  GRRLIB_CreateEmptyTextureFmt (const u32 width, const u32 height, const u32 format);
GRRLIB_texImg*  GRRLIB_LoadTexture    (const u8 *my_img);
GRRLIB_texImg*  GRRLIB_LoadTexturePNG (const u8 *my_png);
GRRLIB_texImg*  GRRLIB_LoadTextureJPG (const u8 *my_jpg);
GRRLIB_texImg*  GRRLIB_LoadTextureJPGEx (const u8 *my_jpg, const u32 my_size);
GRRLIB_texImg*  GRRLIB_LoadTextureBMP (const u8 *my_bmp);

//------------------------------------------------------------------------------
// GRRLIB_gecko.c - USB_Gecko output facilities
bool GRRLIB_GeckoInit(void);
void GRRLIB_GeckoPrintf (const char *text, ...);

//------------------------------------------------------------------------------
// GRRLIB_3D.c - 3D functions for GRRLIB
void GRRLIB_SetBackgroundColour(u8 r, u8 g, u8 b, u8 a);
void GRRLIB_Camera3dSettings(f32 posx, f32 posy, f32 posz, f32 upx, f32 upy, f32 upz, f32 lookx, f32 looky, f32 lookz);
void GRRLIB_3dMode(f32 minDist, f32 maxDist, f32 fov, bool texturemode, bool normalmode);
void GRRLIB_2dMode(void);
void GRRLIB_ObjectViewBegin(void);
void GRRLIB_ObjectViewScale(f32 scalx, f32 scaly, f32 scalz);
void GRRLIB_ObjectViewRotate(f32 angx, f32 angy, f32 angz);
void GRRLIB_ObjectViewTrans(f32 posx, f32 posy, f32 posz);
void GRRLIB_ObjectViewEnd(void);
void GRRLIB_ObjectView(f32 posx, f32 posy, f32 posz, f32 angx, f32 angy, f32 angz, f32 scalx, f32 scaly, f32 scalz);
void GRRLIB_ObjectViewInv(f32 posx, f32 posy, f32 posz, f32 angx, f32 angy, f32 angz, f32 scalx, f32 scaly, f32 scalz);
void GRRLIB_SetTexture(GRRLIB_texImg *tex, bool rep);
void GRRLIB_DrawTorus(f32 r, f32 R, int nsides, int rings, bool filled, u32 col);
void GRRLIB_DrawSphere(f32 r, int lats, int longs, bool filled, u32 col);
void GRRLIB_DrawCube(f32 size, bool filled, u32 col);
void GRRLIB_DrawCylinder(f32 r, f32 h, u16 d, bool filled, u32 col);
void GRRLIB_DrawCone(f32 r, f32 h, u16 d, bool filled, u32 col);
void GRRLIB_DrawTessPanel(f32 w, f32 wstep, f32 h, f32 hstep, bool filled, u32 col);
void GRRLIB_SetLightAmbient(u32 ambientcolor);
void GRRLIB_SetLightDiff(u8 num, guVector pos, f32 distattn, f32 brightness, u32 lightcolor);
void GRRLIB_SetLightSpec(u8 num, guVector dir, f32 shininess, u32 lightcolor, u32 speccolor);
void GRRLIB_SetLightSpot(u8 num, guVector pos, guVector lookat, f32 angAttn0, f32 angAttn1, f32 angAttn2, f32 distAttn0, f32 distAttn1, f32 distAttn2, u32 lightcolor);
void GRRLIB_SetLightOff(void);

//------------------------------------------------------------------------------
// GRRLIB_ttf.c - FreeType function for GRRLIB
GRRLIB_ttfFont* GRRLIB_LoadTTF(const u8* file_base, s32 file_size);
void GRRLIB_FreeTTF(GRRLIB_ttfFont *myFont);
u32 GRRLIB_PrintfTTF(int x, int y, GRRLIB_ttfFont *myFont, const char *string, unsigned int fontSize, const u32 color);
u32 GRRLIB_PrintfTTFW(int x, int y, GRRLIB_ttfFont *myFont, const wchar_t *string, unsigned int fontSize, const u32 color);
u32 GRRLIB_WidthTTF(GRRLIB_ttfFont *myFont, const char *, unsigned int);
u32 GRRLIB_WidthTTFW(GRRLIB_ttfFont *myFont, const wchar_t *, unsigned int);

#endif // __GRRLIB_FNLIB_H__
/** @} */ // end of group
