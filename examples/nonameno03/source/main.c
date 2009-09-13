/*===========================================
    NoNameNo Demo Button ;)

    Perhaps needs some improvement...
============================================*/
#include <grrlib.h>
#include "../GRRLIB_addon/GRRLIB_addon.h"

#include "gfx/pointer.h"

#include <stdlib.h>
#include <wiiuse/wpad.h>

extern GRRLIB_texImg *tex_GRRLIBfont;

int main() {
    u32 WPADDown;
    u32 WPADHeld;
    ir_t ir1;
    int wpadx=0, wpady=0;

    int resdown=0,resheld=0;


    GRRLIB_Init();
    GRRLIB_addon_Init();
    WPAD_Init();
    WPAD_SetIdleTimeout(60);
    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);

    GRRLIB_texImg *tex_pointer = GRRLIB_LoadTexture(pointer);

    while(1) {
        WPAD_SetVRes(0, 640, 480);
        WPAD_ScanPads();
        WPADDown = WPAD_ButtonsDown(0);
        WPADHeld = WPAD_ButtonsHeld(0);
        WPAD_IR(WPAD_CHAN_0, &ir1);
        wpadx=ir1.sx-200;
        wpady=ir1.sy-250;

        GRRLIB_addon_Button(1, 100,100,0xFFFF00FF, wpadx, wpady, WPADDown, WPADHeld, WPAD_BUTTON_A, &resdown, &resheld, " My First Button ");
        GRRLIB_addon_Button(2, 100,140,0xFF00FFFF, wpadx, wpady, WPADDown, WPADHeld, WPAD_BUTTON_A, &resdown, &resheld, " My Second Button ");
        GRRLIB_addon_Button(3, 100,180,0x00FFFFFF, wpadx, wpady, WPADDown, WPADHeld, WPAD_BUTTON_A, &resdown, &resheld, " My Third Button ");
        GRRLIB_addon_Button(4, 100,260,0xCCCCCCFF, wpadx, wpady, WPADDown, WPADHeld, WPAD_BUTTON_A, &resdown, &resheld, " -- QuIt -- ");

        GRRLIB_Printf(100, 310, tex_GRRLIBfont, 0xFFFFFFFF, 1, "button down : %d",resdown);
        GRRLIB_Printf(100, 330, tex_GRRLIBfont, 0xFFFFFFFF, 1, "button held : %d",resheld);

        if(resdown==4){
            exit(0);
        }

        resdown=0,resheld=0;

        GRRLIB_DrawImg(wpadx-32, wpady-32, tex_pointer , 0, 1, 1, 0xFFFFFFFF );

        GRRLIB_Render();
    }
    GRRLIB_FreeTexture(tex_pointer);
    GRRLIB_addon_Exit();
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB
    return 0;
}


