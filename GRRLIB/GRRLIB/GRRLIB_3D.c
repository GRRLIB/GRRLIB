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

#include <math.h>

#include <grrlib.h>

// User should not directly modify these
Mtx       _GRR_view;  // Should be static as soon as all light functions needing this var will be in this file ;)
static  guVector  _GRR_cam  = {0.0f, 0.0f, 0.0f};
static  guVector  _GRR_up   = {0.0f, 1.0f, 0.0f};
static  guVector  _GRR_look = {0.0f, 0.0f, -100.0f};
static  guVector  _GRRaxisx = (guVector){1.0f, 0.0f, 0.0f}; // DO NOT MODIFY!!!
static  guVector  _GRRaxisy = (guVector){0.0f, 1.0f, 0.0f}; // Even at runtime
static  guVector  _GRRaxisz = (guVector){0.0f, 0.0f, 1.0f}; // NOT ever!
static  Mtx       _ObjTransformationMtx;

/**
 * Set the background parameter when screen is cleared.
 * @param r Red component.
 * @param g Green component.
 * @param b Blue component.
 * @param a Alpha component.
 */
void GRRLIB_SetBackgroundColour(u8 r, u8 g, u8 b, u8 a) {
   GX_SetCopyClear((GXColor){ r, g, b, a }, GX_MAX_Z24);
}

/**
 * Set the camera parameter (contributed by chris_c aka DaShAmAn).
 * @param posx x position of the camera.
 * @param posy y position of the camera.
 * @param posz z position of the camera.
 * @param upx x up position of the camera.
 * @param upy y up position of the camera.
 * @param upz z up position of the camera.
 * @param lookx x position of the target.
 * @param looky y position of the target.
 * @param lookz z position of the target.
 */
void GRRLIB_Camera3dSettings(f32 posx, f32 posy, f32 posz,
    f32 upx, f32 upy, f32 upz,
    f32 lookx, f32 looky, f32 lookz) {

   _GRR_cam.x = posx;
   _GRR_cam.y = posy;
   _GRR_cam.z = posz;

   _GRR_up.x = upx;
   _GRR_up.y = upy;
   _GRR_up.z = upz;

   _GRR_look.x = lookx;
   _GRR_look.y = looky;
   _GRR_look.z = lookz;
}

/**
 * Set up the position matrix (contributed by chris_c aka DaShAmAn).
 * @param minDist Minimal distance for the camera.
 * @param maxDist Maximal distance for the camera.
 * @param fov Field of view for the camera.
 * @param texturemode False, GX won't need texture coordinate, True, GX will need texture coordinate.
 * @param normalmode False, GX won't need normal coordinate, True, GX will need normal coordinate.
 */
void GRRLIB_3dMode(f32 minDist, f32 maxDist, f32 fov, bool texturemode, bool normalmode) {
    Mtx44 m;

    guLookAt(_GRR_view, &_GRR_cam, &_GRR_up, &_GRR_look);
    guPerspective(m, fov, (f32)rmode->fbWidth/rmode->efbHeight, minDist, maxDist);
    GX_LoadProjectionMtx(m, GX_PERSPECTIVE);
    GX_SetZMode (GX_TRUE, GX_LEQUAL, GX_TRUE);

    GX_SetCullMode(GX_CULL_NONE);

    GX_ClearVtxDesc();
    GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
    if(normalmode == true) {
        GX_SetVtxDesc(GX_VA_NRM, GX_DIRECT);
    }
    GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    if(texturemode == true) {
        GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    }

    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    if(normalmode == true) {
        GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
    }
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    if(texturemode == true) {
        GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    }

    if(texturemode == true) {
        GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    }
    else {
        GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    }
}

/**
 * Go back to 2D mode (contributed by chris_c aka DaShAmAn).
 */
void GRRLIB_2dMode(void) {
    Mtx view;
    Mtx44 m;

    GX_SetZMode(GX_FALSE, GX_LEQUAL, GX_TRUE);

    GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

    guOrtho(m, 0, rmode->efbHeight, 0, rmode->fbWidth, 0, 1000.0f);
    GX_LoadProjectionMtx(m, GX_ORTHOGRAPHIC);

    guMtxIdentity(view);
    guMtxTransApply(view, view, 0, 0, -100.0f);
    GX_LoadPosMtxImm(view, GX_PNMTX0);

    GX_ClearVtxDesc();
    GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    GX_SetNumTexGens(1);  // One texture exists
    GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

    GX_SetNumTevStages(1);

    GX_SetTevOp  (GX_TEVSTAGE0, GX_PASSCLR);

    GX_SetNumChans(1);
    GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);

    GRRLIB_Settings.lights = 0;
}

/**
 * Init the object matrix to draw object.
 */
void GRRLIB_ObjectViewBegin(void) {
    guMtxIdentity(_ObjTransformationMtx);
}

/**
 * Scale the object matrix to draw object.
 * @param scalx x scale of the object.
 * @param scaly y scale of the object.
 * @param scalz z scale of the object.
 */
void GRRLIB_ObjectViewScale(f32 scalx, f32 scaly, f32 scalz) {
    Mtx m;

    guMtxIdentity(m);
    guMtxScaleApply(m, m, scalx, scaly, scalz);

    guMtxConcat(m, _ObjTransformationMtx, _ObjTransformationMtx);
}

/**
 * Rotate the object matrix to draw object.
 * @param angx x rotation angle of the object.
 * @param angy y rotation angle of the object.
 * @param angz z rotation angle of the object.
 */
void GRRLIB_ObjectViewRotate(f32 angx, f32 angy, f32 angz) {
    Mtx m;
    Mtx rx, ry, rz;

    guMtxIdentity(m);
    guMtxRotAxisDeg(rx, &_GRRaxisx, angx);
    guMtxRotAxisDeg(ry, &_GRRaxisy, angy);
    guMtxRotAxisDeg(rz, &_GRRaxisz, angz);
    guMtxConcat(ry, rx, m);
    guMtxConcat(m, rz, m);

    guMtxConcat(m, _ObjTransformationMtx, _ObjTransformationMtx);
}

/**
 * Translate the object matrix to draw object.
 * @param posx x position of the object.
 * @param posy y position of the object.
 * @param posz z position of the object.
 */
void GRRLIB_ObjectViewTrans(f32 posx, f32 posy, f32 posz) {
    Mtx m;

    guMtxIdentity(m);
    guMtxTransApply(m, m, posx, posy, posz);

    guMtxConcat(m, _ObjTransformationMtx, _ObjTransformationMtx);
}

/**
 * Concat the object and the view matrix and calculate the inverse normal matrix.
 */
void GRRLIB_ObjectViewEnd(void) {
    Mtx mv, mvi;

    guMtxConcat(_GRR_view, _ObjTransformationMtx, mv);
    GX_LoadPosMtxImm(mv, GX_PNMTX0);

    guMtxInverse(mv, mvi);
    guMtxTranspose(mvi, mv);
    GX_LoadNrmMtxImm(mv, GX_PNMTX0);
}

/**
 * Set the view matrix to draw object (in this order scale, rotate AND trans).
 * @param posx x position of the object.
 * @param posy y position of the object.
 * @param posz z position of the object.
 * @param angx x rotation angle of the object.
 * @param angy y rotation angle of the object.
 * @param angz z rotation angle of the object.
 * @param scalx x scale of the object.
 * @param scaly y scale of the object.
 * @param scalz z scale of the object.
 */
void GRRLIB_ObjectView(f32 posx, f32 posy, f32 posz, f32 angx, f32 angy, f32 angz, f32 scalx, f32 scaly, f32 scalz) {
    Mtx ObjTransformationMtx;
    Mtx m;
    Mtx mv, mvi;

    guMtxIdentity(ObjTransformationMtx);

    if((scalx != 1.0f) || (scaly != 1.0f) || (scalz != 1.0f)) {
        guMtxIdentity(m);
        guMtxScaleApply(m, m, scalx, scaly, scalz);

        guMtxConcat(m, ObjTransformationMtx, ObjTransformationMtx);
    }

    if((angx != 0.0f) || (angy != 0.0f) || (angz != 0.0f)) {
        Mtx rx, ry, rz;
        guMtxIdentity(m);
        guMtxRotAxisDeg(rx, &_GRRaxisx, angx);
        guMtxRotAxisDeg(ry, &_GRRaxisy, angy);
        guMtxRotAxisDeg(rz, &_GRRaxisz, angz);
        guMtxConcat(ry, rx, m);
        guMtxConcat(m, rz, m);

        guMtxConcat(m, ObjTransformationMtx, ObjTransformationMtx);
    }

    if((posx != 0.0f) || (posy != 0.0f) || (posz != 0.0f)) {
        guMtxIdentity(m);
        guMtxTransApply(m, m, posx, posy, posz);

        guMtxConcat(m, ObjTransformationMtx, ObjTransformationMtx);
    }

    guMtxConcat(_GRR_view, ObjTransformationMtx, mv);
    GX_LoadPosMtxImm(mv, GX_PNMTX0);

    guMtxInverse(mv, mvi);
    guMtxTranspose(mvi, mv);
    GX_LoadNrmMtxImm(mv, GX_PNMTX0);
}

/**
 * Set the view matrix to draw object (in this order scale, trans AND rotate).
 * @param posx x position of the object.
 * @param posy y position of the object.
 * @param posz z position of the object.
 * @param angx x rotation angle of the object.
 * @param angy y rotation angle of the object.
 * @param angz z rotation angle of the object.
 * @param scalx x scale of the object.
 * @param scaly y scale of the object.
 * @param scalz z scale of the object.
 */
void GRRLIB_ObjectViewInv(f32 posx, f32 posy, f32 posz, f32 angx, f32 angy, f32 angz, f32 scalx, f32 scaly, f32 scalz) {
    Mtx ObjTransformationMtx;
    Mtx m;
    Mtx mv, mvi;

    guMtxIdentity(ObjTransformationMtx);

    if((scalx != 1.0f) || (scaly != 1.0f) || (scalz != 1.0f)) {
        guMtxIdentity(m);
        guMtxScaleApply(m, m, scalx, scaly, scalz);

        guMtxConcat(m, ObjTransformationMtx, ObjTransformationMtx);
    }

    if((posx != 0.0f) || (posy != 0.0f) || (posz != 0.0f)) {
        guMtxIdentity(m);
        guMtxTransApply(m, m, posx, posy, posz);

        guMtxConcat(m, ObjTransformationMtx, ObjTransformationMtx);
    }

    if((angx != 0.0f) || (angy != 0.0f) || (angz != 0.0f)) {
        Mtx rx, ry, rz;
        guMtxIdentity(m);
        guMtxRotAxisDeg(rx, &_GRRaxisx, angx);
        guMtxRotAxisDeg(ry, &_GRRaxisy, angy);
        guMtxRotAxisDeg(rz, &_GRRaxisz, angz);
        guMtxConcat(ry, rx, m);
        guMtxConcat(m, rz, m);

        guMtxConcat(m, ObjTransformationMtx, ObjTransformationMtx);
    }

    guMtxConcat(_GRR_view, ObjTransformationMtx, mv);
    GX_LoadPosMtxImm(mv, GX_PNMTX0);

    guMtxInverse(mv, mvi);
    guMtxTranspose(mvi, mv);
    GX_LoadNrmMtxImm(mv, GX_PNMTX0);
}

/**
 * Set the texture to an object (contributed by chris_c aka DaShAmAn).
 * @param tex Pointer to an image texture (GRRLIB_texImg format).
 * @param rep Texture Repeat Mode, @c true will repeat it, @c false won't.
 */
void GRRLIB_SetTexture(GRRLIB_texImg *tex, bool rep) {
    GXTexObj  texObj;

    if (rep == true) {
        GX_InitTexObj(&texObj, tex->data, tex->w, tex->h, tex->format, GX_REPEAT, GX_REPEAT, GX_FALSE);
    }
    else {
        GX_InitTexObj(&texObj, tex->data, tex->w, tex->h, tex->format, GX_CLAMP, GX_CLAMP, GX_FALSE);
    }
    if (GRRLIB_Settings.antialias == false) {
        GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
        GX_SetCopyFilter(GX_FALSE, rmode->sample_pattern, GX_FALSE, rmode->vfilter);
    }
    else {
        GX_SetCopyFilter(rmode->aa, rmode->sample_pattern, GX_TRUE, rmode->vfilter);
    }

    GX_LoadTexObj(&texObj,      GX_TEXMAP0);
    GX_SetTevOp  (GX_TEVSTAGE0, GX_MODULATE);
    GX_SetVtxDesc(GX_VA_TEX0,   GX_DIRECT);
}

/**
 * Draw a torus (with normal).
 * @param r Radius of the ring.
 * @param R Radius of the torus.
 * @param nsides Number of faces per ring.
 * @param rings Number of rings.
 * @param filled Wired or not.
 * @param col Color of the torus.
 */
void GRRLIB_DrawTorus(f32 r, f32 R, int nsides, int rings, bool filled, u32 col) {
    const f32 ringDelta = 2.0 * M_PI / rings;
    const f32 sideDelta = 2.0 * M_PI / nsides;

    f32 theta = 0.0;
    f32 cosTheta = 1.0;
    f32 sinTheta = 0.0;
    for (int i = rings - 1; i >= 0; i--) {
        const f32 theta1 = theta + ringDelta;
        const f32 cosTheta1 = cosf(theta1);
        const f32 sinTheta1 = sinf(theta1);
        if(filled == true) {
            GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 2 * (nsides + 1));
        }
        else {
            GX_Begin(GX_LINESTRIP, GX_VTXFMT0, 2 * (nsides + 1));
        }
        f32 phi = 0.0;
        for (int j = nsides; j >= 0; j--) {
            phi += sideDelta;
            const f32 cosPhi = cosf(phi);
            const f32 sinPhi = sinf(phi);
            const f32 dist = R + r * cosPhi;

            GX_Position3f32(cosTheta1 * dist, -sinTheta1 * dist, r * sinPhi);
            GX_Normal3f32(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi);
            GX_Color1u32(col);
            GX_Position3f32(cosTheta * dist, -sinTheta * dist,  r * sinPhi);
            GX_Normal3f32(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi);
            GX_Color1u32(col);
        }
        GX_End();
        theta = theta1;
        cosTheta = cosTheta1;
        sinTheta = sinTheta1;
    }
}

/**
 * Draw a sphere (with normal).
 * @param r Radius of the sphere.
 * @param lats Number of latitudes.
 * @param longs Number of longitudes.
 * @param filled Wired or not.
 * @param col Color of the sphere.
 */
void GRRLIB_DrawSphere(f32 r, int lats, int longs, bool filled, u32 col) {
    const f32 dtheta = 2 * M_PI / longs;

    for(int i = 0; i <= lats; i++) {
        const f32 lat0 = M_PI * (-0.5f + (f32) (i - 1) / lats);
        const f32 z0  = sinf(lat0);
        const f32 zr0 = cosf(lat0);

        const f32 lat1 = M_PI * (-0.5f + (f32) i / lats);
        const f32 z1 = sinf(lat1);
        const f32 zr1 = cosf(lat1);

        GX_Begin((filled == true) ? GX_TRIANGLESTRIP : GX_LINESTRIP,
             GX_VTXFMT0, 2 * (longs + 1));

        for(int j = 0; j <= longs; j++) {
            const f32 lng = dtheta * (f32) (j - 1);
            const f32 x = cosf(lng);
            const f32 y = sinf(lng);

            GX_Position3f32(x * zr0 * r, y * zr0 * r, z0 * r);
            GX_Normal3f32(x * zr0 * r, y * zr0 * r, z0 * r);
            GX_Color1u32(col);
            GX_Position3f32(x * zr1 * r, y * zr1 * r, z1 * r);
            GX_Normal3f32(x * zr1 * r, y * zr1 * r, z1 * r);
            GX_Color1u32(col);
        }
        GX_End();
    }
}

/**
 * Draw a cube (with normal).
 * @param size Size of the cube edge.
 * @param filled Wired or not.
 * @param col Color of the cube.
 */
void GRRLIB_DrawCube(f32 size, bool filled, u32 col) {
    static const f32 normal[6][3] =
    {
        {-1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, -1.0f}
    };
    static const u8 faces[6][4] =
    {
        {0, 1, 2, 3},
        {3, 2, 6, 7},
        {7, 6, 5, 4},
        {4, 5, 1, 0},
        {5, 6, 2, 1},
        {7, 4, 0, 3}
    };
    f32 v[8][3];

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2.0f;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2.0f;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2.0f;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2.0f;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2.0f;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2.0f;

    for (s8 i = 5; i >= 0; i--) {
        if(filled == true) {
            GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
        }
        else {
            GX_Begin(GX_LINESTRIP, GX_VTXFMT0, 5);
        }
        GX_Position3f32(v[faces[i][0]][0], v[faces[i][0]][1], v[faces[i][0]][2] );
        GX_Normal3f32(normal[i][0], normal[i][1], normal[i][2]);
        GX_Color1u32(col);
        GX_Position3f32(v[faces[i][1]][0], v[faces[i][1]][1], v[faces[i][1]][2]);
        GX_Normal3f32(normal[i][0], normal[i][1], normal[i][2]);
        GX_Color1u32(col);
        GX_Position3f32(v[faces[i][2]][0], v[faces[i][2]][1], v[faces[i][2]][2]);
        GX_Normal3f32(normal[i][0], normal[i][1], normal[i][2]);
        GX_Color1u32(col);
        GX_Position3f32(v[faces[i][3]][0], v[faces[i][3]][1], v[faces[i][3]][2]);
        GX_Normal3f32(normal[i][0], normal[i][1], normal[i][2]);
        GX_Color1u32(col);
        if(filled == false) {
            GX_Position3f32(v[faces[i][0]][0], v[faces[i][0]][1], v[faces[i][0]][2]);
            GX_Normal3f32(normal[i][0], normal[i][1], normal[i][2]);
            GX_Color1u32(col);
        }
        GX_End();
    }
}

/**
 * Draw a cylinder (with normal).
 * @param r Radius of the cylinder.
 * @param h High of the cylinder.
 * @param d Density of slice.
 * @param filled Wired or not.
 * @param col Color of the cylinder.
 */
void GRRLIB_DrawCylinder(f32 r, f32 h, u16 d, bool filled, u32 col) {
    if(filled == true) {
        GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 2 * (d + 1));
    }
    else {
        GX_Begin(GX_LINESTRIP, GX_VTXFMT0, 2 * (d + 1));
    }
    for(u16 i = 0; i <= d; i++) {
        const f32 dx = cosf( M_PI * 2.0f * i / d );
        const f32 dy = sinf( M_PI * 2.0f * i / d );
        GX_Position3f32( r * dx, -0.5f * h, r * dy );
        GX_Normal3f32( dx, 0.0f, dy );
        GX_Color1u32(col);
        GX_Position3f32( r * dx, 0.5f * h, r * dy );
        GX_Normal3f32( dx, 0.0f, dy );
        GX_Color1u32(col);
    }
    GX_End();

    if(filled == true) {
        GX_Begin(GX_TRIANGLEFAN, GX_VTXFMT0, d + 2);
    }
    else {
        GX_Begin(GX_LINESTRIP, GX_VTXFMT0, d + 2);
    }
    GX_Position3f32(0.0f, -0.5f * h, 0.0f);
    GX_Normal3f32(0.0f, -1.0f, 0.0f);
    GX_Color1u32(col);
    for(u16 i = 0; i <= d; i++) {
        GX_Position3f32( r * cosf( M_PI * 2.0f * i / d ), -0.5f * h, r * sinf( M_PI * 2.0f * i / d ) );
        GX_Normal3f32(0.0f, -1.0f, 0.0f);
        GX_Color1u32(col);
    }
    GX_End();

    if(filled == true) {
        GX_Begin(GX_TRIANGLEFAN, GX_VTXFMT0, d + 2);
    }
    else {
        GX_Begin(GX_LINESTRIP, GX_VTXFMT0, d + 2);
    }
    GX_Position3f32(0.0f, 0.5f * h, 0.0f);
    GX_Normal3f32(0.0f, 1.0f, 0.0f);
    GX_Color1u32(col);
    for(u16 i = 0; i <= d; i++) {
        GX_Position3f32( r * cosf( M_PI * 2.0f * i / d ), 0.5f * h, r * sinf( M_PI * 2.0f * i / d ) );
        GX_Normal3f32(0.0f, 1.0f, 0.0f);
        GX_Color1u32(col);
    }
    GX_End();
}

/**
 * Draw a cone (with normal).
 * @param r Radius of the cone.
 * @param h High of the cone.
 * @param d Density of slice.
 * @param filled Wired or not.
 * @param col Color of the cone.
 */
void GRRLIB_DrawCone(f32 r, f32 h, u16 d, bool filled, u32 col) {
    if(filled == true) {
        GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 2 * (d + 1));
    }
    else {
        GX_Begin(GX_LINESTRIP, GX_VTXFMT0, 2 * (d + 1));
    }
    for(u16 i = 0; i <= d; i++) {
        const f32 dx = cosf( M_PI * 2.0f * i / d );
        const f32 dy = sinf( M_PI * 2.0f * i / d );
        GX_Position3f32( 0, -0.5f * h, 0);
        GX_Normal3f32( dx, 0.0f, dy );
        GX_Color1u32(col);
        GX_Position3f32( r * dx, 0.5f * h, r * dy );
        GX_Normal3f32( dx, 0.0f, dy );
        GX_Color1u32(col);
    }
    GX_End();

    if(filled == true) {
        GX_Begin(GX_TRIANGLEFAN, GX_VTXFMT0, d + 2);
    }
    else {
        GX_Begin(GX_LINESTRIP, GX_VTXFMT0, d + 2);
    }
    GX_Position3f32(0.0f, 0.5f * h, 0.0f);
    GX_Normal3f32(0.0f, 1.0f, 0.0f);
    GX_Color1u32(col);
    for(u16 i = 0; i <= d; i++) {
        GX_Position3f32( r * cosf( M_PI * 2.0f * i / d ), 0.5f * h, r * sinf( M_PI * 2.0f * i / d ) );
        GX_Normal3f32(0.0f, 1.0f, 0.0f);
        GX_Color1u32(col);
    }
    GX_End();
}

/**
 * Draw a Tesselated panel (with normal).
 * @param w Width of the panel.
 * @param wstep Size of width slices.
 * @param h Height of the panel.
 * @param hstep Size the de height slices.
 * @param filled Wired or not.
 * @param col Color in RGBA format.
 */
void GRRLIB_DrawTessPanel(f32 w, f32 wstep, f32 h, f32 hstep, bool filled, u32 col) {
    const f32 tmpy = h / 2.0f;
    const f32 tmpx = w / 2.0f;
    const u16 tmp = ((w / wstep) * 2) + 2;
    for ( f32 y = -tmpy; y <= tmpy; y += hstep )
    {
        if(filled == true) {
            GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, tmp);
        }
        else {
            GX_Begin(GX_LINESTRIP, GX_VTXFMT0, tmp);
        }
        for ( f32 x = -tmpx; x <= tmpx; x += wstep )
        {
            GX_Position3f32( x, y, 0.0f );
            GX_Normal3f32( 0.0f, 0.0f, 1.0f);
            GX_Color1u32(col);
            GX_Position3f32( x, y + hstep, 0.0f );
            GX_Normal3f32( 0.0f, 0.0f, 1.0f);
            GX_Color1u32(col);
        }
        GX_End();
    }
}

/**
 * Set ambient color.
 * When no diffuse light is shining on a object, the color is equal to ambient color.
 * @param ambientcolor Ambient color in RGBA format.
 */
void GRRLIB_SetLightAmbient(u32 ambientcolor) {
    GX_SetChanAmbColor(GX_COLOR0A0, (GXColor) { R(ambientcolor), G(ambientcolor), B(ambientcolor), 0xFF});
}

/**
 * Set diffuse light parameters.
 * @param num Number of the light. It's a number from 0 to 7.
 * @param pos Position of the diffuse light (x/y/z).
 * @param distattn Distance attenuation.
 * @param brightness Brightness of the light. The value should be between 0 and 1.
 * @param lightcolor Color of the light in RGBA format.
 */
void GRRLIB_SetLightDiff(u8 num, guVector pos, f32 distattn, f32 brightness, u32 lightcolor) {
    GXLightObj MyLight;
    guVector lpos = {pos.x, pos.y, pos.z};

    GRRLIB_Settings.lights |= (1<<num);

    guVecMultiply(_GRR_view, &lpos, &lpos);
    GX_InitLightPos(&MyLight, lpos.x, lpos.y, lpos.z);
    GX_InitLightColor(&MyLight, (GXColor) { R(lightcolor), G(lightcolor), B(lightcolor), 0xFF });
    GX_InitLightSpot(&MyLight, 0.0f, GX_SP_OFF);
    GX_InitLightDistAttn(&MyLight, distattn, brightness, GX_DA_MEDIUM); // DistAttn = 20.0  &  Brightness=1.0f (full)
    GX_LoadLightObj(&MyLight, (1<<num));

    // Turn light ON
    GX_SetNumChans(1);
    GX_SetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX, GRRLIB_Settings.lights, GX_DF_CLAMP, GX_AF_SPOT);
}

/**
 * Set specular light parameters.
 * @param num Number of the light. It's a number from 0 to 7.
 * @param dir Direction of the specular ray (x/y/z).
 * @param shininess Shininess of the specular. ( between 4 and 254)
 * @param lightcolor Color of the light in RGBA format.
 * @param speccolor Specular color in RGBA format.
 */
void GRRLIB_SetLightSpec(u8 num, guVector dir, f32 shininess, u32 lightcolor, u32 speccolor) {
    Mtx mr, mv;
    GXLightObj MyLight;
    guVector ldir = {dir.x, dir.y, dir.z};

    GRRLIB_Settings.lights |= (1<<num);

    guMtxInverse(_GRR_view,mr);
    guMtxTranspose(mr,mv);
    guVecMultiplySR(mv, &ldir,&ldir);
    GX_InitSpecularDirv(&MyLight, &ldir);

    GX_InitLightShininess(&MyLight, shininess);  // between 4 and 255 !!!
    GX_InitLightColor(&MyLight, (GXColor) { R(lightcolor), G(lightcolor), B(lightcolor), 0xFF });
    GX_LoadLightObj(&MyLight, (1<<num));

    /////////////////////// Turn light ON ////////////////////////////////////////////////
    GX_SetNumChans(2);    // use two color channels
    GX_SetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT0, GX_DF_CLAMP, GX_AF_NONE);
    GX_SetChanCtrl(GX_COLOR1, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_NONE, GX_AF_SPEC);
    GX_SetChanCtrl(GX_ALPHA0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHTNULL, GX_DF_NONE, GX_AF_NONE);
    GX_SetChanCtrl(GX_ALPHA1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHTNULL, GX_DF_NONE, GX_AF_NONE);


    GX_SetNumTevStages(2);
    GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0 );
    GX_SetTevOrder(GX_TEVSTAGE1, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR1A1 );
    GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GX_SetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV );
    GX_SetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_RASC, GX_CC_ONE, GX_CC_CPREV );

    /////////////////////// Define Material and Ambient color and draw object /////////////////////////////////////
    GX_SetChanAmbColor(GX_COLOR1, (GXColor){0x00,0x00,0x00,0xFF});  // specular ambient forced to black
    GX_SetChanMatColor(GX_COLOR1, (GXColor) { R(speccolor), G(speccolor), B(speccolor), 0xFF }); // specular reflection color
}

/**
 * Set Spot light parameters.
 * @param num Number of the light. It's a number from 0 to 7.
 * @param pos Position of the spot light (x/y/z).
 * @param lookat Where spot light look at (x/y/z).
 * @param angAttn0 cone attenuation factor 0.
 * @param angAttn1 cone attenuation factor 1.
 * @param angAttn2 cone attenuation factor 2.
 * @param distAttn0 Distance attenuation factor 0.
 * @param distAttn1 Distance attenuation factor 1.
 * @param distAttn2 Distance attenuation factor 2.
 * @param lightcolor Color of the light in RGBA format.
 */
void GRRLIB_SetLightSpot(u8 num, guVector pos, guVector lookat, f32 angAttn0, f32 angAttn1, f32 angAttn2, f32 distAttn0, f32 distAttn1, f32 distAttn2, u32 lightcolor) {
    GXLightObj lobj;
    guVector lpos = (guVector){ pos.x, pos.y, pos.z };
    guVector ldir = (guVector){ lookat.x-pos.x, lookat.y-pos.y, lookat.z-pos.z };
    guVecNormalize(&ldir);

    GRRLIB_Settings.lights |= (1<<num);

    guVecMultiplySR(_GRR_view, &ldir,&ldir);
    guVecMultiply(_GRR_view, &lpos, &lpos);

    GX_InitLightDirv(&lobj, &ldir);
    GX_InitLightPosv(&lobj, &lpos);
    GX_InitLightColor(&lobj, (GXColor) { R(lightcolor), G(lightcolor), B(lightcolor), 0xFF });

    //this is just for code readers, wanting to know how to use direct cut off
    //GX_InitLightSpot(&lobj, 0<angle<90, GX_SP_FLAT);

    GX_InitLightAttn(&lobj, angAttn0, angAttn1, angAttn2, distAttn0, distAttn1, distAttn2);

    GX_LoadLightObj(&lobj, (1<<num));

    // Turn light ON
    GX_SetNumChans(1);
    GX_SetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX, GRRLIB_Settings.lights, GX_DF_CLAMP, GX_AF_SPOT);
}

/**
 * Set all lights off, like at init.
 */
void GRRLIB_SetLightOff(void) {
    GX_SetNumTevStages(1);

    GX_SetTevOp  (GX_TEVSTAGE0, GX_PASSCLR);

    GX_SetNumChans(1);
    GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);

    GRRLIB_Settings.lights = 0;
}
