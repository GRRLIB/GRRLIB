/*===========================================
    NoNameNo
    Simple Diffuse light sample code
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <math.h>
#include <wiiuse/wpad.h>


#include "gfx/font.h"

extern Mtx _GRR_view;

int main() {
    float l1=0, l2=0;
    float a=0;
    int camZ=13.0f;

    GRRLIB_Init();
    WPAD_Init();


    GRRLIB_texImg *tex_font = GRRLIB_LoadTexture(font);
    GRRLIB_InitTileSet(tex_font, 16, 16, 32);


    GRRLIB_Settings.antialias = true;

    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);

    while(1) {
        GRRLIB_2dMode();
        WPAD_ScanPads();
        if(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) break;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_PLUS) camZ++;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_MINUS) camZ--;

        GRRLIB_Camera3dSettings(0.0f,0.0f,camZ, 0,1,0, 0,0,0);
        GRRLIB_SetLightAmbient(0x333333FF);

        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_A) {
            // Set all light off to get the spere no light sourced (only get the vertex color)
            GRRLIB_SetLightOff();
            GRRLIB_3dMode(0.1,1000,45,0,1);
            GRRLIB_ObjectView(sin(l1)*4.0f,0.0f,cos(l1)*4.0f, 0,0,0,1,1,1);
            GRRLIB_DrawSphere(0.2f, 20, 20, true, 0xFF0000FF);
            l1+=0.03f;
        }

        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_B) {
            // Set all light off to get the spere no light sourced (only get the vertex color)
            GRRLIB_SetLightOff();
            GRRLIB_3dMode(0.1,1000,45,0,1);
            GRRLIB_ObjectView(0.0f,sin(l2)*4.0f,cos(l2)*4.0f, 0,0,0,1,1,1);
            GRRLIB_DrawSphere(0.2f, 20, 20, true, 0x00FF00FF);
            l2+=0.05f;
        }

        // Set a dummy black light to get the ambiant one when no light is selected
        GRRLIB_SetLightDiff(0,(guVector){sin(l1)*4.0f,0.0f,cos(l1)*4.0f},20.0f,1.0f,0x000000FF);

        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_A) {
            GRRLIB_SetLightDiff(0,(guVector){sin(l1)*4.0f,0.0f,cos(l1)*4.0f},20.0f,1.0f,0xFF0000FF);
        }

        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_B) {
            GRRLIB_SetLightDiff(1,(guVector){0.0f,sin(l2)*4.0f,cos(l2)*4.0f},20.0f,1.0f,0x00FF00FF);
        }


        GRRLIB_3dMode(0.1,1000,45,0,1);

        GRRLIB_ObjectView(0,0,0, a,a*2,a*3,1,1,1);
        GRRLIB_DrawTorus(1, 2, 60, 60, true, 0xFFFFFFFF);

        a+=0.5f;

        // Switch to 2D Mode to display text
        GRRLIB_2dMode();
        GRRLIB_Printf((640-(16*29))/2, 20, tex_font, 0xFFFFFFFF, 1, "PRESS + OR - TO ZOOM");
        GRRLIB_Printf((640-(16*29))/2, 40, tex_font, 0xFFFFFFFF, 1, "HOLD A - RED / B - GREEN");

        GRRLIB_Render();
    }
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB
    GRRLIB_FreeTexture(tex_font);

    exit(0);
}
