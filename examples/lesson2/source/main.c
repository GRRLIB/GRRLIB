/*===========================================
        GRRLIB (GX version) 3.0.5 alpha
        Code     : NoNameNo
        Additional Code : Crayon
        GX hints : RedShade

        How To use GRRLIB_BMFX_Blur example code
============================================*/
#include "../../../GRRLIB/GRRLIB/GRRLIB.h"

#include <stdlib.h>
#include <wiiuse/wpad.h>

#include "gfx/pirate.h"


Mtx GXmodelView2D;


int main() {
    u32 wpaddown;

// Load the original texture and create 6 empty texture of the same size as the original one
    GRRLIB_texImg tex_pirate = GRRLIB_LoadTexturePNG(pirate);
    GRRLIB_texImg tex_new1 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_new2 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_new3 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_new4 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_new5 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_new6 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);

// Let's precalculte 6 differents blur texture with 6 differents blur factor
    GRRLIB_BMFX_Blur(tex_pirate, tex_new1,1);
    GRRLIB_FlushTex(tex_new1);

    GRRLIB_BMFX_Blur(tex_pirate, tex_new2,2);
    GRRLIB_FlushTex(tex_new2);

    GRRLIB_BMFX_Blur(tex_pirate, tex_new3,3);
    GRRLIB_FlushTex(tex_new3);

    GRRLIB_BMFX_Blur(tex_pirate, tex_new4,4);
    GRRLIB_FlushTex(tex_new4);

    GRRLIB_BMFX_Blur(tex_pirate, tex_new5,5);
    GRRLIB_FlushTex(tex_new5);

    GRRLIB_BMFX_Blur(tex_pirate, tex_new6,6);
    GRRLIB_FlushTex(tex_new6);


    GRRLIB_Init();
    WPAD_Init();

    while(1) {
        WPAD_ScanPads();
        wpaddown = WPAD_ButtonsDown(0);

        GRRLIB_FillScreen(0xFFFFFFFF);

        GRRLIB_DrawImg(10, 50, tex_pirate, 0, 1, 1, 0xFFFFFFFF);
        GRRLIB_DrawImg(10+tex_pirate.w*1, 50, tex_new1, 0, 1, 1, 0xFFFFFFFF);
        GRRLIB_DrawImg(10+tex_pirate.w*2, 50, tex_new2, 0, 1, 1, 0xFFFFFFFF);
        GRRLIB_DrawImg(10+tex_pirate.w*3, 50, tex_new3, 0, 1, 1, 0xFFFFFFFF);
        GRRLIB_DrawImg(10, 50+tex_pirate.h*1, tex_new4, 0, 1, 1, 0xFFFFFFFF);
        GRRLIB_DrawImg(10+tex_pirate.w*1, 50+tex_pirate.h*1, tex_new5, 0, 1, 1, 0xFFFFFFFF);
        GRRLIB_DrawImg(10+tex_pirate.w*2, 50+tex_pirate.h*1, tex_new6, 0, 1, 1, 0xFFFFFFFF);

        GRRLIB_Render();
        if(wpaddown & WPAD_BUTTON_HOME) {
            exit(0);
        }
    }
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB
    free(tex_pirate.data);
    free(tex_new1.data);
    free(tex_new2.data);
    free(tex_new3.data);
    free(tex_new4.data);
    free(tex_new5.data);
    free(tex_new6.data);
    return 0;
}
