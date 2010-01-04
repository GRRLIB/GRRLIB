/*===========================================
        TrueType Font demo
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <wiiuse/wpad.h>

// Font
#include "FreeMonoBold_ttf.h"

extern GXRModeObj *rmode;

int main(int argc, char **argv) {
    // Initialise the Graphics & Video subsystem
    GRRLIB_Init();
    GRRLIB_ttfFont *myFont = GRRLIB_LoadTTF(FreeMonoBold_ttf, FreeMonoBold_ttf_size);

    GRRLIB_texImg *CopiedImg = GRRLIB_CreateEmptyTexture(rmode->fbWidth, rmode->efbHeight);

    // Initialise the Wii Remotes
    WPAD_Init();

    // Loop forever
    while(1) {
        GRRLIB_DrawImg(0, 0, CopiedImg, 0, 1, 1, 0xFFFFFFFF);
        char Letter[2] = {rand() % 93 + 32, '\0'};
        GRRLIB_PrintfTTF(rand() % rmode->fbWidth - 50,
                         rand() % rmode->efbHeight - 50,
                         myFont,
                         Letter,
                         rand() % 180 + 20,
                         rand() % 0xFFFFFF);
        GRRLIB_Screen2Texture(0, 0, CopiedImg, false);

        GRRLIB_Render();  // Render the frame buffer to the TV

        WPAD_ScanPads();  // Scan the Wii Remotes

        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME)
            break;
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_A) {
            GRRLIB_Rectangle(0, 0, rmode->fbWidth, rmode->efbHeight, 0x000000FF, 1);
            GRRLIB_Screen2Texture(0, 0, CopiedImg, false);
        }
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_1 && WPAD_ButtonsHeld(0) & WPAD_BUTTON_2) {
            WPAD_Rumble(0, true);  // Rumble on
            GRRLIB_ScrShot("sd:/grrlib_ttf.png");
            WPAD_Rumble(0, false); // Rumble off
        }
    }

    GRRLIB_FreeTTF(myFont);
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB

    exit(0);  // Use exit() to exit a program, do not use 'return' from main()
}
