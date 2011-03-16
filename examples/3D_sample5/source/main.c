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

int main(void){
    f32 a = 0;
    f32 offset = 1.5f;
    f32 rotpas = 60.0f;

    GRRLIB_Init();
    WPAD_Init();

    GRRLIB_texImg *tex_font = GRRLIB_LoadTexture(font);
    GRRLIB_InitTileSet(tex_font, 16, 16, 32);

    GRRLIB_Settings.antialias = true;

    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);
    GRRLIB_Camera3dSettings(0.0f,0.0f,5.0f, 0,1,0, 0,0,0);


    while(1) {
        GRRLIB_2dMode();
        WPAD_ScanPads();
        if(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);


        GRRLIB_3dMode(0.1,1000,45,0,1);
        GRRLIB_SetLightAmbient(0x111111FF);
        GRRLIB_SetLightDiff(0,(guVector){0.0f,3.0f,3.0f},20.0f,1.0f,0xFFFFFFFF);


        GRRLIB_ObjectViewBegin();
        GRRLIB_ObjectViewRotate(0,a*3,0);
        GRRLIB_ObjectViewTrans(0,offset,0);
        GRRLIB_ObjectViewRotate(a,a*3,a*2+0*rotpas);
        GRRLIB_ObjectViewEnd();
        GRRLIB_DrawCube(0.8f,1,0xFFFFFFFF);

        GRRLIB_ObjectViewBegin();
        GRRLIB_ObjectViewRotate(0,a*3,0);
        GRRLIB_ObjectViewTrans(0,offset,0);
        GRRLIB_ObjectViewRotate(a,a*3,a*2+1*rotpas);
        GRRLIB_ObjectViewEnd();
        GRRLIB_DrawCube(0.8f,1,0xFFFFFFFF);


        GRRLIB_ObjectViewBegin();
        GRRLIB_ObjectViewRotate(0,a*3,0);
        GRRLIB_ObjectViewTrans(0,offset,0);
        GRRLIB_ObjectViewRotate(a,a*3,a*2+2*rotpas);
        GRRLIB_ObjectViewEnd();
        GRRLIB_DrawCube(0.8f,1,0xFFFFFFFF);

        GRRLIB_ObjectViewBegin();
        GRRLIB_ObjectViewRotate(0,a*3,0);
        GRRLIB_ObjectViewTrans(0,offset,0);
        GRRLIB_ObjectViewRotate(a,a*3,a*2+3*rotpas);
        GRRLIB_ObjectViewEnd();
        GRRLIB_DrawCube(0.8f,1,0xFFFFFFFF);

        GRRLIB_ObjectViewBegin();
        GRRLIB_ObjectViewRotate(0,a*3,0);
        GRRLIB_ObjectViewTrans(0,offset,0);
        GRRLIB_ObjectViewRotate(a,a*3,a*2+4*rotpas);
        GRRLIB_ObjectViewEnd();
        GRRLIB_DrawCube(0.8f,1,0xFFFFFFFF);

        GRRLIB_ObjectViewBegin();
        GRRLIB_ObjectViewRotate(0,a*3,0);
        GRRLIB_ObjectViewTrans(0,offset,0);
        GRRLIB_ObjectViewRotate(a,a*3,a*2+5*rotpas);
        GRRLIB_ObjectViewEnd();
        GRRLIB_DrawCube(0.8f,1,0xFFFFFFFF);

        GRRLIB_ObjectViewBegin();
        GRRLIB_ObjectViewRotate(0,a*3,0);
        GRRLIB_ObjectViewTrans(0,offset,0);
        GRRLIB_ObjectViewRotate(a,a*3,a*2+6*rotpas);
        GRRLIB_ObjectViewEnd();
        GRRLIB_DrawCube(0.8f,1,0xFFFFFFFF);

        a+=0.8;
        GRRLIB_2dMode();

        // Switch To 2D Mode to display text
        GRRLIB_2dMode();
        GRRLIB_Printf((640-(16*29))/2, 20, tex_font, 0xFFFFFFFF, 1, "JUST ANOTHER 3D SAMPLE");
        GRRLIB_Render();
    }
    GRRLIB_FreeTexture(tex_font);
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB

    exit(0);
}

