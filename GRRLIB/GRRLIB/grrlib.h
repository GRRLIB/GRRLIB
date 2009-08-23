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
 * @file GRRLIB.h
 * GRRLIB user include file
 */

#ifndef __GRRLIB_H__
#define __GRRLIB_H__

#define __HAVE_GRRLIB__

//==============================================================================
// Includes
//==============================================================================
#include <gccore.h>

//==============================================================================
/*
*** Temporary fix ***
After the release of libogc v1.7.1a, Vector was renamed to guVector
"to avoid collisions" [svn 3650]
...also Quaternion was renamed to guQuaternion - but GRRLIB does not use these!

The main codebase of GRRLIB has been updated to reflect this change.
But until the new libogc is officially released,
if you are using a version of libogc later than v1.7.1a/svn3649,
you will need to add:
  -DNOGUFIX
to the compiler flags in your makefile

When the libogc changes are officially released,
this *temporary fix* should be removed
The requirement for -DNOGUFIX will be deprecated,
but its lingering presence will not be a hinderance

Thanks to Nicksasa for reporting this problem :)
*/
//==============================================================================
#ifndef NOGUFIX
// If you are getting a compile error here, read the above comment!
typedef  Vector      guVector ;
typedef  Quaternion  guQuaternion ;
#endif

//==============================================================================
// C++ header
//==============================================================================
#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

//==============================================================================
// Extra standard declarations
//==============================================================================
typedef  unsigned int  uint;

//==============================================================================
// Primitive colour macros
//==============================================================================
// Feel free to convert these to inline functions if it floats your boat
#define R(c)  (((c) >>24) &0xFF)  /**< Exract RED   component of colour. */
#define G(c)  (((c) >>16) &0xFF)  /**< Exract GREEN component of colour. */
#define B(c)  (((c) >> 8) &0xFF)  /**< Exract BLUE  component of colour. */
#define A(c)  ( (c)       &0xFF)  /**< Exract ALPHA component of colour. */

/**
 * Build an RGB pixel from components.
 */
#define RGBA(r,g,b,a) ( (u32)( ( ((u32)(r))        <<24) |  \
                               ((((u32)(g)) &0xFF) <<16) |  \
                               ((((u32)(b)) &0xFF) << 8) |  \
                               ( ((u32)(a)) &0xFF      ) ) )

//==============================================================================
// typedefs, enumerators & structs
//==============================================================================
/**
 * Compositions Modes.
 */
typedef  enum Composition_Modes {
    GRRLIB_COMPOSE_NORMAL,      /**< NORMAL : a-over-b alpha composition (normal) */
} GRRLIB_ComposeMode;

//------------------------------------------------------------------------------
/**
 * GRRLIB Blending Modes.
 */
typedef  enum GRRLIB_blendMode {
    GRRLIB_BLEND_ALPHA  = 0,    /**< Alpha Blending.        */
    GRRLIB_BLEND_ADD    = 1,    /**< Additive Blending.     */
    GRRLIB_BLEND_SCREEN = 2,    /**< Alpha Light Blending.  */
    GRRLIB_BLEND_MULTI  = 3,    /**< Multiply Blending.     */
    GRRLIB_BLEND_INV    = 4,    /**< Invert Color Blending. */
} GRRLIB_blendMode;

// Blending mode aliases
#define GRRLIB_BLEND_NONE   (GRRLIB_BLEND_ALPHA)
#define GRRLIB_BLEND_LIGHT  (GRRLIB_BLEND_ADD)
#define GRRLIB_BLEND_SHADE  (GRRLIB_BLEND_MULTI)

//------------------------------------------------------------------------------
/**
 * Structure to hold the current drawing settings.
 */
typedef  struct GRRLIB_drawSettings {
    bool  antialias;    /**< true => AntiAlias enabled. */
    int   blend;        /**< Blending Mode.             */
} GRRLIB_drawSettings;

//------------------------------------------------------------------------------
/**
 * Structure to hold the texture informations.
 */
typedef  struct GRRLIB_texImg {
    uint   w;           /**< Texture Width.    */
    uint   h;           /**< Texture Height.   */
    int    handlex;     /**< Texture Handle x. */
    int    handley;     /**< Texture Handle y. */
    int    offsetx;     /**< Texture Offset x. */
    int    offsety;     /**< Texture Offset y. */

    bool   tiledtex;    /**< true => Texture is tiled.         */
    uint   tilew;       /**< Width of one tile.                */
    uint   tileh;       /**< Height of one tile.               */
    uint   nbtilew;     /**< Number of tiles for the x axis.   */
    uint   nbtileh;     /**< Number of tiles for the y axis.   */
    uint   tilestart;   /**< Offset to tile starting position. */

    void  *data;        /**< Pointer to the texture data. */
} GRRLIB_texImg;

//------------------------------------------------------------------------------
/**
 * Structure to hold the bytemap character informations.
 */
typedef  struct GRRLIB_bytemapChar {
    u8  character;      /**< Character identity. */
    u8  width;          /**< Character width.    */
    u8  height;         /**< Character height.   */
    s8  relx;           /**< Horizontal offset relative to cursor (-128 to 127).            */
    s8  rely;           /**< Vertical   offset relative to cursor (-128 to 127).            */
    u8  kerning;        /**< Kerning (Horizontal cursor shift after drawing the character). */
    u8  *data;          /**< Character data (uncompressed, 8 bits per pixel).               */
} GRRLIB_bytemapChar;

//------------------------------------------------------------------------------
/**
 * Structure to hold the bytemap font informations.
 */
typedef
    struct GRRLIB_bytemapFont {
        char  *name;                /**< Font name.                      */
        u32   *palette;             /**< Font palette.                   */
        u16   nbChar;               /**< Number of characters in font.   */
        u8    version;              /**< Version.                        */
        s8    tracking;             /**< Tracking (Add-space after each char) (-128 to 127). */

        GRRLIB_bytemapChar  *charDef;   /**< Array of bitmap characters. */
    }
GRRLIB_bytemapFont;

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

GRR_EXTERN  GXRModeObj  *rmode;
GRR_EXTERN  void        *xfb[2]  GRR_INITS(NULL, NULL) ;
GRR_EXTERN  u32         fb       GRR_INIT(0) ;

//==============================================================================
// procedure and function prototypes
// Inline function handling - http://www.greenend.org.uk/rjk/2003/03/inline.html
//==============================================================================
#include "grrlib/GRRLIB__lib.h"

#if defined __GRRLIB_CORE__
#  define INLINE
#else
# if __GNUC__ && !__GNUC_STDC_INLINE__
#  define INLINE extern inline
# else
#  define INLINE inline
# endif
#endif
#include "grrlib/GRRLIB__inline.h"

//==============================================================================
// C++ footer
//==============================================================================
#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif // __GRRLIB_H__

/**
 * @mainpage GRRLIB Documentation
 * @image html grrlib_logo.png
 * Welcome to the GRRLIB documentation.
 *
 * @section Introduction
 * GRRLIB is a C/C++ 2D Graphics library for Wii application developers.
 * It is essentially a wrapper which presents a friendly interface to the Nintendo GX core.
 *
 * @section Credits
 * Project Leader : NoNameNo\n
 * Documentation  : Crayon, BlueChip\n
 * Lead Coder     : NoNameNo\n
 * Support Coders : Crayon, Xane, DragonMinded, BlueChip\n
 * Advisors       : RedShade, JESPA\n
 *
 * @section Licence
 * Copyright (c) 2009 The GRRLIB Team
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
