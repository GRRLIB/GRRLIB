/*===========================================
        GRRLIB (GX version) 3.0.5 alpha
        Code     : NoNameNo
        Additional Code : Crayon
        GX hints : RedShade
===========================================*/

#ifndef __GXHDR__
#define __GXHDR__

#include <gccore.h>


/**
 * @struct GRRLIB_texImg
 * @brief Structure to hold the texture informations.
 * @param w width of the texture.
 * @param h height of the texture.
 * @param tilew widht of a tile.
 * @param tileh height of a tile.
 * @param nbtilew number of tiles for the x axis.
 * @param nbtileh number of tiles for the y axis.
 * @param tilestart
 * @param data pointer to the texture data.
 */
typedef struct GRRLIB_texImg{
    unsigned int w;
    unsigned int h;
    unsigned int tilew;
    unsigned int tileh;
    unsigned int nbtilew;
    unsigned int nbtileh;
    unsigned int tilestart;
    void *data;
} GRRLIB_texImg;

extern Mtx GXmodelView2D;

inline void GRRLIB_FillScreen(u32 color);

inline void GRRLIB_Plot(f32 x, f32 y, u32 color);
void GRRLIB_NPlot(Vector v[], u32 color, long n);

inline void GRRLIB_Line(f32 x1, f32 y1, f32 x2, f32 y2, u32 color);

inline void GRRLIB_Rectangle(f32 x, f32 y, f32 width, f32 height, u32 color, u8 filled);
void GRRLIB_NGone(Vector v[], u32 color, long n);
void GRRLIB_NGoneFilled(Vector v[], u32 color, long n);


GRRLIB_texImg GRRLIB_LoadTexturePNG(const unsigned char my_png[]);
GRRLIB_texImg GRRLIB_LoadTextureJPG(const unsigned char my_jpg[]);


void GRRLIB_InitTileSet(struct GRRLIB_texImg *tex, unsigned int tilew, unsigned int tileh, unsigned int tilestart);

inline void GRRLIB_DrawImg(f32 xpos, f32 ypos, GRRLIB_texImg tex, float degrees, float scaleX, f32 scaleY, u32 color );
inline void GRRLIB_DrawTile(f32 xpos, f32 ypos, GRRLIB_texImg tex, float degrees, float scaleX, f32 scaleY, u32 color, int frame);

void GRRLIB_Printf(f32 xpos, f32 ypos, GRRLIB_texImg tex, u32 color, f32 zoom, char *text,...);


void GRRLIB_GXEngine(Vector v[], u32 color, long count, u8 fmt);


void GRRLIB_InitVideo ();
void GRRLIB_Start();
void GRRLIB_Render ();


#endif
