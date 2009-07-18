/*===========================================
        GRRLIB (GX Version)
        - Template Code -

        Minimum Code To Use GRRLIB
============================================*/
#include "../../../GRRLIB/GRRLIB/GRRLIB.h"


#include <stdlib.h>
#include <wiiuse/wpad.h>



int main() {
    u32 WPADDown;

    GRRLIB_Init();
    WPAD_Init();

    while(1) {
        WPAD_ScanPads();
        WPADDown = WPAD_ButtonsDown(0);

        GRRLIB_Render();
        if(WPADDown & WPAD_BUTTON_HOME) {
            break;
        }
    }
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB
    return 0;
}
