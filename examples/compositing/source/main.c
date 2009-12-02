/*===========================================
    NoNameNo Compositing Sample Code
    Bugged since Screen2texture conversion
    loose color precision.
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <wiiuse/wpad.h>

#include "gfx/font3d.h"
extern GXRModeObj *rmode;



int main() {
    float rot=0;
    float i=0;
    int circsize=150;

    GRRLIB_Init();
    WPAD_Init();


    GRRLIB_texImg *tex_font = GRRLIB_LoadTexture(font3d);
    GRRLIB_InitTileSet(tex_font, 64, 64, 32);
    GRRLIB_SetHandle (tex_font, tex_font->tilew/2, tex_font->tileh+circsize);

    GRRLIB_texImg *tex_screen;
    tex_screen = GRRLIB_CreateEmptyTexture(rmode->fbWidth,rmode->efbHeight);

    GRRLIB_Settings.antialias = true;

    while(1) {
        WPAD_ScanPads();
        if(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);

        GRRLIB_CompoStart();
            for(i=0;i<360;i+=30) {
                GRRLIB_DrawTile((rmode->fbWidth/2)-(tex_font->tilew/2), (rmode->efbHeight/2)-(tex_font->tileh+circsize), tex_font, rot+i, 1, 1, 0xFFFFFFFF, 65-32+((int)i/45));
            }
        GRRLIB_CompoEnd(0, 0, tex_screen);

        rot-=0.6;

        GRRLIB_DrawImg(50, 50, tex_screen, 0, 1, 1, 0xFFFFFFFF);
        GRRLIB_DrawImg(100, 100, tex_screen, 0, 1, 1, 0xFFFFFFFF);

        GRRLIB_Render();
    }

    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB
    GRRLIB_FreeTexture(tex_screen);
    GRRLIB_FreeTexture(tex_font);
    exit(0);
}

