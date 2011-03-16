/*===========================================
    NoNameNo
    Since i cant code all these new function alone,
    i really need help to find the best way to provide
    GRRLIB functions to all these new effets in this demo.
    With the help of the doc you can try also to find how it works,
    for exemple i didnt succes fully coded a specular position and direction....
    So i repeat i need HELP !!! ;)
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <wiiuse/wpad.h>

#include "gfx/font9x12.h"
#include "gfx/room.h"

extern Mtx _GRR_view;

int main() {
f32 a= 0.0f;
GXLightObj MyLight0;
GXLightObj MyLight1;
int demo=0;

Mtx mr,mv,rx,ry,rz,m;
GXTexObj  texObj;

    GRRLIB_Init();
    WPAD_Init();


    GRRLIB_texImg *tex_room = GRRLIB_LoadTexture(room);

    GRRLIB_texImg *tex_font = GRRLIB_LoadTexture(font9x12);
    GRRLIB_InitTileSet(tex_font, 9, 12, 32);

    GRRLIB_Settings.antialias = false;

    GRRLIB_SetBackgroundColour(0x40, 0x40, 0x40, 0xFF);

    while(1) {
	GRRLIB_Camera3dSettings(0.0f,0.0f,10.0f, 0,1,0, 0,0,0);

        GRRLIB_2dMode();
        WPAD_ScanPads();
        if(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);
        if(WPAD_ButtonsDown(0) & WPAD_BUTTON_PLUS) if(demo<5) demo++;
        if(WPAD_ButtonsDown(0) & WPAD_BUTTON_MINUS) if(demo>0) demo--;

        GRRLIB_3dMode(0.1,1000,45,0,1);

	if(demo==0){
		/////////////////// DEFINE A  DIFUSE LIGHT /////////////////////////////////////////////
		guVector l0pos={0.0f,20.0f,0.0f};
	    	guVecMultiply(_GRR_view, &l0pos, &l0pos);
	    	GX_InitLightPos(&MyLight0, l0pos.x, l0pos.y, l0pos.z);
	    	GX_InitLightColor(&MyLight0, (GXColor) { 0xFF, 0x00, 0x00, 0xFF });
	    	GX_InitLightSpot(&MyLight0, 0.0f, GX_SP_OFF);
	    	GX_InitLightDistAttn(&MyLight0, 20.0f, 1.0f, GX_DA_MEDIUM); // DistAttn = 20.0  &  Brightness=1.0f (full)
	    	GX_LoadLightObj(&MyLight0, GX_LIGHT0);

		/////////////////////// Turn light ON ////////////////////////////////////////////////
	    	GX_SetNumChans(1);
	    	GX_SetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_CLAMP,GX_AF_SPOT); //4th param is where come from the material color (REG(with setChanMatColor or VTX (vertex)) same for ambiant (3th param)

		/////////////////////// Define MAterial and Ambiant color and draw object /////////////////////////////////////
	        GRRLIB_ObjectView(0,0,0, a,a*2,a*3, 1.0f,1.0f,1.0f);
	    	GX_SetChanAmbColor(GX_COLOR0A0, (GXColor) { 0x33, 0x33, 0x33, 0xFF});
	    	GX_SetChanMatColor(GX_COLOR0A0, (GXColor) { 0xFF, 0xFF, 0xFF, 0xFF});
	        GRRLIB_DrawTorus(0.6f, 2.0f, 60, 60,true,0xFFFFFFFF);
		a+=0.8f;

		//////////////////////////// Turn light off and Write demo name
		GX_SetNumChans(1);
		GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        	GRRLIB_2dMode();
		GRRLIB_Printf(20, 30, tex_font, 0xFFFFFFFF, 1, "Simple Demo 1 Diffuse Light Source");
	}

	if(demo==1){
		/////////////////// DEFINE 2 DIFUSE LIGHTS /////////////////////////////////////////////
		guVector l0pos={0.0f,20.0f,0.0f};
	    	guVecMultiply(_GRR_view, &l0pos, &l0pos);
	    	GX_InitLightPos(&MyLight0, l0pos.x, l0pos.y, l0pos.z);
	    	GX_InitLightColor(&MyLight0, (GXColor) { 0x00, 0xFF, 0x00, 0xFF });
	    	GX_InitLightSpot(&MyLight0, 0.0f, GX_SP_OFF);
	    	GX_InitLightDistAttn(&MyLight0, 20.0f, 1.0f, GX_DA_MEDIUM); // DistAttn = 20.0  &  Brightness=1.0f (full)
	    	GX_LoadLightObj(&MyLight0, GX_LIGHT0);

		guVector l1pos={0.0f,-20.0f,0.0f};
	    	guVecMultiply(_GRR_view, &l1pos, &l1pos);
	    	GX_InitLightPos(&MyLight1, l1pos.x, l1pos.y, l1pos.z);
	    	GX_InitLightColor(&MyLight1, (GXColor) { 0x00, 0x00, 0xFF, 0xFF });
	    	GX_InitLightSpot(&MyLight1, 0.0f, GX_SP_OFF);
	    	GX_InitLightDistAttn(&MyLight1, 20.0f, 1.0f, GX_DA_MEDIUM); // DistAttn = 20.0  &  Brightness=1.0f (full)
	    	GX_LoadLightObj(&MyLight1, GX_LIGHT1);

		/////////////////////// Turn lights ON ////////////////////////////////////////////////
	    	GX_SetNumChans(1);
	    	GX_SetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0|GX_LIGHT1, GX_DF_CLAMP,GX_AF_SPOT); //4th param is where come from the material color (REG(with setChanMatColor or VTX (vertex)) same for ambiant (3th param)

		/////////////////////// Define MAterial and Ambiant color and draw object /////////////////////////////////////
	        GRRLIB_ObjectView(0,0,0, a,a*2,a*3, 1.0f,1.0f,1.0f);
	    	GX_SetChanAmbColor(GX_COLOR0A0, (GXColor) { 0x33, 0x33, 0x33, 0xFF});
	    	GX_SetChanMatColor(GX_COLOR0A0, (GXColor) { 0xFF, 0xFF, 0xFF, 0xFF});
	        GRRLIB_DrawTorus(0.6f, 2.0f, 60, 60,true, 0xFFFFFFFF);
		a+=0.8f;

		//////////////////////////// Turn light off and Write demo name
		GX_SetNumChans(1);
		GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        	GRRLIB_2dMode();
		GRRLIB_Printf(20, 30, tex_font, 0xFFFFFFFF, 1, "Simple Demo 2 Diffuse Light Sources");
	}

	if(demo==2){
		/////////////////// DEFINE A  SPECUALR LIGHT /////////////////////////////////////////////
		guVector l0dir={0.0f,0.0f,0.0f};
    		GX_InitSpecularDir(&MyLight0, l0dir.x,l0dir.y,l0dir.z);

		GX_InitLightShininess(&MyLight0, 20);  // entre 4 et 255 !!!
    		GX_InitLightColor(&MyLight0, (GXColor){0xFF,0xFF,0xFF,0xFF});
    		GX_LoadLightObj(&MyLight0, GX_LIGHT0);

		/////////////////////// Turn light ON ////////////////////////////////////////////////
		GX_SetNumChans(2);    // use two color channels
    		GX_SetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_CLAMP, GX_AF_NONE);
    		GX_SetChanCtrl(GX_COLOR1, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_NONE, GX_AF_SPEC);
    		GX_SetChanCtrl(GX_ALPHA0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHTNULL, GX_DF_NONE, GX_AF_NONE);
    		GX_SetChanCtrl(GX_ALPHA1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHTNULL, GX_DF_NONE, GX_AF_NONE);

  		GX_SetNumTevStages(2);
    		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0 );
    		GX_SetTevOrder(GX_TEVSTAGE1, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR1A1 );
    		GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    		GX_SetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV );
    		GX_SetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_ONE, GX_CC_RASC, GX_CC_ZERO );

		/////////////////////// Define MAterial and Ambiant color and draw object /////////////////////////////////////
	        GRRLIB_ObjectView(0,0,0, a,a*2,a*3, 1.0f,1.0f,1.0f);
    		GX_SetChanAmbColor(GX_COLOR0, (GXColor) {  0xFF,  0xFF, 0xFF, 0xFF});
    		GX_SetChanMatColor(GX_COLOR0, (GXColor) {  0x00,  0x00, 0x00, 0xFF});
    		GX_SetChanAmbColor(GX_COLOR1, (GXColor){0x00,0x00,0x00,0x00});
    		GX_SetChanMatColor(GX_COLOR1, (GXColor){0xFF,0xFF,0xFF,0xFF});
	        GRRLIB_DrawTorus(0.6f, 2.0f, 60, 60,true, 0xFFFFFFFF);
		a+=0.8f;

		//////////////////////////// Turn light off and Write demo name
		GX_SetNumChans(1);
		GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        	GRRLIB_2dMode();
		GRRLIB_Printf(20, 30, tex_font, 0xFFFFFFFF, 1, "Simple Demo 1 Specular Source with shininess = 20");
	}

	if(demo==3){
		/////////////////// DEFINE A  SPECUALR LIGHT /////////////////////////////////////////////
		guVector l0dir={0.0f,0.0f,0.0f};
    		GX_InitSpecularDir(&MyLight0, l0dir.x,l0dir.y,l0dir.z);

		GX_InitLightShininess(&MyLight0, 200);  // entre 4 et 255 !!!
    		GX_InitLightColor(&MyLight0, (GXColor){0xFF,0xFF,0xFF,0xFF});
    		GX_LoadLightObj(&MyLight0, GX_LIGHT0);

		/////////////////////// Turn light ON ////////////////////////////////////////////////
		GX_SetNumChans(2);    // use two color channels
    		GX_SetChanCtrl(GX_COLOR0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_CLAMP, GX_AF_NONE);
    		GX_SetChanCtrl(GX_COLOR1, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_NONE, GX_AF_SPEC);
    		GX_SetChanCtrl(GX_ALPHA0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHTNULL, GX_DF_NONE, GX_AF_NONE);
    		GX_SetChanCtrl(GX_ALPHA1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHTNULL, GX_DF_NONE, GX_AF_NONE);

  		GX_SetNumTevStages(2);
    		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0 );
    		GX_SetTevOrder(GX_TEVSTAGE1, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR1A1 );
    		GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    		GX_SetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV );
    		GX_SetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_ONE, GX_CC_RASC, GX_CC_ZERO );

		/////////////////////// Define MAterial and Ambiant color and draw object /////////////////////////////////////
	        GRRLIB_ObjectView(0,0,0, a,a*2,a*3, 1.0f,1.0f,1.0f);
    		GX_SetChanAmbColor(GX_COLOR0, (GXColor) {  0xFF,  0xFF, 0xFF, 0xFF});
    		GX_SetChanMatColor(GX_COLOR0, (GXColor) {  0x00,  0x00, 0x00, 0xFF});
    		GX_SetChanAmbColor(GX_COLOR1, (GXColor){0x00,0x00,0x00,0x00});
    		GX_SetChanMatColor(GX_COLOR1, (GXColor){0xFF,0xFF,0xFF,0xFF});
	        GRRLIB_DrawTorus(0.6f, 2.0f, 60, 60,true, 0xFFFFFFFF);
		a+=0.8f;

		//////////////////////////// Turn light off and Write demo name
		GX_SetNumChans(1);
		GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        	GRRLIB_2dMode();
		GRRLIB_Printf(20, 30, tex_font, 0xFFFFFFFF, 1, "Simple Demo 1 Specular Source with shininess = 200");
	}
	if(demo==4){
		GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_NRM, GX_TEXMTX0); // We say we use the Normal coord to map the texture (since there is no textcoord with this torus)
        	guLightFrustum(mv, 1.0F, -1.0F, 1.0F, -1.0F, 1.0F, 0.5F, 0.5F, 0.5F, 0.5F); // we are projecting the texture like a light (ie : videoprojector))

        	guMtxScale(mr, -2.8f, -2.8f, 0.0f); //here is a little scaling to fit the torus
        	guMtxConcat(mr, mv, mv);

        	GX_LoadTexMtxImm(mv, GX_TEXMTX0, GX_MTX3x4);
        	GX_InitTexObj(&texObj, tex_room->data, tex_room->w, tex_room->h, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
        	GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_4);
        	GX_SetNumTexGens(1);
        	GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
        	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        	GX_LoadTexObj(&texObj, GX_TEXMAP0);

	        GRRLIB_ObjectView(0,0,0, a,a*2,a*3, 1.0f,1.0f,1.0f);
	        GRRLIB_DrawTorus(0.6f, 2.0f, 60, 60,true, 0xFFFFFFFF);
		a+=0.8f;

        	GRRLIB_2dMode();
		GRRLIB_Printf(20, 30, tex_font, 0xFFFFFFFF, 1, "Simple Demo Mapping Using textgen from normal coord");
	}
	if(demo==5){
		GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_NRM, GX_TEXMTX0); // We say we use the Normal coord to map the texture (since there is no textcoord with this torus)
        	guLightFrustum(mv, 1.0F, -1.0F, 1.0F, -1.0F, 1.0F, 0.5F, 0.5F, 0.5F, 0.5F); // we are projecting the texture like a light (ie : videoprojector))
        	guMtxRotDeg(rx, 'X', a);     //  Here i rotate the texture
        	guMtxRotDeg(ry, 'Y', a*2);   //  in the inverse way
        	guMtxRotDeg(rz, 'Z', a*3);   //  of the
        	guMtxConcat(ry, rx, m);      //  rotation of
        	guMtxConcat(m, rz, m);       //  the object
        	guMtxConcat(mv, m, mv);      //  make it show the texture not moving.... ;)

        	guMtxScale(mr, -2.8f, -2.8f, 0.0f); //here is a little scaling to fit the torus
        	guMtxConcat(mr, mv, mv);

        	GX_LoadTexMtxImm(mv, GX_TEXMTX0, GX_MTX3x4);
        	GX_InitTexObj(&texObj, tex_room->data, tex_room->w, tex_room->h, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
        	GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_4);
        	GX_SetNumTexGens(1);
        	GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
        	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        	GX_LoadTexObj(&texObj, GX_TEXMAP0);

	        GRRLIB_ObjectView(0,0,0, a,a*2,a*3, 1.0f,1.0f,1.0f);
	        GRRLIB_DrawTorus(0.6f, 2.0f, 60, 60,true, 0xFFFFFFFF);
		a+=0.8f;

        	GRRLIB_2dMode();
		GRRLIB_Printf(20, 30, tex_font, 0xFFFFFFFF, 1, "Simple Demo Environmental Mapping");
	}


        GRRLIB_Render();
    }
    GRRLIB_FreeTexture(tex_font);
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB

    exit(0);
}

