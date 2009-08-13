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

#include <math.h>

#include <grrlib.h>

extern  GRRLIB_drawSettings  GRRLIB_Settings;
extern  Mtx                  GXmodelView2D;

/**
 * Draw a texture.
 * @param xpos Specifies the x-coordinate of the upper-left corner.
 * @param ypos Specifies the y-coordinate of the upper-left corner.
 * @param tex The texture to draw.
 * @param degrees Angle of rotation.
 * @param scaleX Specifies the x-coordinate scale. -1 could be used for flipping the texture horizontally.
 * @param scaleY Specifies the y-coordinate scale. -1 could be used for flipping the texture vertically.
 * @param color Color in RGBA format.
 */
void  GRRLIB_DrawImg (const f32 xpos, const f32 ypos, const GRRLIB_texImg *tex,
                      const f32 degrees, const f32 scaleX, const f32 scaleY,
                      const u32 color) {
    if (tex == NULL || tex->data == NULL) {
        return;
    }

    GXTexObj texObj;
    u16 width, height;
    Mtx m, m1, m2, mv;

    GX_InitTexObj(&texObj, tex->data, tex->w, tex->h, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    if (GRRLIB_Settings.antialias == false) {
        GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
    }

    GX_LoadTexObj(&texObj, GX_TEXMAP0);
    GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    width = tex->w * 0.5;
    height = tex->h * 0.5;
    guMtxIdentity(m1);
    guMtxScaleApply(m1, m1, scaleX, scaleY, 1.0);
    guVector axis = (guVector) {0, 0, 1};
    guMtxRotAxisDeg (m2, &axis, degrees);
    guMtxConcat(m2, m1, m);

    guMtxTransApply(m, m, xpos+width+tex->handlex-tex->offsetx+(scaleX*( -tex->handley*sin(-DegToRad(degrees)) - tex->handlex*cos(-DegToRad(degrees)) )), ypos+height+tex->handley-tex->offsety+(scaleX*( -tex->handley*cos(-DegToRad(degrees)) + tex->handlex*sin(-DegToRad(degrees)) )), 0);
    guMtxConcat(GXmodelView2D, m, mv);

    GX_LoadPosMtxImm(mv, GX_PNMTX0);
    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
        GX_Position3f32(-width, -height, 0);
        GX_Color1u32(color);
        GX_TexCoord2f32(0, 0);

        GX_Position3f32(width, -height, 0);
        GX_Color1u32(color);
        GX_TexCoord2f32(1, 0);

        GX_Position3f32(width, height, 0);
        GX_Color1u32(color);
        GX_TexCoord2f32(1, 1);

        GX_Position3f32(-width, height, 0);
        GX_Color1u32(color);
        GX_TexCoord2f32(0, 1);
    GX_End();
    GX_LoadPosMtxImm(GXmodelView2D, GX_PNMTX0);

    GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);
}

/**
 * Draw a textured quad.
 * @param pos Vector array of the 4 points.
 * @param tex The texture to draw.
 * @param color Color in RGBA format.
 */
void  GRRLIB_DrawImgQuad (const guVector pos[4], GRRLIB_texImg *tex,
                          const u32 color) {
    if (tex == NULL || tex->data == NULL) {
        return;
    }

    GXTexObj texObj;
    Mtx m, m1, m2, mv;

    GX_InitTexObj(&texObj, tex->data, tex->w, tex->h, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    if (GRRLIB_Settings.antialias == false) {
        GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
    }

    GX_LoadTexObj(&texObj, GX_TEXMAP0);
    GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    guMtxIdentity(m1);
    guMtxScaleApply(m1, m1, 1, 1, 1.0);
    guVector axis = (guVector) {0, 0, 1};
    guMtxRotAxisDeg (m2, &axis, 0);
    guMtxConcat(m2, m1, m);

    guMtxConcat(GXmodelView2D, m, mv);

    GX_LoadPosMtxImm(mv, GX_PNMTX0);
    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
        GX_Position3f32(pos[0].x, pos[0].y, 0);
        GX_Color1u32(color);
        GX_TexCoord2f32(0, 0);

        GX_Position3f32(pos[1].x, pos[1].y, 0);
        GX_Color1u32(color);
        GX_TexCoord2f32(1, 0);

        GX_Position3f32(pos[2].x, pos[2].y, 0);
        GX_Color1u32(color);
        GX_TexCoord2f32(1, 1);

        GX_Position3f32(pos[3].x, pos[3].y, 0);
        GX_Color1u32(color);
        GX_TexCoord2f32(0, 1);
    GX_End();
    GX_LoadPosMtxImm(GXmodelView2D, GX_PNMTX0);

    GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);
}

/**
 * Draw a tile.
 * @param xpos Specifies the x-coordinate of the upper-left corner.
 * @param ypos Specifies the y-coordinate of the upper-left corner.
 * @param tex The texture containing the tile to draw.
 * @param degrees Angle of rotation.
 * @param scaleX Specifies the x-coordinate scale. -1 could be used for flipping the texture horizontally.
 * @param scaleY Specifies the y-coordinate scale. -1 could be used for flipping the texture vertically.
 * @param color Color in RGBA format.
 * @param frame Specifies the frame to draw.
 */
void  GRRLIB_DrawTile (const f32 xpos, const f32 ypos, const GRRLIB_texImg *tex,
                       const f32 degrees, const f32 scaleX, const f32 scaleY,
                       const u32 color, const int frame) {
    if (tex == NULL || tex->data == NULL) {
        return;
    }

    GXTexObj texObj;
    f32 width, height;
    Mtx m, m1, m2, mv;

    // Frame Correction by spiffen
    f32 FRAME_CORR = 0.001f;
    f32 s1 = (((frame%tex->nbtilew))/(f32)tex->nbtilew)+(FRAME_CORR/tex->w);
    f32 s2 = (((frame%tex->nbtilew)+1)/(f32)tex->nbtilew)-(FRAME_CORR/tex->w);
    f32 t1 = (((int)(frame/tex->nbtilew))/(f32)tex->nbtileh)+(FRAME_CORR/tex->h);
    f32 t2 = (((int)(frame/tex->nbtilew)+1)/(f32)tex->nbtileh)-(FRAME_CORR/tex->h);

    GX_InitTexObj(&texObj, tex->data, tex->tilew*tex->nbtilew, tex->tileh*tex->nbtileh, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    if (GRRLIB_Settings.antialias == false) {
        GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
    }
    GX_LoadTexObj(&texObj, GX_TEXMAP0);

    GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    width = tex->tilew * 0.5f;
    height = tex->tileh * 0.5f;
    guMtxIdentity(m1);
    guMtxScaleApply(m1, m1, scaleX, scaleY, 1.0f);

    guVector axis = (guVector) {0, 0, 1};
    guMtxRotAxisDeg(m2, &axis, degrees);
    guMtxConcat(m2, m1, m);
    guMtxTransApply(m, m, xpos+width+tex->handlex-tex->offsetx+(scaleX*( -tex->handley*sin(-DegToRad(degrees)) - tex->handlex*cos(-DegToRad(degrees)) )), ypos+height+tex->handley-tex->offsety+(scaleX*( -tex->handley*cos(-DegToRad(degrees)) + tex->handlex*sin(-DegToRad(degrees)) )), 0);
    guMtxConcat(GXmodelView2D, m, mv);

    GX_LoadPosMtxImm(mv, GX_PNMTX0);
    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
        GX_Position3f32(-width, -height, 0);
        GX_Color1u32(color);
        GX_TexCoord2f32(s1, t1);

        GX_Position3f32(width, -height,  0);
        GX_Color1u32(color);
        GX_TexCoord2f32(s2, t1);

        GX_Position3f32(width, height,  0);
        GX_Color1u32(color);
        GX_TexCoord2f32(s2, t2);

        GX_Position3f32(-width, height,  0);
        GX_Color1u32(color);
        GX_TexCoord2f32(s1, t2);
    GX_End();
    GX_LoadPosMtxImm(GXmodelView2D, GX_PNMTX0);

    GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);
}

/**
 * Draw a tile in a quad.
 * @param pos Vector array of the 4 points.
 * @param tex The texture to draw.
 * @param color Color in RGBA format.
 * @param frame Specifies the frame to draw.
 */
void  GRRLIB_DrawTileQuad (const guVector pos[4], GRRLIB_texImg *tex,
                           const u32 color, const int frame) {
    if (tex == NULL || tex->data == NULL) {
        return;
    }

    GXTexObj texObj;
    Mtx m, m1, m2, mv;

    // Frame Correction by spiffen
    f32 FRAME_CORR = 0.001f;
    f32 s1 = (((frame%tex->nbtilew))/(f32)tex->nbtilew)+(FRAME_CORR/tex->w);
    f32 s2 = (((frame%tex->nbtilew)+1)/(f32)tex->nbtilew)-(FRAME_CORR/tex->w);
    f32 t1 = (((int)(frame/tex->nbtilew))/(f32)tex->nbtileh)+(FRAME_CORR/tex->h);
    f32 t2 = (((int)(frame/tex->nbtilew)+1)/(f32)tex->nbtileh)-(FRAME_CORR/tex->h);

    GX_InitTexObj(&texObj, tex->data, tex->tilew*tex->nbtilew, tex->tileh*tex->nbtileh, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    if (GRRLIB_Settings.antialias == false) {
        GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
    }
    GX_LoadTexObj(&texObj, GX_TEXMAP0);

    GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    guMtxIdentity(m1);
    guMtxScaleApply(m1, m1, 1, 1, 1.0f);

    guVector axis = (guVector) {0, 0, 1};
    guMtxRotAxisDeg(m2, &axis, 0);
    guMtxConcat(m2, m1, m);

    guMtxConcat(GXmodelView2D, m, mv);

    GX_LoadPosMtxImm(mv, GX_PNMTX0);
    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
        GX_Position3f32(pos[0].x, pos[0].y, 0);
        GX_Color1u32(color);
        GX_TexCoord2f32(s1, t1);

        GX_Position3f32(pos[1].x, pos[1].y, 0);
        GX_Color1u32(color);
        GX_TexCoord2f32(s2, t1);

        GX_Position3f32(pos[2].x, pos[2].y, 0);
        GX_Color1u32(color);
        GX_TexCoord2f32(s2, t2);

        GX_Position3f32(pos[3].x, pos[3].y, 0);
        GX_Color1u32(color);
        GX_TexCoord2f32(s1, t2);
    GX_End();
    GX_LoadPosMtxImm(GXmodelView2D, GX_PNMTX0);

    GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);
}

/**
 * Call this function after drawing.
 */
void  GRRLIB_Render (void) {
    GX_DrawDone();
    GX_InvalidateTexAll();

    fb ^= 1;        // Flip framebuffer
    GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GX_SetColorUpdate(GX_TRUE);
    GX_CopyDisp(xfb[fb], GX_TRUE);

    VIDEO_SetNextFramebuffer(xfb[fb]);
    VIDEO_Flush();

    VIDEO_WaitVSync();
    if (rmode->viTVMode &VI_NON_INTERLACE)  VIDEO_WaitVSync() ;
}
