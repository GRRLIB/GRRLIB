/*===========================================
    NoNameNo
    Simple Textured 3D cube and Compositing
    to make a nice sin wave on it ;)
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <wiiuse/wpad.h>


#include "font_png.h"
#include "girl_png.h"


int main() {
    float a=0;
    int cubeZ=5;
    float sinx=0;

    GRRLIB_Init();
    WPAD_Init();

    GRRLIB_texImg *tex_screen = GRRLIB_CreateEmptyTexture(rmode->fbWidth,rmode->efbHeight);
    GRRLIB_InitTileSet(tex_screen, rmode->fbWidth, 1, 0);

    GRRLIB_texImg *tex_girl= GRRLIB_LoadTexture(girl_png);

    GRRLIB_texImg *tex_font = GRRLIB_LoadTexture(font_png);
    GRRLIB_InitTileSet(tex_font, 16, 16, 32);


    GRRLIB_Settings.antialias = true;

    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);
    GRRLIB_Camera3dSettings(0.0f,0.0f,13.0f, 0,1,0, 0,0,0);

    while(1) {
        GRRLIB_2dMode();
        WPAD_ScanPads();
        if(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_A) cubeZ++;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_B) cubeZ--;

        GRRLIB_3dMode(0.1,1000,45,1,0);
        GRRLIB_SetTexture(tex_girl, FALSE);
        GRRLIB_ObjectView(0,0,cubeZ, a,a*2,a*3,1,1,1);
        GX_Begin(GX_QUADS, GX_VTXFMT0, 24);
            GX_Position3f32(-1.0f,1.0f,1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(0.0f,0.0f);
            GX_Position3f32(1.0f,1.0f,1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(1.0f,0.0f);
            GX_Position3f32(1.0f,-1.0f,1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(1.0f,1.0f);
            GX_Position3f32(-1.0f,-1.0f,1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(0.0f,1.0f);

            GX_Position3f32(1.0f,1.0f,-1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(0.0f,0.0f);
            GX_Position3f32(-1.0f,1.0f,-1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(1.0f,0.0f);
            GX_Position3f32(-1.0f,-1.0f,-1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(1.0f,1.0f);
            GX_Position3f32(1.0f,-1.0f,-1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(0.0f,1.0f);

            GX_Position3f32(1.0f,1.0f,1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(0.0f,0.0f);
            GX_Position3f32(1.0f,1.0f,-1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(1.0f,0.0f);
            GX_Position3f32(1.0f,-1.0f,-1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(1.0f,1.0f);
            GX_Position3f32(1.0f,-1.0f,1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(0.0f,1.0f);

            GX_Position3f32(-1.0f,1.0f,-1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(0.0f,0.0f);
            GX_Position3f32(-1.0f,1.0f,1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(1.0f,0.0f);
            GX_Position3f32(-1.0f,-1.0f,1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(1.0f,1.0f);
            GX_Position3f32(-1.0f,-1.0f,-1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(0.0f,1.0f);

            GX_Position3f32(-1.0f,1.0f,-1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(0.0f,0.0f);
            GX_Position3f32(1.0f,1.0f,-1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(1.0f,0.0f);
            GX_Position3f32(1.0f,1.0f,1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(1.0f,1.0f);
            GX_Position3f32(-1.0f,1.0f,1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(0.0f,1.0f);

            GX_Position3f32(1.0f,-1.0f,-1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(0.0f,0.0f);
            GX_Position3f32(-1.0f,-1.0f,-1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(1.0f,0.0f);
            GX_Position3f32(-1.0f,-1.0f,1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(1.0f,1.0f);
            GX_Position3f32(1.0f,-1.0f,1.0f);
            GX_Color1u32(0xFFFFFFFF);
            GX_TexCoord2f32(0.0f,1.0f);
        GX_End();
        GRRLIB_Screen2Texture(0,0,tex_screen,1);
        a+=0.5f;

        // Switch To 2D Mode to display text
        GRRLIB_2dMode();
        const float oldsinx=sinx;
        for(int i=0; i<rmode->efbHeight; i++) {
            GRRLIB_DrawTile(0+sin(sinx)*60,i,tex_screen,0,1,1,0xFFFFFFFF,i);
            sinx+=0.02f;
        }
        sinx=oldsinx+0.02f;

        GRRLIB_Printf((640-(16*29))/2, 20, tex_font, 0xFFFFFFFF, 1, "PRESS A OR B TO ZOOM THE CUBE");

        GRRLIB_Render();
    }
    GRRLIB_FreeTexture(tex_girl);
    GRRLIB_FreeTexture(tex_font);
    GRRLIB_FreeTexture(tex_screen);
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB
    exit(0);
}
