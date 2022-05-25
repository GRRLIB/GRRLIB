/*===========================================
    NoNameNo
    Simple Lights and GRRLIB_ObjectViewInv sample
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <wiiuse/wpad.h>

#include "Rockwell_Condensed_12_Bold_png.h"


int main() {
    float a = 0;
    float objscal = 0.5f;
    int objqual = 20;

    GRRLIB_Init();
    WPAD_Init();

    GRRLIB_texImg *tex_font = GRRLIB_LoadTexture(Rockwell_Condensed_12_Bold_png);
    GRRLIB_InitTileSet(tex_font, 12, 19, 32);


    GRRLIB_Settings.antialias = true;

    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);


    while(1) {
        GRRLIB_2dMode();
        WPAD_ScanPads();
        if(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) break;

        GRRLIB_Camera3dSettings(0.0f, 0.0f,13.0f, 0,1,0, 0,0,0);

        GRRLIB_SetLightAmbient(0x333333FF);
        GRRLIB_SetLightDiff(0,(guVector){0.0f,0.0f,0.0f},20.0f,1.0f,0x00FFFFFF);
        GRRLIB_SetLightDiff(1,(guVector){0.0f,13.0f,3.0f},20.0f,1.0f,0xFF00FFFF);
        GRRLIB_SetLightDiff(2,(guVector){0.0f,-13.0f,3.0f},20.0f,1.0f,0xFFFF00FF);
        GRRLIB_SetLightDiff(3,(guVector){13.0f,0.0f,3.0f},20.0f,1.0f,0xFF0000FF);
        GRRLIB_SetLightDiff(4,(guVector){-13.0f,0.0f,3.0f},20.0f,1.0f,0x00FF00FF);

        GRRLIB_3dMode(0.1,1000,45,0,1);

        GRRLIB_ObjectViewInv(1.0f, -1.0f, 1.0f, a, a*2, a*3, 1.0f, 1.0f, 1.0f);
        GRRLIB_DrawSphere(objscal,objqual,objqual,true,0xFFFFFFFF);
        GRRLIB_ObjectViewInv(-1.0f, -1.0f, 1.0f, a, a*2, a*3, 1.0f, 1.0f, 1.0f);
        GRRLIB_DrawSphere(objscal,objqual,objqual,true,0xFFFFFFFF);
        GRRLIB_ObjectViewInv(1.0f, 1.0f, 1.0f, a, a*2, a*3, 1.0f, 1.0f, 1.0f);
        GRRLIB_DrawSphere(objscal,objqual,objqual,true,0xFFFFFFFF);
        GRRLIB_ObjectViewInv(-1.0f, 1.0f, 1.0f, a, a*2, a*3, 1.0f, 1.0f, 1.0f);
        GRRLIB_DrawSphere(objscal,objqual,objqual,true,0xFFFFFFFF);

        GRRLIB_ObjectViewInv(1.0f, -1.0f, -1.0f, a, a*2, a*3, 1.0f, 1.0f, 1.0f);
        GRRLIB_DrawSphere(objscal,objqual,objqual,true,0xFFFFFFFF);
        GRRLIB_ObjectViewInv(-1.0f, -1.0f, -1.0f, a, a*2, a*3, 1.0f, 1.0f, 1.0f);
        GRRLIB_DrawSphere(objscal,objqual,objqual,true,0xFFFFFFFF);
        GRRLIB_ObjectViewInv(1.0f, 1.0f, -1.0f, a, a*2, a*3, 1.0f, 1.0f, 1.0f);
        GRRLIB_DrawSphere(objscal,objqual,objqual,true,0xFFFFFFFF);
        GRRLIB_ObjectViewInv(-1.0f, 1.0f, -1.0f, a, a*2, a*3, 1.0f, 1.0f, 1.0f);
        GRRLIB_DrawSphere(objscal,objqual,objqual,true,0xFFFFFFFF);

        GRRLIB_ObjectView(0.0f, 0.0f, -1.0f, a, a*2, a*3, 1.0f, 1.0f, 1.0f);
        GRRLIB_DrawCube(3.0, true, 0xFFFFFF44);

        a+=0.6f;

        // Switch To 2D Mode to display text
        GRRLIB_2dMode();
        GRRLIB_Printf((640-(16*29))/2, 20, tex_font, 0xFFFFFFFF, 1, "LIGHT SAMPLE CODE 2");

        GRRLIB_Render();
    }
    GRRLIB_FreeTexture(tex_font);
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB

    exit(0);
}
