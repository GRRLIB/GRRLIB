/*===========================================
        GRRLIB (GX version) 3.0.5 alpha
        Code     : NoNameNo
        Additional Code : Crayon
        GX hints : RedShade

        How To use Bitmap FX example code
============================================*/
#include "../../../GRRLIB/GRRLIB/GRRLIB.h"

#include <stdlib.h>
#include <wiiuse/wpad.h>

#include "gfx/pirate.h"
#include "gfx/font1.h"


Mtx GXmodelView2D;


int main() {
    u32 wpaddown;
    int page = 0;

    // Font texture
    GRRLIB_texImg text_font1 = GRRLIB_LoadTexturePNG(font1);
    GRRLIB_InitTileSet(&text_font1, 32, 36, 32);

    // Load the original texture and create empty texture of the same size as the original one
    GRRLIB_texImg tex_pirate = GRRLIB_LoadTexturePNG(pirate);
    GRRLIB_texImg tex_gray = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_invert = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_blur1 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_blur2 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_blur3 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_blur4 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_blur5 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_blur6 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_pixel1 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_pixel2 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_pixel3 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_pixel4 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_pixel5 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_pixel6 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_scatter1 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_scatter2 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_scatter3 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_scatter4 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_scatter5 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);
    GRRLIB_texImg tex_scatter6 = GRRLIB_CreateEmptyTexture(tex_pirate.w, tex_pirate.h);

    // Let's precalculte the grayscale texture
    GRRLIB_BMFX_Grayscale(tex_pirate, tex_gray);
    GRRLIB_FlushTex(tex_gray);

    // Let's precalculte the inverted color texture
    GRRLIB_BMFX_Invert(tex_pirate, tex_invert);
    GRRLIB_FlushTex(tex_invert);

    // Let's precalculte 6 differents blur texture with 6 differents blur factor
    GRRLIB_BMFX_Blur(tex_pirate, tex_blur1, 1);
    GRRLIB_FlushTex(tex_blur1);
    GRRLIB_BMFX_Blur(tex_pirate, tex_blur2, 2);
    GRRLIB_FlushTex(tex_blur2);
    GRRLIB_BMFX_Blur(tex_pirate, tex_blur3, 3);
    GRRLIB_FlushTex(tex_blur3);
    GRRLIB_BMFX_Blur(tex_pirate, tex_blur4, 4);
    GRRLIB_FlushTex(tex_blur4);
    GRRLIB_BMFX_Blur(tex_pirate, tex_blur5, 5);
    GRRLIB_FlushTex(tex_blur5);
    GRRLIB_BMFX_Blur(tex_pirate, tex_blur6, 6);
    GRRLIB_FlushTex(tex_blur6);

    // Let's precalculte 6 differents pixelate texture with 6 differents pixelate factor
    GRRLIB_BMFX_Pixelate(tex_pirate, tex_pixel1, 1);
    GRRLIB_FlushTex(tex_pixel1);
    GRRLIB_BMFX_Pixelate(tex_pirate, tex_pixel2, 2);
    GRRLIB_FlushTex(tex_pixel2);
    GRRLIB_BMFX_Pixelate(tex_pirate, tex_pixel3, 3);
    GRRLIB_FlushTex(tex_pixel3);
    GRRLIB_BMFX_Pixelate(tex_pirate, tex_pixel4, 4);
    GRRLIB_FlushTex(tex_pixel4);
    GRRLIB_BMFX_Pixelate(tex_pirate, tex_pixel5, 5);
    GRRLIB_FlushTex(tex_pixel5);
    GRRLIB_BMFX_Pixelate(tex_pirate, tex_pixel6, 6);
    GRRLIB_FlushTex(tex_pixel6);

    // Let's precalculte 6 differents pixelate texture with 6 differents pixelate factor
    GRRLIB_BMFX_Scatter(tex_pirate, tex_scatter1, 2);
    GRRLIB_FlushTex(tex_pixel1);
    GRRLIB_BMFX_Scatter(tex_pirate, tex_scatter2, 4);
    GRRLIB_FlushTex(tex_pixel2);
    GRRLIB_BMFX_Scatter(tex_pirate, tex_scatter3, 6);
    GRRLIB_FlushTex(tex_pixel3);
    GRRLIB_BMFX_Scatter(tex_pirate, tex_scatter4, 8);
    GRRLIB_FlushTex(tex_pixel4);
    GRRLIB_BMFX_Scatter(tex_pirate, tex_scatter5, 10);
    GRRLIB_FlushTex(tex_pixel5);
    GRRLIB_BMFX_Scatter(tex_pirate, tex_scatter6, 12);
    GRRLIB_FlushTex(tex_pixel6);

    GRRLIB_Init();
    WPAD_Init();

    while(1) {
        WPAD_ScanPads();
        wpaddown = WPAD_ButtonsDown(0);

        GRRLIB_FillScreen(0xFFFFFFFF);

        switch(page)
        {
            case 1:
                GRRLIB_Printf(10, 10, text_font1, 0X000000FF, 1, "GRAYSCALE FX");

                GRRLIB_DrawImg(10, 60, tex_pirate, 0, 1, 1, 0xFFFFFFFF);
                GRRLIB_DrawImg(10+tex_pirate.w*1, 60, tex_gray, 0, 1, 1, 0xFFFFFFFF);
                break;
            case 2:
                GRRLIB_Printf(10, 10, text_font1, 0X000000FF, 1, "INVERT FX");

                GRRLIB_DrawImg(10, 60, tex_pirate, 0, 1, 1, 0xFFFFFFFF);
                GRRLIB_DrawImg(10+tex_pirate.w*1, 60, tex_invert, 0, 1, 1, 0xFFFFFFFF);
                break;
            case 3:
                GRRLIB_Printf(10, 10, text_font1, 0X000000FF, 1, "BLUR FX");

                GRRLIB_DrawImg(10, 60, tex_pirate, 0, 1, 1, 0xFFFFFFFF);
                GRRLIB_DrawImg(10+tex_pirate.w*1, 60, tex_blur1, 0, 1, 1, 0xFFFFFFFF);
                GRRLIB_DrawImg(10+tex_pirate.w*2, 60, tex_blur2, 0, 1, 1, 0xFFFFFFFF);
                GRRLIB_DrawImg(10+tex_pirate.w*3, 60, tex_blur3, 0, 1, 1, 0xFFFFFFFF);
                GRRLIB_DrawImg(10, 60+tex_pirate.h*1, tex_blur4, 0, 1, 1, 0xFFFFFFFF);
                GRRLIB_DrawImg(10+tex_pirate.w*1, 60+tex_pirate.h*1, tex_blur5, 0, 1, 1, 0xFFFFFFFF);
                GRRLIB_DrawImg(10+tex_pirate.w*2, 60+tex_pirate.h*1, tex_blur6, 0, 1, 1, 0xFFFFFFFF);
                break;
            case 4:
                GRRLIB_Printf(10, 10, text_font1, 0X000000FF, 1, "PIXELATE FX");

                GRRLIB_DrawImg(10, 60, tex_pirate, 0, 1, 1, 0xFFFFFFFF);
                GRRLIB_DrawImg(10+tex_pirate.w*1, 60, tex_pixel1, 0, 1, 1, 0xFFFFFFFF);
                GRRLIB_DrawImg(10+tex_pirate.w*2, 60, tex_pixel2, 0, 1, 1, 0xFFFFFFFF);
                GRRLIB_DrawImg(10+tex_pirate.w*3, 60, tex_pixel3, 0, 1, 1, 0xFFFFFFFF);
                GRRLIB_DrawImg(10, 60+tex_pirate.h*1, tex_pixel4, 0, 1, 1, 0xFFFFFFFF);
                GRRLIB_DrawImg(10+tex_pirate.w*1, 60+tex_pirate.h*1, tex_pixel5, 0, 1, 1, 0xFFFFFFFF);
                GRRLIB_DrawImg(10+tex_pirate.w*2, 60+tex_pirate.h*1, tex_pixel6, 0, 1, 1, 0xFFFFFFFF);
                break;
            case 5:
                GRRLIB_Printf(10, 10, text_font1, 0X000000FF, 1, "SCATTER FX");

                GRRLIB_DrawImg(10, 60, tex_pirate, 0, 1, 1, 0xFFFFFFFF);
                GRRLIB_DrawImg(10+tex_pirate.w*1, 60, tex_scatter1, 0, 1, 1, 0xFFFFFFFF);
                GRRLIB_DrawImg(10+tex_pirate.w*2, 60, tex_scatter2, 0, 1, 1, 0xFFFFFFFF);
                GRRLIB_DrawImg(10+tex_pirate.w*3, 60, tex_scatter3, 0, 1, 1, 0xFFFFFFFF);
                GRRLIB_DrawImg(10, 60+tex_pirate.h*1, tex_scatter4, 0, 1, 1, 0xFFFFFFFF);
                GRRLIB_DrawImg(10+tex_pirate.w*1, 60+tex_pirate.h*1, tex_scatter5, 0, 1, 1, 0xFFFFFFFF);
                GRRLIB_DrawImg(10+tex_pirate.w*2, 60+tex_pirate.h*1, tex_scatter6, 0, 1, 1, 0xFFFFFFFF);
                break;
            default:
                GRRLIB_Printf(10, 10, text_font1, 0X000000FF, 1, "WELCOME TO THE");
                GRRLIB_Printf(10, 40, text_font1, 0X000000FF, 1, "GRRLIB FX DEMO.");
                GRRLIB_Printf(10, 80, text_font1, 0X000000FF, 1, "TO QUIT PRESS THE");
                GRRLIB_Printf(10, 120, text_font1, 0X000000FF, 1, "HOME BUTTON.");
                GRRLIB_Printf(10, 160, text_font1, 0X000000FF, 1, "USE + OR - TO MOVE");
                GRRLIB_Printf(10, 200, text_font1, 0X000000FF, 1, "ACROSS PAGES.");
        }

        GRRLIB_Render();
        if(wpaddown & WPAD_BUTTON_HOME) {
            exit(0);
        }
        if(wpaddown & WPAD_BUTTON_MINUS) {
            page--;
            if(page < 0) page = 5;
        }
        if(wpaddown & WPAD_BUTTON_PLUS) {
            page++;
            if(page > 5) page = 0;
        }
    }
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB
    free(tex_pirate.data);
    free(tex_gray.data);
    free(tex_invert.data);
    free(tex_blur1.data);
    free(tex_blur2.data);
    free(tex_blur3.data);
    free(tex_blur4.data);
    free(tex_blur5.data);
    free(tex_blur6.data);
    free(tex_pixel1.data);
    free(tex_pixel2.data);
    free(tex_pixel3.data);
    free(tex_pixel4.data);
    free(tex_pixel5.data);
    free(tex_pixel6.data);
    free(tex_scatter1.data);
    free(tex_scatter2.data);
    free(tex_scatter3.data);
    free(tex_scatter4.data);
    free(tex_scatter5.data);
    free(tex_scatter6.data);
    return 0;
}
