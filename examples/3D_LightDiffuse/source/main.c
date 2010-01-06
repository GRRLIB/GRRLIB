/*===========================================
    NoNameNo
    Simple Flat 3D cube
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <wiiuse/wpad.h>

#include "gfx/font.h"

extern Mtx _GRR_view;


int main() {
    float a=0;
    u8 Amb=0x80;
    f32 zlight=0.0f;

    GRRLIB_Init();
    WPAD_Init();


    GRRLIB_texImg *tex_font = GRRLIB_LoadTexture(font);
    GRRLIB_InitTileSet(tex_font, 16, 16, 32);

    GRRLIB_Settings.antialias = true;

    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);
    GRRLIB_Camera3dSettings(0.0f,0.0f,13.0f, 0,1,0, 0,0,0);

    while(1) {
        GRRLIB_2dMode();
        WPAD_ScanPads();
        if(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_PLUS){if(Amb<255) Amb++; }
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_MINUS){if(Amb>0) Amb--; }
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_A){zlight++; }
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_B){zlight--; }


        GRRLIB_3dMode(0.1,1000,45,0,0,1);

	GRRLIB_InitLight(GX_LIGHT0, (guVector){-6, 0, zlight}, 0xFF0000FF);
	GRRLIB_InitLight(GX_LIGHT1, (guVector){6, 0, zlight}, 0x00FF00FF);
	GRRLIB_InitLight(GX_LIGHT2, (guVector){0, -6, zlight}, 0x0000FFFF);

	GRRLIB_LightSwitch(GX_LIGHT0|GX_LIGHT1|GX_LIGHT2,RGBA(Amb,Amb,Amb,0xFF),0x808080FF,0);

        GRRLIB_ObjectView(0,0,0, a,a*2,a*3, 1,1,1);
	GRRLIB_DrawTorus(0.4f, 2.0f, 20, 50, 0);
        GRRLIB_ObjectView(0,0,0, -a,-a*2,-a*3, 1,1,1);
	GRRLIB_DrawTorus(0.8f, 4.0f, 20, 50, 1);

        a+=0.5f;

	GRRLIB_LightOff();
        // Switch To 2D Mode to display text
        GRRLIB_2dMode();
        GRRLIB_Printf((640-(16*40))/2, 20, tex_font, 0xFFFFFFFF, 1, "PRESS PLUS OR MINUS TO PLAY WITH AMBIENT");
        GRRLIB_Printf((640-(16*15))/2, 36, tex_font, 0xFFFFFFFF, 1, "AMBIENT = 0X%02X%02X%02XFF",Amb,Amb,Amb,Amb);
        GRRLIB_Printf((640-(16*15))/2, 52, tex_font, 0xFFFFFFFF, 1, "ZLIGHT = %f",zlight);

        GRRLIB_Render();
    }
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB
    GRRLIB_FreeTexture(tex_font);

    exit(0);
}

