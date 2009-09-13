/*===========================================
    NoNameNo FAKE Unlimited Sprites rout ;)
    Bugged since Screen2texture conversion
    loose color precision.
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <wiiuse/wpad.h>

#include "gfx/ball.h"

extern GXRModeObj *rmode;

int main() {
    u32 WPADDown;
    float sinx[4]={0,0,0,0},siny[4]={0,0,0,0};
    int i;



    GRRLIB_Init();
    WPAD_Init();


    GRRLIB_texImg *tex_screen[10];
    for(i=0;i<10;i++){
        tex_screen[i] = GRRLIB_CreateEmptyTexture(rmode->fbWidth,rmode->efbHeight);
    }

    GRRLIB_texImg *tex_ball = GRRLIB_LoadTexture(ball);

    while(1) {
        WPAD_ScanPads();
        WPADDown = WPAD_ButtonsDown(0);

        for(i=0;i<10;i++){
            GRRLIB_DrawImg(0,0, tex_screen[i] , 0, 1, 1, 0xFFFFFFFF);
            GRRLIB_DrawImg(((640-64)/2)+sin(sinx[0])*160+sin(sinx[1])*sin(sinx[2])*50,((480-64)/2)+sin(siny[0])*120+sin(siny[1])*sin(siny[2])*50, tex_ball , 0, 1, 1, 0xFFFFFFFF);
            sinx[0]+=0.02;sinx[1]+=0.03;sinx[2]+=0.05;siny[0]+=0.03;siny[1]+=0.01;siny[2]+=0.06;
            GRRLIB_Screen2Texture(tex_screen[i]);
            GRRLIB_Render();
        }


        if(WPADDown & WPAD_BUTTON_HOME) {
            exit(0);
        }
    }
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB
    return 0;
}
