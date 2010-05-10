/*===========================================
        GRRLIB 
	Spot Light Sample Code
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <wiiuse/wpad.h>

#include "font.h"

extern Mtx _GRR_view;

int main(int argc, char **argv) {
 f32 lightx=0.0f;

    GRRLIB_Init();

    GRRLIB_texImg *tex_font = GRRLIB_LoadTexture(font);
    GRRLIB_InitTileSet(tex_font, 16, 16, 32);

    WPAD_Init();

    GRRLIB_SetBackgroundColour(0x40, 0x40, 0x40, 0xFF);

    while(1) {
	GRRLIB_2dMode();
        WPAD_ScanPads();
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME)  break;

	GRRLIB_Camera3dSettings(0.0f,0.0f,3.0f, 0,1,0, 0,0,0);

	GRRLIB_SetLightAmbient(0x404040FF);

	GRRLIB_SetLightSpot(1, (guVector){ sin(lightx)*2.5f, 0.8f, 0 }, (guVector){  sin(lightx)*2.5f, 0.0f, 0.0f }, -4.0f, 5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0x0000FFFF);
	GRRLIB_SetLightSpot(2, (guVector){ -sin(lightx)*2.5f, 0.8f, 0 }, (guVector){  -sin(lightx)*2.5f, 0.0f, 0.0f }, -4.0f, 5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0xFF0000FF);

	GRRLIB_3dMode(0.1,1000,45,0,1);
    	GRRLIB_ObjectView(0,-0.8,0, -90,0,0,1,1,1);
    	GRRLIB_DrawTessPanel(6.2f,0.17f,3.7f,0.1f,0,0xFFFFFFFF);

	lightx+=0.05f;

	GRRLIB_2dMode();
        GRRLIB_Printf((640-(16*28))/2, 480-16, tex_font, 0xFFFFFFFF, 1, "GRRLIB SPOT LIGHT SAMPLE 1");


        GRRLIB_Render();
    }

    GRRLIB_Exit();

    exit(0);
}
