#include <grrlib.h>
#include <wiiuse/wpad.h>
#include <stdlib.h>

int main(void)
{
    // Initializes GRRLIB and WiiPad
    GRRLIB_Init();
    WPAD_Init();

    GRRLIB_SetBackgroundColour(255, 0, 0, 1); // Sets the background color to red

    // Main loop with the whole of content
    while (1) {
        GRRLIB_Render(); // Renders our content
    }

    GRRLIB_Exit(); // Finish GRRLIB work
    exit(0); // Exit from executable
}
