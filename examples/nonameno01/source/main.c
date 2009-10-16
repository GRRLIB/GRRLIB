/*===========================================
        GRRLIB (GX Version)
        Example code by NoNameNo

        This code shows how to draw
        vectors using GRRLIB_DrawImgQuad.
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <math.h>
#include <wiiuse/wpad.h>

#include "gfx/pirate.h"


int main() {
    u32 wpaddown;

    guVector ln[4];
    guVector d[4];
    float xt ,yt, zt;
    int i;
    float a=0.0, b=0.0;
    float c=2;
    guVector l[4]={{-0.8,-0.8,0},{0.8,-0.8,0},{0.8,0.8,0},{-0.8,0.8,0}};

    GRRLIB_Init();
    WPAD_Init();
    GRRLIB_texImg *tex_pirate = GRRLIB_LoadTexture(pirate);

    while(1) {
        WPAD_ScanPads();
        wpaddown = WPAD_ButtonsDown(0);

        a+=0.04;
        b+=0.02;
        for(i=0;i<4;i++){
            xt=l[i].x*cos(a)-l[i].z*sin(a);
            yt=l[i].y;
            zt=l[i].x*sin(a)+l[i].z*cos(a);
            ln[i].x=xt;
            ln[i].y=yt*cos(b)-zt*sin(b);
            ln[i].z=yt*sin(b)+zt*cos(b)+c;
            d[i].x=(640/2)+(480*ln[i].x)/(2*ln[i].z);
            d[i].y=(480/2)+(480*ln[i].y)/(2*ln[i].z);
        }

        GRRLIB_FillScreen(0xFFFFFFFF);
        GRRLIB_DrawImgQuad(d, tex_pirate, 0xFFFFFFFF);


        GRRLIB_Render();
        if(wpaddown & WPAD_BUTTON_HOME) {
            exit(0);
        }
    }
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB
    GRRLIB_FreeTexture(tex_pirate);
    return 0;
}
