#include <grrlib.h>

/**
 * Set the BackGround Parameter when screen is cleared.
 * @param r Red component.
 * @param g Green component.
 * @param b Blue component.
 * @param a Alpha component.
 */
void GRRLIB_setBackgroundColour(u8 r, u8 g, u8 b, u8 a) {
   GX_SetCopyClear((GXColor){ r, g, b, a }, GX_MAX_Z24); 
}

// user should not directly modify these
guVector _GRR_cam = {0.0F, 0.0F, 0.0F},
         _GRR_up  = {0.0F, 1.0F, 0.0F},
         _GRR_look   = {0.0F, 0.0F, -100.0F};
Mtx _GRR_view;

/**
 * Set the camera parameter (contributed my chris_c aka DaShAmAn).
 * @param posx x posision of the cam.
 * @param posy y posision of the cam.
 * @param posz z posision of the cam.
 * @param upx Alpha component.
 * @param upy Alpha component.
 * @param upz Alpha component.
 * @param lookx x up posision of the cam.
 * @param looky y up posision of the cam.
 * @param lookz z up posision of the cam.
 */
void GRRLIB_camera3dSettings(f32 posx, f32 posy, f32 posz, 
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
 * Set up the position matrix (contributed my chris_c aka DaShAmAn).
 * @param miniDist Minimal distance for the cam.
 * @param maxDist Maximal distance for the cam.
 * @param fov Field of view for the cam.
 * @param texturemode False, GX won't need TexCoord, True, GX will need TexCoord.
 */
void GRRLIB_3dMode(f32 minDist,f32 maxDist,f32 fov, bool texturemode) {
    Mtx m;

    guLookAt(_GRR_view, &_GRR_cam, &_GRR_up, &_GRR_look);   
    guPerspective(m, fov, (f32)rmode->fbWidth/rmode->efbHeight, minDist, maxDist);
    GX_LoadProjectionMtx(m, GX_PERSPECTIVE);     
    GX_SetZMode (GX_TRUE, GX_LEQUAL, GX_TRUE);

    GX_SetCullMode(GX_CULL_NONE);

    GX_ClearVtxDesc();
    GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);

    if(texturemode==FALSE) GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);
    if(texturemode==TRUE) GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    if(texturemode==FALSE) GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    if(texturemode==TRUE) GX_SetTevOp(GX_TEVSTAGE0, GX_REPLACE);
}

/**
 * Go back to 2D Mode (contributed my chris_c aka DaShAmAn).
 */
void GRRLIB_2dMode() {
    Mtx view,m;

    GX_SetZMode(GX_FALSE, GX_LEQUAL, GX_TRUE);

    GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

    guOrtho(m, 0, rmode->efbHeight, 0, rmode->fbWidth, 0, 1000.0f);
    GX_LoadProjectionMtx(m, GX_ORTHOGRAPHIC);

    guMtxIdentity(view);
    guMtxTransApply(view,view, 0,0, -100.0F);
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

static  guVector  _GRRaxisx = (guVector){1,0,0}; // DO NOT MODIFY!!!
static  guVector  _GRRaxisy = (guVector){0,1,0}; // Even at runtime
static  guVector  _GRRaxisz = (guVector){0,0,1}; // NOT ever!

/**
 * Set the view matrix to draw object (contributed my chris_c aka DaShAmAn).
 * @param posx x posision of the object.
 * @param posy y posision of the object.
 * @param posz z posision of the object.
 * @param angx x rotation angle of the object.
 * @param angy y rotation angle of the object.
 * @param angz z rotation angle of the object.
 */
void GRRLIB_objectView(f32 posx, f32 posy, f32 posz, f32 angx, f32 angy, f32 angz) {
    Mtx m,mv,rx,ry,rz;
    Mtx mvi ;

    guMtxIdentity(m);
    guMtxRotAxisDeg(rx, &_GRRaxisx, angx);
    guMtxRotAxisDeg(ry, &_GRRaxisy, angy);
    guMtxRotAxisDeg(rz, &_GRRaxisz, angz);
    guMtxConcat(ry,rx,m);
    guMtxConcat(m,rz,m);
			
    guMtxTransApply(m, m, posx,posy,posz);
    guMtxConcat(_GRR_view,m,mv);
    GX_LoadPosMtxImm(mv, GX_PNMTX0);

    guMtxInverse(mv,mvi);
    guMtxTranspose(mvi,mv);
    GX_LoadNrmMtxImm(mv, GX_PNMTX0);
}

/**
 * Set the texture to an object (contributed my chris_c aka DaShAmAn).
 * @param tex poiter to an image texture (GRRLIB_texImg format).
 * @param rep Texture Repeat Mode, True will repeat it, False won't.
*/
void GRRLIB_setTexture(GRRLIB_texImg *tex, bool rep) {
    GXTexObj  texObj;

    if (rep) {
        GX_InitTexObj(&texObj, tex->data, tex->w, tex->h,GX_TF_RGBA8, GX_REPEAT, GX_REPEAT, GX_FALSE);
    } 
    else {
        GX_InitTexObj(&texObj, tex->data, tex->w, tex->h,GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    }
    if (GRRLIB_Settings.antialias == false){
        GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
        GX_SetCopyFilter(GX_FALSE, rmode->sample_pattern, GX_FALSE, rmode->vfilter);
    }
    else{
        GX_SetCopyFilter(rmode->aa, rmode->sample_pattern, GX_TRUE, rmode->vfilter);
    }

    GX_LoadTexObj(&texObj,      GX_TEXMAP0);
    GX_SetTevOp  (GX_TEVSTAGE0, GX_MODULATE);
    GX_SetVtxDesc(GX_VA_TEX0,   GX_DIRECT);
}

