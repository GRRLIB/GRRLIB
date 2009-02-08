/*===========================================
        GRRLIB (GX version) 3.0.5 alpha
        Code     : NoNameNo
        Additional Code : Crayon
        GX hints : RedShade
===========================================*/

#ifndef __GXHDR__
#define __GXHDR__

/** 
 * @file GRRLIB.h
 * GRRLIB library.
 */

#include <gccore.h>

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

/**
 * Structure to hold the texture informations.
 */
typedef struct GRRLIB_texImg{
    unsigned int w;         /**< width of the texture. */
    unsigned int h;         /**< height of the texture. */
    unsigned int tilew;     /**< tilew widht of a tile. */
    unsigned int tileh;     /**< tileh height of a tile. */
    unsigned int nbtilew;   /**< nbtilew number of tiles for the x axis. */
    unsigned int nbtileh;   /**< nbtileh number of tiles for the y axis. */
    unsigned int tilestart; /**<  */
    void *data;             /**< pointer to the texture data. */
} GRRLIB_texImg;

extern Mtx GXmodelView2D;

inline void GRRLIB_FillScreen(u32 color);

inline void GRRLIB_Plot(f32 x, f32 y, u32 color);
void GRRLIB_NPlot(Vector v[], u32 color, long n);

inline void GRRLIB_Line(f32 x1, f32 y1, f32 x2, f32 y2, u32 color);

inline void GRRLIB_Rectangle(f32 x, f32 y, f32 width, f32 height, u32 color, u8 filled);
void GRRLIB_NGone(Vector v[], u32 color, long n);
void GRRLIB_NGoneFilled(Vector v[], u32 color, long n);

GRRLIB_texImg GRRLIB_CreateEmptyTexture(unsigned int, unsigned int);
GRRLIB_texImg GRRLIB_LoadTexturePNG(const unsigned char my_png[]);
GRRLIB_texImg GRRLIB_LoadTextureJPG(const unsigned char my_jpg[]);


void GRRLIB_InitTileSet(struct GRRLIB_texImg *tex, unsigned int tilew, unsigned int tileh, unsigned int tilestart);

inline void GRRLIB_DrawImg(f32 xpos, f32 ypos, GRRLIB_texImg tex, float degrees, float scaleX, f32 scaleY, u32 color );
inline void GRRLIB_DrawTile(f32 xpos, f32 ypos, GRRLIB_texImg tex, float degrees, float scaleX, f32 scaleY, u32 color, int frame);

void GRRLIB_Printf(f32 xpos, f32 ypos, GRRLIB_texImg tex, u32 color, f32 zoom, const char *text, ...);

bool GRRLIB_PtInRect(int hotx, int hoty, int hotw, int hoth, int wpadx, int wpady);
bool GRRLIB_RectInRect(int rect1x, int rect1y, int rect1w, int rect1h, int rect2x, int rect2y, int rect2w, int rect2h);
bool GRRLIB_RectOnRect(int rect1x, int rect1y, int rect1w, int rect1h, int rect2x, int rect2y, int rect2w, int rect2h);

u32 GRRLIB_GetPixelFromtexImg(int x, int y, GRRLIB_texImg tex);
void GRRLIB_SetPixelTotexImg(int x, int y, GRRLIB_texImg tex, u32 color);

void GRRLIB_FlushTex(GRRLIB_texImg tex);

void GRRLIB_BMFX_GrayScale(GRRLIB_texImg texsrc, GRRLIB_texImg texdest);
void GRRLIB_BMFX_Scatter(GRRLIB_texImg texsrc, GRRLIB_texImg texdest, int factor);

void GRRLIB_GXEngine(Vector v[], u32 color, long count, u8 fmt);


void GRRLIB_Init();

void GRRLIB_Render();

void GRRLIB_Exit();

#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif

/**
 * @mainpage GRRLIB Documentation
 * @image html grrlib_logo.png
 * Welcome to the GRRLIB documentation.
 */
