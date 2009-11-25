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
 * @file GRRLIB_settings.h
 * Inline functions for configuring the GRRLIB settings.
 */

#include <grrlib.h>

extern  GRRLIB_drawSettings  GRRLIB_Settings;

/**
 * Set a blending mode.
 * @param blendmode The blending mode to use (Default: GRRLIB_BLEND_ALPHA).
 */
INLINE
void  GRRLIB_SetBlend (const GRRLIB_blendMode blendmode) {
    GRRLIB_Settings.blend = blendmode;
    switch (GRRLIB_Settings.blend) {
        case GRRLIB_BLEND_ALPHA:
            GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
            break;
        case GRRLIB_BLEND_ADD:
            GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_DSTALPHA, GX_LO_CLEAR);
            break;
        case GRRLIB_BLEND_SCREEN:
            GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCCLR, GX_BL_DSTALPHA, GX_LO_CLEAR);
            break;
        case GRRLIB_BLEND_MULTI:
            GX_SetBlendMode(GX_BM_SUBSTRACT, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
            break;
        case GRRLIB_BLEND_INV:
            GX_SetBlendMode(GX_BM_BLEND, GX_BL_INVSRCCLR, GX_BL_INVSRCCLR, GX_LO_CLEAR);
            break;
    }
}

/**
 * Get the current blending mode.
 * @return The current blending mode.
 */
INLINE
GRRLIB_blendMode  GRRLIB_GetBlend (void) {
    return GRRLIB_Settings.blend;
}

/**
 * Turn anti-aliasing on/off.
 * @param aa Set to true to enable anti-aliasing (Default: Enabled).
 */
INLINE
void  GRRLIB_SetAntiAliasing (const bool aa) {
    GRRLIB_Settings.antialias = aa;
}

/**
 * Get current anti-aliasing setting.
 * @return True if anti-aliasing is enabled.
 */
INLINE
bool  GRRLIB_GetAntiAliasing (void) {
    return GRRLIB_Settings.antialias;
}

/**
 * Set global rendering color.
 * @param r Red component of color.
 * @param g Green component of color.
 * @param b Blue component of color.
 */
INLINE
void  GRRLIB_SetColor (const u8 r, const u8 g, const u8 b) {
    GRRLIB_Settings.colorR = r;
    GRRLIB_Settings.colorG = g;
    GRRLIB_Settings.colorB = b;
    GRRLIB_Settings.colorRGBA = RGBA(r, g, b, GRRLIB_Settings.colorA);
}

/**
 * Set global rendering color in RGBA-format.
 * @param color Color in RGBA-format.
 */
INLINE
void  GRRLIB_SetColorRGBA (const u32 color) {
    GRRLIB_Settings.colorR = R(color);
    GRRLIB_Settings.colorG = G(color);
    GRRLIB_Settings.colorB = B(color);
    GRRLIB_Settings.colorA = A(color);
    GRRLIB_Settings.colorRGBA = color;
}

/**
 * Set global rendering color in RGB-format.
 * @param a Alpha component of color.
 */
INLINE
void  GRRLIB_SetAlpha (const u32 alpha) {
    GRRLIB_Settings.colorA = alpha;
    GRRLIB_Settings.colorRGBA = RGBA(GRRLIB_Settings.colorR, GRRLIB_Settings.colorG, GRRLIB_Settings.colorB, alpha);
}

/**
 * Get red component of current color.
 * @return Red component of color.
 */
INLINE
u8  GRRLIB_GetColorRed (void) {
    return GRRLIB_Settings.colorR;
}

/**
 * Get green component of current color.
 * @return Green component of color.
 */
INLINE
u8  GRRLIB_GetColorGreen (void) {
    return GRRLIB_Settings.colorG;
}

/**
 * Get blue component of current color.
 * @return Blue component of color.
 */
INLINE
u8  GRRLIB_GetColorBlue (void) {
    return GRRLIB_Settings.colorB;
}

/**
 * Get current alpha.
 * @return Alpha.
 */
INLINE
u8  GRRLIB_GetAlpha (void) {
    return GRRLIB_Settings.colorA;
}

/**
 * Set global rotation.
 * @param rot Rotation in degrees.
 */
INLINE
void  GRRLIB_SetRotation (const f32 rot) {
    GRRLIB_Settings.rotation = rot;
}

/**
 * Get current rotation.
 * @return Rotation in degrees.
 */
INLINE
f32  GRRLIB_GetRotation (void) {
    return GRRLIB_Settings.rotation;
}

/**
 * Set global drawing scale.
 * @param sx Horizontal scale.
 * @param sy Vertical scale.
 */
INLINE
void  GRRLIB_SetScale (const f32 sx, const f32 sy) {
    GRRLIB_Settings.scaleX = sx;
    GRRLIB_Settings.scaleY = sy;
}

/**
 * Set global horizontal drawing scale.
 * @param sx Horizontal scale.
 */
INLINE
void  GRRLIB_SetScaleX (const f32 sx) {
    GRRLIB_Settings.scaleX = sx;
}

/**
 * Set global vertical drawing scale.
 * @param sy Vertical scale.
 */
INLINE
void  GRRLIB_SetScaleY (const f32 sy) {
    GRRLIB_Settings.scaleY = sy;
}

/**
 * Get current horizontal scale
 * @return Horizontal scale.
 */
INLINE
f32  GRRLIB_GetScaleX (void) {
    return GRRLIB_Settings.scaleX;
}

/**
 * Get current vertical scale
 * @return Vertical scale.
 */
INLINE
f32  GRRLIB_GetScaleY (void) {
    return GRRLIB_Settings.scaleY;
}

/**
 * Reset global drawing settings to default.
 */
INLINE
void  GRRLIB_ResetSettings (void) {
    GRRLIB_SetColorRGBA( 0xFFFFFFFF );
    GRRLIB_SetScale( 1.0f, 1.0f );
    GRRLIB_SetRotation( 0.0f );
}