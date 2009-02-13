/*===========================================
        GRRLIB (GX version) 3.0.5 alpha
        Code     : NoNameNo
        Additional Code : Crayon
        GX hints : RedShade

        Minimum Code To Use GRRLIB
============================================*/
#include "../../GRRLIB/GRRLIB/GRRLIB.h"


#include <stdlib.h>
#include <wiiuse/wpad.h>


Mtx GXmodelView2D;


int main() {
    u32 wpaddown;

    GRRLIB_Init();
    WPAD_Init();

    while(1) {
        WPAD_ScanPads();
        wpaddown = WPAD_ButtonsDown(0);

        GRRLIB_Render();
        if(wpaddown & WPAD_BUTTON_HOME) {
            exit(0);
        }
    }
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB
    return 0;
}
