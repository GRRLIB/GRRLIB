/*===========================================
        GRRLIB (GX version) 4.0.0 addon
        Code     : NoNameNo
        Additional Code : Crayon & Xane
        GX hints : RedShade
===========================================*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdarg.h>
#include <string.h>
#include "../lib/libpng/pngu/pngu.h"
#include "GRRLIB.h"
#include <fat.h>

#include "GRRLIB_addon/GRRLIBfont.h"
#include "GRRLIB_addon/GRRLIBbutton.h"

extern u32 fb;
extern void *xfb[2];
extern GXRModeObj *rmode;

GRRLIB_texImg *tex_GRRLIBfont;
GRRLIB_texImg *tex_GRRLIBbutton;

/**
 * Initalize all addon requirement
 */
void GRRLIB_addon_Init(){
    tex_GRRLIBfont = GRRLIB_LoadTexture(GRRLIBfont);
    GRRLIB_InitTileSet(tex_GRRLIBfont, 16, 19, 32);

    tex_GRRLIBbutton = GRRLIB_LoadTexture(GRRLIBbutton);
    GRRLIB_InitTileSet(tex_GRRLIBbutton, 4, 24, 0);
}

/**
 * Free all addon requirement
 */
void GRRLIB_addon_Exit(){
    GRRLIB_FreeTexture(tex_GRRLIBfont);
    GRRLIB_FreeTexture(tex_GRRLIBbutton);
}

/**
 * Load a texture from a file.
 * @param filename The JPEG or PNG filename to load.
 * @return A GRRLIB_texImg structure filled with image informations.
 */
GRRLIB_texImg *GRRLIB_LoadTextureFromFile(const char *filename) {
    fatInitDefault();
    FILE *fd = fopen(filename, "rb");

    fseek(fd, 0, SEEK_END);
    long lsize = ftell(fd);
    rewind(fd);

    unsigned char *buffer = (unsigned char*) malloc (sizeof(unsigned char)*lsize);
    fread (buffer, 1, lsize, fd);
    GRRLIB_texImg *tex = GRRLIB_LoadTexture(buffer);
    free(buffer);

    fclose(fd);
    return tex;
}

/**
 * Make a PNG screenshot on the SD card.
 * libfat is required to use the function.
 * @param File name of the file to write.
 * @return true if every thing worked, false otherwise.
 */
bool GRRLIB_ScrShot(const char* File) {
    int ErrorCode = -1;
    IMGCTX pngContext;

    if(fatInitDefault() && (pngContext = PNGU_SelectImageFromDevice(File))) {
        ErrorCode = PNGU_EncodeFromYCbYCr(pngContext, rmode->fbWidth, rmode->efbHeight, xfb[fb], 0);
        PNGU_ReleaseImageContext(pngContext);
    }
    return !ErrorCode;
}

/**
 * Easy Button Maker.
 * @param indice Index number of your button.
 * @param x top-left corner X position of the button.
 * @param y top-left corner Y position of the button.
 * @param col color of your button.
 * @param wpadx your X wpad posistion.
 * @param wpady your Y wpad posistion.
 * @param WPADDown your wpad button Down Status.
 * @param WPADHeld your wpad button Held Status.
 * @param but The wpad button you want to check.
 * @param resdown You will find here the downed button index number.
 * @param resheld You will find here the helded button index number.
 * @param toto Text on the button.
 */
void GRRLIB_addon_Button(int indice, int x,int y,u32 col, int wpadx, int wpady, u32 WPADDown, u32 WPADHeld, int but, int *resdown, int *resheld, char toto[]){
    int butwidth=strlen(toto)*16+8;
    Vector bg[]={{x+4,y,0},{x+4+strlen(toto)*16,y,0},{x+4+strlen(toto)*16,y+24,0},{x+4,y+24,0}};
    if((toto[0]=='^') && ((toto[1]=='U') || (toto[1]=='D') || (toto[1]=='L') || (toto[1]=='R'))){
        butwidth=1*16+8;
        bg[1].x=x+4+1*16;
        bg[2].x=x+4+1*16;
    }

    GRRLIB_DrawTile(x,y, tex_GRRLIBbutton , 0, 1, 1, col,0 );
    GRRLIB_DrawTileQuad(bg, tex_GRRLIBbutton, col,1 );
    GRRLIB_DrawTile(bg[1].x,y, tex_GRRLIBbutton , 0, 1, 1, col,2);

    if(GRRLIB_PtInRect(x, y, butwidth, 24, wpadx, wpady)) {
        if((toto[0]=='^') && (toto[1]=='U')) GRRLIB_Printf(x+4, y+2, tex_GRRLIBfont, 0xFFFFFFFF, 1, "%c", 0xa1);
        else if((toto[0]=='^') && (toto[1]=='D')) GRRLIB_Printf(x+4, y+2, tex_GRRLIBfont, 0xFFFFFFFF, 1, "%c", 0xa2);
        else if((toto[0]=='^') && (toto[1]=='L')) GRRLIB_Printf(x+4, y+2, tex_GRRLIBfont, 0xFFFFFFFF, 1, "%c", 0xa3);
        else if((toto[0]=='^') && (toto[1]=='R')) GRRLIB_Printf(x+4, y+2, tex_GRRLIBfont, 0xFFFFFFFF, 1, "%c", 0xa4);
        else GRRLIB_Printf(x+4, y+2, tex_GRRLIBfont, 0xFFFFFFFF, 1, toto);
        if(WPADDown & but) {
            *resdown=indice;
        }
        if(WPADHeld & but) {
            *resheld=indice;
        }
    }
    else{
        if((toto[0]=='^') && (toto[1]=='U')) GRRLIB_Printf(x+4, y+2, tex_GRRLIBfont, 0xFFFFFF77, 1, "%c", 0xa1);
        else if((toto[0]=='^') && (toto[1]=='D')) GRRLIB_Printf(x+4, y+2, tex_GRRLIBfont, 0xFFFFFF77, 1, "%c", 0xa2);
        else if((toto[0]=='^') && (toto[1]=='L')) GRRLIB_Printf(x+4, y+2, tex_GRRLIBfont, 0xFFFFFF77, 1, "%c", 0xa3);
        else if((toto[0]=='^') && (toto[1]=='R')) GRRLIB_Printf(x+4, y+2, tex_GRRLIBfont, 0xFFFFFF77, 1, "%c", 0xa4);
        else GRRLIB_Printf(x+4, y+2, tex_GRRLIBfont, 0xFFFFFF77, 1, toto);
    }
}
