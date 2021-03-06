/*------------------------------------------------------------------------------
Copyright (c) 2009-2021 The GRRLIB Team

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
 * @file grrlib.h
 * GRRLIB user include file.
 */
/**
 * @defgroup AllFunc Everything in GRRLIB
 * This is the complete list of functions, structures, defines, typedefs, enumerations and variables you may want to used to make your homebrew with GRRLIB.
 * You simply need to include grrlib.h in your project to have access to all of these.
 * @{
 */

#ifndef __GRRLIB_H__
#define __GRRLIB_H__

/**
 * Version information for GRRLIB.
 */
#define GRRLIB_VER_STRING "4.4.1"

//==============================================================================
// Includes
//==============================================================================
#include <gccore.h>
//==============================================================================

//==============================================================================
// C++ header
//==============================================================================
#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

//==============================================================================
// Primitive colour macros
//==============================================================================
#define R(c)  (((c) >>24) &0xFF)  /**< Extract red component of colour. */
#define G(c)  (((c) >>16) &0xFF)  /**< Extract green component of colour. */
#define B(c)  (((c) >> 8) &0xFF)  /**< Extract blue component of colour. */
#define A(c)  ( (c)       &0xFF)  /**< Extract alpha component of colour. */

/**
 * Build an RGB pixel from components.
 * @param r Red component.
 * @param g Green component.
 * @param b Blue component.
 * @param a Alpha component.
 */
#define RGBA(r,g,b,a) ( (u32)( ( ((u32)(r))        <<24) |  \
                               ((((u32)(g)) &0xFF) <<16) |  \
                               ((((u32)(b)) &0xFF) << 8) |  \
                               ( ((u32)(a)) &0xFF      ) ) )

//==============================================================================
// typedefs, enumerators & structs
//==============================================================================
/**
 * GRRLIB Blending Modes.
 */
typedef  enum GRRLIB_blendMode {
    GRRLIB_BLEND_ALPHA  = 0,    /**< Alpha Blending. */
    GRRLIB_BLEND_ADD    = 1,    /**< Additive Blending. */
    GRRLIB_BLEND_SCREEN = 2,    /**< Alpha Light Blending. */
    GRRLIB_BLEND_MULTI  = 3,    /**< Multiply Blending. */
    GRRLIB_BLEND_INV    = 4,    /**< Invert Color Blending. */
} GRRLIB_blendMode;

#define GRRLIB_BLEND_NONE   (GRRLIB_BLEND_ALPHA)    /**< Alias for GRRLIB_BLEND_ALPHA. */
#define GRRLIB_BLEND_LIGHT  (GRRLIB_BLEND_ADD)      /**< Alias for GRRLIB_BLEND_ADD. */
#define GRRLIB_BLEND_SHADE  (GRRLIB_BLEND_MULTI)    /**< Alias for GRRLIB_BLEND_MULTI. */

//------------------------------------------------------------------------------
/**
 * Structure to hold the current drawing settings.
 */
typedef  struct GRRLIB_drawSettings {
    bool              antialias;    /**< AntiAlias is enabled when set to true. */
    GRRLIB_blendMode  blend;        /**< Blending Mode.                         */
    int               lights;       /**< Active lights.                         */
} GRRLIB_drawSettings;

//------------------------------------------------------------------------------
/**
 * Structure to hold the texture information.
 */
typedef  struct GRRLIB_texImg {
    u32    w;           /**< The width of the texture in pixels.  */
    u32    h;           /**< The height of the texture in pixels. */
    int    handlex;     /**< Texture handle x. */
    int    handley;     /**< Texture handle y. */
    int    offsetx;     /**< Texture offset x. */
    int    offsety;     /**< Texture offset y. */

    bool   tiledtex;    /**< Texture is tiled if set to true.   */
    u32    tilew;       /**< The width of one tile in pixels.   */
    u32    tileh;       /**< The height of one tile in pixels.  */
    u32    nbtilew;     /**< Number of tiles for the x axis.    */
    u32    nbtileh;     /**< Number of tiles for the y axis.    */
    u32    tilestart;   /**< Offset to tile starting position.  */
    f32    ofnormaltexx;/**< Offset of normalized texture on x. */
    f32    ofnormaltexy;/**< Offset of normalized texture on y. */

    void  *data;        /**< Pointer to the texture data. */
} GRRLIB_texImg;

//------------------------------------------------------------------------------
/**
 * Structure to hold the bytemap character information.
 */
typedef  struct GRRLIB_bytemapChar {
    u8  width;          /**< Character width.    */
    u8  height;         /**< Character height.   */
    s8  relx;           /**< Horizontal offset relative to cursor (-128 to 127).            */
    s8  rely;           /**< Vertical offset relative to cursor (-128 to 127).              */
    u8  kerning;        /**< Kerning (Horizontal cursor shift after drawing the character). */
    u8  *data;          /**< Character data (uncompressed, 8 bits per pixel).               */
} GRRLIB_bytemapChar;

//------------------------------------------------------------------------------
/**
 * Structure to hold the bytemap font information.
 */
typedef  struct GRRLIB_bytemapFont {
    char  *name;                /**< Font name.                      */
    u32   *palette;             /**< Font palette.                   */
    u16   nbChar;               /**< Number of characters in font.   */
    u8    version;              /**< Version.                        */
    s8    tracking;             /**< Tracking (Add-space after each char) (-128 to 127). */

    GRRLIB_bytemapChar charDef[256];   /**< Array of bitmap characters. */
} GRRLIB_bytemapFont;

//------------------------------------------------------------------------------
/**
 * Structure to hold the TTF information.
 */
typedef  struct GRRLIB_Font {
    void *face;     /**< A TTF face object. */
    bool kerning;   /**< true whenever a face object contains kerning data that can be accessed with FT_Get_Kerning. */
} GRRLIB_ttfFont;

//==============================================================================
// Allow general access to screen and frame information
//==============================================================================
#if defined __GRRLIB_CORE__
# define GRR_EXTERN
# define GRR_INIT(v)     = v
# define GRR_INITS(...)  = { __VA_ARGS__ }
#else
# define GRR_EXTERN      extern
# define GRR_INIT(v)
# define GRR_INITS(...)
#endif

GRR_EXTERN  GXRModeObj  *rmode; /**< Video mode. */
GRR_EXTERN  void        *xfb[2]  GRR_INITS(NULL, NULL);
GRR_EXTERN  u32         fb       GRR_INIT(0);
//==============================================================================
// procedure and function prototypes
// Inline function handling
//==============================================================================
#include "grrlib/GRRLIB__lib.h"

#if defined __GRRLIB_CORE__
#  define INLINE
#else
#  define INLINE static inline
#endif
#include "grrlib/GRRLIB__inline.h"

//==============================================================================
// C++ footer
//==============================================================================
#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif // __GRRLIB_H__
/** @} */ // end of group
