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

#include <math.h>

#include <grrlib.h>

// User should not directly modify these
static  Mtx       _GRR_view;
static  guVector  _GRR_cam  = {0.0F, 0.0F, 0.0F},
                  _GRR_up   = {0.0F, 1.0F, 0.0F},
                  _GRR_look = {0.0F, 0.0F, -100.0F};
static  guVector  _GRRaxisx = (guVector){1, 0, 0}; // DO NOT MODIFY!!!
static  guVector  _GRRaxisy = (guVector){0, 1, 0}; // Even at runtime
static  guVector  _GRRaxisz = (guVector){0, 0, 1}; // NOT ever!

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
 * Set the camera parameter (contributed my chris_c aka DaShAmAn).
 * @param posx x position of the camera.
 * @param posy y position of the camera.
 * @param posz z position of the camera.
 * @param upx Alpha component.
 * @param upy Alpha component.
 * @param upz Alpha component.
 * @param lookx x up position of the camera.
 * @param looky y up position of the camera.
 * @param lookz z up position of the camera.
 */
void GRRLIB_Camera3dSettings(f32 posx, f32 posy, f32 posz,
    f32 upx, f32 upy, f32 upz,
    f32 lookx, f32 looky, f32 lookz) {

   _GRR_cam.x=posx;
   _GRR_cam.y=posy;
   _GRR_cam.z=posz;

   _GRR_up.x=upx;
   _GRR_up.y=upy;
   _GRR_up.z=upz;

   _GRR_look.x=lookx;
   _GRR_look.y=looky;
   _GRR_look.z=lookz;
}

/**
 * Set up the position matrix (contributed by chris_c aka DaShAmAn).
 * @param minDist Minimal distance for the camera.
 * @param maxDist Maximal distance for the camera.
 * @param fov Field of view for the camera.
 * @param colormode False, GX won't need vertex colors, True, GX will need vertex colors.
 * @param texturemode False, GX won't need texture coordinate, True, GX will need texture coordinate.
 * @param normalmode False, GX won't need normal coordinate, True, GX will need normal coordinate.
 */
void GRRLIB_3dMode(f32 minDist, f32 maxDist, f32 fov, bool colormode, bool texturemode, bool normalmode) {
    Mtx m;

    guLookAt(_GRR_view, &_GRR_cam, &_GRR_up, &_GRR_look);
    guPerspective(m, fov, (f32)rmode->fbWidth/rmode->efbHeight, minDist, maxDist);
    GX_LoadProjectionMtx(m, GX_PERSPECTIVE);
    GX_SetZMode (GX_TRUE, GX_LEQUAL, GX_TRUE);

    GX_SetCullMode(GX_CULL_NONE);

    GX_ClearVtxDesc();
    GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
    if(normalmode)   GX_SetVtxDesc(GX_VA_NRM, GX_DIRECT);
    if(colormode)    GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    if(texturemode)  GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    if(normalmode)   GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
    if(colormode)    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    if(texturemode)  GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    if(texturemode)  GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    else             GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
}

/**
 * Go back to 2D mode (contributed by chris_c aka DaShAmAn).
 */
void GRRLIB_2dMode() {
    Mtx view, m;

    GX_SetZMode(GX_FALSE, GX_LEQUAL, GX_TRUE);

    GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

    guOrtho(m, 0, rmode->efbHeight, 0, rmode->fbWidth, 0, 1000.0f);
    GX_LoadProjectionMtx(m, GX_ORTHOGRAPHIC);

    guMtxIdentity(view);
    guMtxTransApply(view, view, 0, 0, -100.0F);
    GX_LoadPosMtxImm(view, GX_PNMTX0);

    GX_ClearVtxDesc();
    GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    GX_SetTevOp  (GX_TEVSTAGE0, GX_PASSCLR);
}

/**
 * Set the view matrix to draw object (contributed by chris_c aka DaShAmAn).
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
    Mtx m, m1, mv, rx, ry, rz;
    Mtx mvi;

    guMtxIdentity(m1);
    guMtxScaleApply(m1, m1, scalx, scaly, scalz);

    guMtxRotAxisDeg(rx, &_GRRaxisx, angx);
    guMtxRotAxisDeg(ry, &_GRRaxisy, angy);
    guMtxRotAxisDeg(rz, &_GRRaxisz, angz);
    guMtxConcat(ry, rx, m);
    guMtxConcat(m, rz, m);

    guMtxConcat(m, m1, m);

    guMtxTransApply(m, m, posx, posy, posz);
    guMtxConcat(_GRR_view, m, mv);
    GX_LoadPosMtxImm(mv, GX_PNMTX0);

    guMtxInverse(mv, mvi);
    guMtxTranspose(mvi, mv);
    GX_LoadNrmMtxImm(mv, GX_PNMTX0);
}

/**
 * Set the texture to an object (contributed by chris_c aka DaShAmAn).
 * @param tex poiter to an image texture (GRRLIB_texImg format).
 * @param rep Texture Repeat Mode, True will repeat it, False won't.
*/
void GRRLIB_SetTexture(GRRLIB_texImg *tex, bool rep) {
    GXTexObj  texObj;

    if (rep) {
        GX_InitTexObj(&texObj, tex->data, tex->w, tex->h, GX_TF_RGBA8, GX_REPEAT, GX_REPEAT, GX_FALSE);
    }
    else {
        GX_InitTexObj(&texObj, tex->data, tex->w, tex->h, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
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
 * Initialise a diffuse light.
 * @param id A light ID in libogc style : GX_LIGHT0,..., GX_LIGHT7.
 * @param lpos A guVector x,y,z position of the light.
 * @param lcol Color of the light.
*/
void GRRLIB_InitLight(u8 id, guVector lpos, u32 lcol) {
    GXLightObj MyLight;
    guVecMultiply(_GRR_view, &lpos, &lpos);
    GX_InitLightPos(&MyLight, lpos.x, lpos.y, lpos.z);
    GX_InitLightColor(&MyLight, (GXColor) { R(lcol), G(lcol), B(lcol), A(lcol) });
    GX_InitLightAttn(&MyLight, 1.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F);
    GX_LoadLightObj(&MyLight, id);
}

/**
 * All light off, colors come from the Vertex.
*/
void GRRLIB_LightOff(void) {
    GX_SetNumChans(1);
    GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
}

/**
 * Define what light to turn on and some other param.
 * @param id Light IDs of the desired switched ON lights (ORed) (ie GX_LIGHT0|GX_LIGHT7).
 * @param ambcol Ambiant color u32 formated
 * @param matcol Material color u32 formated.
 * @param colsrc Material color sources comes from the Vertex ???? (True/False)
*/
void GRRLIB_LightSwitch(u8 id, u32 ambcol, u32 matcol, u8 colsrc) {
    u8 src;
    if(colsrc==0) src = GX_SRC_REG;
    else src = GX_SRC_VTX;

    GX_SetNumChans(1);
    GX_SetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, src, id, GX_DF_CLAMP, GX_AF_SPOT);
    GX_SetChanAmbColor(GX_COLOR0A0, (GXColor) {  R(ambcol),  G(ambcol), B(ambcol), A(ambcol)});
    GX_SetChanMatColor(GX_COLOR0A0, (GXColor) {  R(matcol),  G(matcol), B(matcol), A(matcol)});
}

/**
 * Draw a torus (with normal).
 * @param r Radius of the ring.
 * @param R Radius of the torus.
 * @param nsides Number of faces per ring.
 * @param rings Number of rings.
 * @param filled Wired or not.
*/
void GRRLIB_DrawTorus(f32 r, f32 R, int nsides, int rings, bool filled) {
    int i, j;
    f32 theta, phi, theta1;
    f32 cosTheta, sinTheta;
    f32 cosTheta1, sinTheta1;
    f32 ringDelta, sideDelta;
    f32 cosPhi, sinPhi, dist;

    ringDelta = 2.0 * M_PI / rings;
    sideDelta = 2.0 * M_PI / nsides;

    theta = 0.0;
    cosTheta = 1.0;
    sinTheta = 0.0;
    for (i = rings - 1; i >= 0; i--) {
        theta1 = theta + ringDelta;
        cosTheta1 = cos(theta1);
        sinTheta1 = sin(theta1);
        if(filled) GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 2*(nsides+1));
        else       GX_Begin(GX_LINESTRIP, GX_VTXFMT0, 2*(nsides+1));
        phi = 0.0;
        for (j = nsides; j >= 0; j--) {
            phi += sideDelta;
            cosPhi = cos(phi);
            sinPhi = sin(phi);
            dist = R + r * cosPhi;

            GX_Position3f32(cosTheta1 * dist, -sinTheta1 * dist, r * sinPhi);
            GX_Normal3f32(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi);
            GX_Position3f32(cosTheta * dist, -sinTheta * dist,  r * sinPhi);
            GX_Normal3f32(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi);
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
 * @param lats Number of lattitudes.
 * @param longs Number of longitutes.
 * @param filled Wired or not.
*/
void GRRLIB_DrawSphere(f32 r, int lats, int longs, bool filled) {
    int i, j;
    f32 lat0, z0, zr0,
        lat1, z1, zr1,
        lng, x, y;

    for(i = 0; i <= lats; i++) {
        lat0 = M_PI * (-0.5F + (f32) (i - 1) / lats);
        z0  = sin(lat0);
        zr0 = cos(lat0);

        lat1 = M_PI * (-0.5F + (f32) i / lats);
        z1 = sin(lat1);
        zr1 = cos(lat1);
        if(filled) GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 2*(longs+1));
        else       GX_Begin(GX_LINESTRIP, GX_VTXFMT0, 2*(longs+1));
        for(j = 0; j <= longs; j++) {
            lng = 2 * M_PI * (f32) (j - 1) / longs;
            x = cos(lng);
            y = sin(lng);

            GX_Position3f32(x * zr0 * r, y * zr0 * r, z0 * r);
            GX_Normal3f32(x * zr0 * r, y * zr0 * r, z0 * r);
            GX_Position3f32(x * zr1 * r, y * zr1 * r, z1 * r);
            GX_Normal3f32(x * zr1 * r, y * zr1 * r, z1 * r);
        }
        GX_End();
    }
}

/**
 * Draw a cube (with normal).
 * @param size Size of the cube edge.
 * @param filled Wired or not.
*/
void GRRLIB_DrawCube(f32 size, bool filled)
{
    static f32 n[6][3] =
    {
        {-1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {1.0, 0.0, 0.0},
        {0.0, -1.0, 0.0},
        {0.0, 0.0, 1.0},
        {0.0, 0.0, -1.0}
    };
    static int faces[6][4] =
    {
        {0, 1, 2, 3},
        {3, 2, 6, 7},
        {7, 6, 5, 4},
        {4, 5, 1, 0},
        {5, 6, 2, 1},
        {7, 4, 0, 3}
    };
    f32 v[8][3];
    int i;

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

    for (i = 5; i >= 0; i--) {
        if(filled) GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
        else       GX_Begin(GX_LINESTRIP, GX_VTXFMT0, 5);
        GX_Position3f32(v[faces[i][0]][0], v[faces[i][0]][1], v[faces[i][0]][2] );
        GX_Normal3f32(n[i][0], n[i][1], n[i][2]);
        GX_Position3f32(v[faces[i][1]][0], v[faces[i][1]][1], v[faces[i][1]][2]);
        GX_Normal3f32(n[i][0], n[i][1], n[i][2]);
        GX_Position3f32(v[faces[i][2]][0], v[faces[i][2]][1], v[faces[i][2]][2]);
        GX_Normal3f32(n[i][0], n[i][1], n[i][2]);
        GX_Position3f32(v[faces[i][3]][0], v[faces[i][3]][1], v[faces[i][3]][2]);
        GX_Normal3f32(n[i][0], n[i][1], n[i][2]);
        if(!filled) {
            GX_Position3f32(v[faces[i][0]][0], v[faces[i][0]][1], v[faces[i][0]][2]);
            GX_Normal3f32(n[i][0], n[i][1], n[i][2]);
        }
        GX_End();
    }
}

/**
 * Draw a cylinder (with normal).
 * @param r Radius of the cylinder.
 * @param h High of the cylinder.
 * @param d Dencity of slice.
 * @param filled Wired or not.
*/
void GRRLIB_DrawCylinder(f32 r, f32 h, int d, bool filled) {
    int i;
    f32 dx, dy;

    if(filled) GX_Begin(GX_TRIANGLESTRIP, GX_VTXFMT0, 2 * (d+1));
    else       GX_Begin(GX_LINESTRIP, GX_VTXFMT0, 2 * (d+1));
    for(i = 0 ; i <= d ; i++) {
        dx = cosf( M_PI * 2.0f * i / d );
        dy = sinf( M_PI * 2.0f * i / d );
        GX_Position3f32( r * dx, -0.5f * h, r * dy );
        GX_Normal3f32( dx, 0.0f, dy );
        GX_Position3f32( r * dx, 0.5f * h, r * dy );
        GX_Normal3f32( dx, 0.0f, dy );
    }
    GX_End();

    if(filled) GX_Begin(GX_TRIANGLEFAN, GX_VTXFMT0, d+2);
    else       GX_Begin(GX_LINESTRIP, GX_VTXFMT0, d+2);
    GX_Position3f32(0.0f, -0.5f * h, 0.0f);
    GX_Normal3f32(0.0f, -1.0f, 0.0f);
    for(i = 0 ; i <= d ; i++) {
        GX_Position3f32( r * cosf( M_PI * 2.0f * i / d ), -0.5f * h, r * sinf( M_PI * 2.0f * i / d ) );
        GX_Normal3f32(0.0f, -1.0f, 0.0f);
    }
    GX_End();

    if(filled) GX_Begin(GX_TRIANGLEFAN, GX_VTXFMT0, d+2);
    else       GX_Begin(GX_LINESTRIP, GX_VTXFMT0, d+2);
    GX_Position3f32(0.0f, 0.5f * h, 0.0f);
    GX_Normal3f32(0.0f, 1.0f, 0.0f);
    for(i = 0 ; i <= d ; i++) {
        GX_Position3f32( r * cosf( M_PI * 2.0f * i / d ), 0.5f * h, r * sinf( M_PI * 2.0f * i / d ) );
        GX_Normal3f32(0.0f, 1.0f, 0.0f);
    }
    GX_End();
}
