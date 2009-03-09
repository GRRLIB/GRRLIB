/*===========================================
        GRRLIB (GX version) 4.0.0
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
 * GRRLIB Blending Modes
 */
#define GRRLIB_BLEND_ALPHA    0        /**< Alpha Blending */
#define GRRLIB_BLEND_ADD    1        /**< Additive Blending */
#define GRRLIB_BLEND_SUB    2        /**< Subtractive Blending */
#define GRRLIB_BLEND_INV    3        /**< Invertive Blending */
#define GRRLIB_BLEND_NONE    GRRLIB_BLEND_ALPHA
#define GRRLIB_BLEND_LIGHT    GRRLIB_BLEND_ADD
#define GRRLIB_BLEND_SHADE    GRRLIB_BLEND_SUB


/**
 * Structure to hold the current drawing settings.
 */
typedef struct GRRLIB_drawSettings {
    bool antialias;         /**< Flag for AntiAlias On/Off. */
    unsigned char blend;    /**< Blending Mode */
} GRRLIB_drawSettings;

/**
 * Structure to hold the texture informations.
 */
typedef struct GRRLIB_texImg {
    unsigned int w;         /**< Width of the texture. */
    unsigned int h;         /**< Height of the texture. */
    int handlex;            /**< Handle x of the texture. */
    int handley;            /**< Handle y of the texture. */
    int offsetx;            /**< Offset x of the texture. */
    int offsety;            /**< Offset y of the texture. */
    bool tiledtex;          /**< Is it a tiled texture? */
    unsigned int tilew;     /**< Widht of a tile. */
    unsigned int tileh;     /**< Height of a tile. */
    unsigned int nbtilew;   /**< Number of tiles for the x axis. */
    unsigned int nbtileh;   /**< Number of tiles for the y axis. */
    unsigned int tilestart; /**< Offset for starting position. */
    void *data;             /**< Pointer to the texture data. */
} GRRLIB_texImg;

/**
 * Structure to hold the bytemap character informations.
 */
typedef struct GRRLIB_bytemapChar {
    u8 character;    /**< Which character. */
    u8 width;        /**< Character width. */
    u8 height;       /**< Character height. */
    s8 relx;         /**< Horizontal offset according to cursor (-128 to 127). */
    s8 rely;         /**< Vertical offset according to cursor (-128 to 127). */
    u8 shift;        /**< Horizontal cursor shift after drawing the character. */
    u8 *data;        /**< Character data itself (uncompressed, 8 bits per pixel). */
} GRRLIB_bytemapChar;

/**
 * Structure to hold the bytemap font informations.
 */
typedef struct GRRLIB_bytemapFont {
    u8 version;                     /**< Version. */
    s8 addSpace;                    /**< Add-space after each char (-128 to 127). */
    u32 *palette;                   /**< Font palette. */
    char *name;                     /**< Font name. */
    u16 nbChar;                     /**< Number of characters in font. */
    GRRLIB_bytemapChar *charDef;    /**< List of bitmap character definitions. */
} GRRLIB_bytemapFont;

/**
 * Linked List to inherit all members of an GRRLIB struct.
 */
typedef struct GRRLIB_linkedList {
    GRRLIB_texImg *texture;             /**< GRRLIB Texture */
    int Num;
    struct GRRLIB_linkedList *next;     /**< Pointer to next LinkedList */
} GRRLIB_linkedList;


extern Mtx GXmodelView2D;

void GRRLIB_SetAntiAliasing(bool aa);
bool GRRLIB_GetAntiAliasing();

inline void GRRLIB_SetBlend( unsigned char blendmode );
unsigned char GRRLIB_GetBlend();


inline void GRRLIB_FillScreen(u32 color);

inline void GRRLIB_Plot(f32 x, f32 y, u32 color);
void GRRLIB_NPlot(Vector v[], u32 color[], long n);

inline void GRRLIB_Line(f32 x1, f32 y1, f32 x2, f32 y2, u32 color);

inline void GRRLIB_Rectangle(f32 x, f32 y, f32 width, f32 height, u32 color, u8 filled);
inline void GRRLIB_Circle(f32 x, f32 y, f32 radius, u32 color, u8 filled);
void GRRLIB_NGone(Vector v[], u32 color[], long n);
void GRRLIB_NGoneFilled(Vector v[], u32 color[], long n);

GRRLIB_texImg GRRLIB_CreateEmptyTexture(unsigned int, unsigned int);
GRRLIB_texImg GRRLIB_LoadTexture(const unsigned char my_img[]);
GRRLIB_texImg GRRLIB_LoadTextureJPG(const unsigned char my_jpg[]);
GRRLIB_texImg GRRLIB_LoadTexturePNG(const unsigned char my_png[]);

GRRLIB_bytemapFont GRRLIB_LoadBMF(const unsigned char my_bmf[]);
void GRRLIB_FreeBMF(GRRLIB_bytemapFont bmf);

void GRRLIB_InitTileSet(struct GRRLIB_texImg *tex, unsigned int tilew, unsigned int tileh, unsigned int tilestart);

inline void GRRLIB_DrawImg(f32 xpos, f32 ypos, struct GRRLIB_texImg tex, float degrees, float scaleX, f32 scaleY, u32 color );
inline void GRRLIB_DrawImgQuad(Vector pos[4], struct GRRLIB_texImg tex, u32 color);
inline void GRRLIB_DrawTile(f32 xpos, f32 ypos, struct GRRLIB_texImg tex, float degrees, float scaleX, f32 scaleY, u32 color, int frame);

void GRRLIB_Printf(f32 xpos, f32 ypos, struct GRRLIB_texImg tex, u32 color, f32 zoom, const char *text, ...);
void GRRLIB_PrintBMF(f32 xpos, f32 ypos, struct GRRLIB_bytemapFont bmf, f32 zoom, const char *text, ...);

bool GRRLIB_PtInRect(int hotx, int hoty, int hotw, int hoth, int wpadx, int wpady);
bool GRRLIB_RectInRect(int rect1x, int rect1y, int rect1w, int rect1h, int rect2x, int rect2y, int rect2w, int rect2h);
bool GRRLIB_RectOnRect(int rect1x, int rect1y, int rect1w, int rect1h, int rect2x, int rect2y, int rect2w, int rect2h);

void GRRLIB_ClipDrawing( int x, int y, int width, int height );
void GRRLIB_ClipReset();

void GRRLIB_SetHandle(struct GRRLIB_texImg * tex, int x, int y);
void GRRLIB_SetMidHandle( struct GRRLIB_texImg * tex, bool enabled );

u32 GRRLIB_GetPixelFromtexImg(int x, int y, struct GRRLIB_texImg tex);
void GRRLIB_SetPixelTotexImg(int x, int y, struct GRRLIB_texImg tex, u32 color);

void GRRLIB_FlushTex(struct GRRLIB_texImg tex);

void GRRLIB_BMFX_Grayscale(struct GRRLIB_texImg texsrc, GRRLIB_texImg texdest);
void GRRLIB_BMFX_Invert(struct GRRLIB_texImg texsrc, GRRLIB_texImg texdest);
void GRRLIB_BMFX_FlipH(struct GRRLIB_texImg texsrc, GRRLIB_texImg texdest);
void GRRLIB_BMFX_FlipV(struct GRRLIB_texImg texsrc, GRRLIB_texImg texdest);
void GRRLIB_BMFX_Blur(struct GRRLIB_texImg texsrc, GRRLIB_texImg texdest, int factor);
void GRRLIB_BMFX_Scatter(struct GRRLIB_texImg texsrc, GRRLIB_texImg texdest, int factor);
void GRRLIB_BMFX_Pixelate(struct GRRLIB_texImg texsrc, GRRLIB_texImg texdest, int factor);

void GRRLIB_GXEngine(Vector v[], u32 color[], long count, u8 fmt);


void GRRLIB_Init();

void GRRLIB_Render();

void GRRLIB_Exit();

bool GRRLIB_ScrShot(const char*);

void GRRLIB_ListAddTexture( struct GRRLIB_texImg *img );
int GRRLIB_ListDelTexture( struct GRRLIB_texImg *img );
void GRRLIB_ListRemove( struct GRRLIB_linkedList **list );
unsigned int GRRLIB_ListGetTextureEntries();


#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif

/**
 * @mainpage GRRLIB Documentation
 * @image html grrlib_logo.png
 * Welcome to the GRRLIB documentation.
 */
