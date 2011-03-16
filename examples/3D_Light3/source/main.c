/*===========================================
    NoNameNo

    A little Specular light sample code
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <wiiuse/wpad.h>

#include "gfx/font9x12.h"


int main() {
    f32 rot = 0.0f;
    float shy = 10.0f;

    GRRLIB_Init();
    WPAD_Init();

    GRRLIB_texImg *tex_font = GRRLIB_LoadTexture(font9x12);
    GRRLIB_InitTileSet(tex_font, 9, 12, 32);

    GRRLIB_Settings.antialias = true;

    GRRLIB_SetBackgroundColour(0x40, 0x40, 0x40, 0xFF);

    while(1) {
        GRRLIB_Camera3dSettings(0.0f,0.0f,10.0f, 0,1,0, 0,0,0);

        WPAD_ScanPads();

        if(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) break;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_A) shy+=1;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_B) shy-=1;

        GRRLIB_3dMode(0.1,1000,45,0,1);

        // dir param of GRRLIB_SetLightSpec function have to be a crazy 0,0,0 value
        // there is a bug in libogc about this
        // http://forums.devkitpro.org/viewtopic.php?f=7&t=1933
        // we are waiting for a fix from libogc devs
        GRRLIB_SetLightAmbient(0x404040FF);
        GRRLIB_SetLightSpec(0, (guVector){0.0f,0.0f,0.0f}, shy, 0xFFFFFFFF, 0xFFFFFFFF);

        GRRLIB_ObjectViewBegin();
        GRRLIB_ObjectViewTrans(0.0f,1.3f,0.0f);
        GRRLIB_ObjectViewRotate(rot,rot*2,rot*3);
        GRRLIB_ObjectViewEnd();
        GRRLIB_DrawCone(0.6f, 2.6f, 60,true, 0x502010FF);

        GRRLIB_ObjectViewBegin();
        GRRLIB_ObjectViewRotate(0.0f,0.0f,90.0f);
        GRRLIB_ObjectViewTrans(-1.3f,0.0f,0.0f);
        GRRLIB_ObjectViewRotate(rot,rot*2,rot*3);
        GRRLIB_ObjectViewEnd();
        GRRLIB_DrawCone(0.6f, 2.6f, 60,true, 0x502010FF);

        GRRLIB_ObjectViewBegin();
        GRRLIB_ObjectViewRotate(0.0f,0.0f,180.0f);
        GRRLIB_ObjectViewTrans(0.0f,-1.3f,0.0f);
        GRRLIB_ObjectViewRotate(rot,rot*2,rot*3);
        GRRLIB_ObjectViewEnd();
        GRRLIB_DrawCone(0.6f, 2.6f, 60,true, 0x502010FF);

        GRRLIB_ObjectViewBegin();
        GRRLIB_ObjectViewRotate(0.0f,0.0f,-90.0f);
        GRRLIB_ObjectViewTrans(1.3f,0.0f,0.0f);
        GRRLIB_ObjectViewRotate(rot,rot*2,rot*3);
        GRRLIB_ObjectViewEnd();
        GRRLIB_DrawCone(0.6f, 2.6f, 60,true, 0x502010FF);

        GRRLIB_ObjectViewBegin();
        GRRLIB_ObjectViewRotate(-90.0f,0.0f,0.0f);
        GRRLIB_ObjectViewTrans(0.0f,0.0f,-1.3f);
        GRRLIB_ObjectViewRotate(rot,rot*2,rot*3);
        GRRLIB_ObjectViewEnd();
        GRRLIB_DrawCone(0.6f, 2.6f, 60,true, 0x502010FF);

        GRRLIB_ObjectViewBegin();
        GRRLIB_ObjectViewRotate(90.0f,0.0f,0.0f);
        GRRLIB_ObjectViewTrans(0.0f,0.0f,1.3f);
        GRRLIB_ObjectViewRotate(rot,rot*2,rot*3);
        GRRLIB_ObjectViewEnd();
        GRRLIB_DrawCone(0.6f, 2.6f, 60,true, 0x502010FF);

        GRRLIB_ObjectViewBegin();
        GRRLIB_ObjectViewRotate(rot,rot*2,rot*3);
        GRRLIB_ObjectViewEnd();
        GRRLIB_DrawCube( 0.6f, true, 0x102050FF);


        rot+=0.8f;

        GRRLIB_2dMode();
        GRRLIB_Printf(50, 60, tex_font, 0xFFFFFFFF, 1, "Use ( A / B ) to change the shyniness value : %d ",(int)shy);
        GRRLIB_Render();
    }
    GRRLIB_FreeTexture(tex_font);
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB

    exit(0);
}

