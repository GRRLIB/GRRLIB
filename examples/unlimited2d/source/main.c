/*===========================================
    NoNameNo FAKE Unlimited 2D Sprites rout ;)
    It's a lame but Well Known technic....
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <wiiuse/wpad.h>


#include "gfx/logo.h"
#include "gfx/font.h"
#include "gfx/ball.h"

#define PI    3.14159265358979323846F

extern GXRModeObj *rmode;

int main() {
    int i;
    int screen_index = 0;
    float t=0;
    int R=81;
    int r=71;
    int d=120;
    float f=0;
    float ff=0;
    float spr=0;
    int n=1;


    GRRLIB_Init();
    WPAD_Init();

    GRRLIB_Settings.antialias = false;

    GRRLIB_texImg *tex_screen[10];
    for(i=0; i<10; i++) {
        tex_screen[i] = GRRLIB_CreateEmptyTexture(rmode->fbWidth, rmode->efbHeight);
    }

    GRRLIB_texImg *tex_ball = GRRLIB_LoadTexture(ball);
    GRRLIB_texImg *tex_logo = GRRLIB_LoadTexture(logo);
    GRRLIB_texImg *tex_font = GRRLIB_LoadTexture(font);
    GRRLIB_InitTileSet(tex_font, 16, 16, 32);


    for(i=0;i<=255;i+=1){
        GRRLIB_Printf((640-(16*16))/2, 200, tex_font, 0xFFFFFF00|i, 1, "HOW MANY SPRITES");
        GRRLIB_Printf((640-(16*20))/2, 216, tex_font, 0xFFFFFF00|i, 1, "CAN YOU DISPLAY WITH");
        GRRLIB_DrawImg((640-352)/2, 248, tex_logo, 0, 1, 1, 0xFFFFFF00|i);
        GRRLIB_Printf((640-(16*28))/2, 480-16, tex_font, 0xFFFFFF00|i, 1, "BY NONAMENO FROM GRRLIB TEAM");
        GRRLIB_Render();
    }
    for(i=255;i>=0;i-=2){
        GRRLIB_Printf((640-(16*16))/2, 200, tex_font, 0xFFFFFF00|i, 1, "HOW MANY SPRITES");
        GRRLIB_Printf((640-(16*20))/2, 216, tex_font, 0xFFFFFF00|i, 1, "CAN YOU DISPLAY WITH");
        GRRLIB_DrawImg((640-352)/2, 248, tex_logo, 0, 1, 1, 0xFFFFFF00|i);
        GRRLIB_Printf((640-(16*28))/2, 480-16, tex_font, 0xFFFFFF00|i, 1, "BY NONAMENO FROM GRRLIB TEAM");
        GRRLIB_Render();
    }

    while(1) {
        WPAD_ScanPads();

        GRRLIB_DrawImg(0, 0, tex_screen[screen_index], 0, 1, 1, 0xFFFFFFFF);
        GRRLIB_DrawImg(((R + r-ff)*cos(t-f) - d*cos(((R + r-f)/r)*t))+rmode->fbWidth/2-32, ((R + r-ff)*sin(t) - d*sin(((R + r)/r)*t)-f)+rmode->efbHeight/2-32, tex_ball, 1, 1, 1, 0xFFFFFFFF);
        GRRLIB_Screen2Texture(0, 0, tex_screen[screen_index], GX_FALSE);
        GRRLIB_Printf((640-(16*6*5))/2+5, 200+5, tex_font, 0x00000088, 5, "%06d",(int)spr);
        GRRLIB_Printf((640-(16*6*5))/2, 200, tex_font, 0xFFEEEE88, 5, "%06d",(int)spr);

        GRRLIB_Render();
        screen_index++;
        screen_index %= 10;		
        spr+=0.1f;	
        t+=0.01f;

        if(t>n*2*PI){
            n++;
            f+=0.01f;
        }

        if(f>2*PI){
            f=0;
            ff+=0.02f;
        }


        if(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);
        
    }
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB
    GRRLIB_FreeTexture(tex_logo);
    GRRLIB_FreeTexture(tex_ball);   
    GRRLIB_FreeTexture(tex_font);
    for(i=0; i<10; i++) {
        GRRLIB_FreeTexture(tex_screen[i]);
    }
    return 0;
}
