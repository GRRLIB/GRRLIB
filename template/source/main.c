/*===========================================
        GRRLIB (GX version) 3.0.5 alpha
        Code     : NoNameNo
        Additional Code : Crayon
        GX hints : RedShade

        Template Code (Minimum Requirement)
============================================*/
#include "GRRLIB/GRRLIB.h"

#include <stdlib.h>

#include <wiiuse/wpad.h>
#include <fat.h>

#include "gfx/BMfont1.h"
#include "gfx/BMfont2.h"
#include "gfx/BMfont3.h"
#include "gfx/BMfont4.h"
#include "gfx/test_jpg.h"
#include "gfx/link.h"

// Tile stuff
#define TILE_DELAY  10
#define TILE_UP     12*0
#define TILE_RIGHT  12*1
#define TILE_DOWN   12*2
#define TILE_LEFT   12*3
#define TILE_UP2    12*4+9
#define TILE_RIGHT2 12*5+9
#define TILE_DOWN2  12*6+9
#define TILE_LEFT2  12*7+9

// RGBA Colors
#define GRRLIB_BLACK  0x000000FF
#define GRRLIB_WHITE  0xFFFFFFFF
#define GRRLIB_RED    0xFF0000FF
#define GRRLIB_BLUE   0x0000FFFF
#define GRRLIB_GREEN  0x008000FF
#define GRRLIB_GRAY   0x808080FF
#define GRRLIB_YELLOW 0xFFFF00FF
#define GRRLIB_SILVER 0xC0C0C0FF

Mtx GXmodelView2D;

int main() {
    int left = 0, top = 0, page = 0, frame = TILE_DOWN + 1;
    unsigned int wait = TILE_DELAY, direction = TILE_DOWN, direction_new = TILE_DOWN;
    ir_t ir1;
    u32 wpaddown, wpadheld;

    GRRLIB_Init();

    fatInitDefault();
    WPAD_Init();
    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);

    GRRLIB_texImg tex_test_jpg = GRRLIB_LoadTextureJPG(test_jpg);

    GRRLIB_texImg tex_link_png = GRRLIB_LoadTexturePNG(link);
    GRRLIB_InitTileSet(&tex_link_png, 24, 32, 0);

    GRRLIB_texImg tex_BMfont1 = GRRLIB_LoadTexturePNG(BMfont1);
    GRRLIB_InitTileSet(&tex_BMfont1, 32, 32, 32);

    GRRLIB_texImg tex_BMfont2 = GRRLIB_LoadTexturePNG(BMfont2);
    GRRLIB_InitTileSet(&tex_BMfont2, 16, 16, 32);

    GRRLIB_texImg tex_BMfont3 = GRRLIB_LoadTexturePNG(BMfont3);
    GRRLIB_InitTileSet(&tex_BMfont3, 32, 32, 32);

    GRRLIB_texImg tex_BMfont4 = GRRLIB_LoadTexturePNG(BMfont4);
    GRRLIB_InitTileSet(&tex_BMfont4, 16, 16, 32);


    while(1) {
        WPAD_SetVRes(0, 640, 480);
        WPAD_ScanPads();
        wpaddown = WPAD_ButtonsDown(0);
        wpadheld = WPAD_ButtonsHeld(0);

        WPAD_IR(WPAD_CHAN_0, &ir1);

        GRRLIB_FillScreen(GRRLIB_BLACK);    // Clear the screen
        switch(page)
        {
            case 1:   // Draw images
                GRRLIB_Printf(5, 25, tex_BMfont2, GRRLIB_WHITE, 1, "IMAGES DEMO");

                GRRLIB_DrawImg(10, 50, tex_test_jpg, 0, 1, 1, GRRLIB_WHITE);
                // Draw a sprite
                GRRLIB_DrawTile(600, 400, tex_link_png, 0, 2, 2, GRRLIB_WHITE, 12*4); // Rupy
                GRRLIB_DrawTile(320+left, 240+top, tex_link_png, 0, 2, 2, GRRLIB_WHITE, frame);
                if(direction_new != direction) {
                    // Direction has changed, modify frame immidiately
                    direction = direction_new;
                    frame = direction;
                    wait = 0;
                }
                wait++;
                if(wait > TILE_DELAY) {
                    // wait is needed for the number of frame per second to be ok
                    wait = 0;
                    if(wpadheld & WPAD_BUTTON_LEFT || wpadheld & WPAD_BUTTON_RIGHT ||
                        wpadheld & WPAD_BUTTON_UP || wpadheld & WPAD_BUTTON_DOWN) {
                        frame++;
                    }
                    else {
                        frame = direction + 1;  // Not moving
                        wait = TILE_DELAY;      // Ready to move
                    }
                    if(frame > direction+2) frame = direction;
                }
                break;
            case 2:   // Draw shapes
                GRRLIB_Printf(5, 25, tex_BMfont2, GRRLIB_WHITE, 1, "SHAPES DEMO");

                GRRLIB_Rectangle(100, 100, 200, 100, GRRLIB_RED, 1);
                GRRLIB_Rectangle(left + 150, top + 150, 200, 200, 0x0000FFC8, 1); // Blue with alpha
                GRRLIB_Line(100, 100, 350, 200, GRRLIB_SILVER);

                // Draw a yellow four pixel dot where the wiimote is pointing
                GRRLIB_Plot(ir1.sx, ir1.sy, GRRLIB_YELLOW);
                GRRLIB_Plot(ir1.sx + 1, ir1.sy, GRRLIB_YELLOW);
                GRRLIB_Plot(ir1.sx, ir1.sy + 1, GRRLIB_YELLOW);
                GRRLIB_Plot(ir1.sx + 1, ir1.sy + 1, GRRLIB_YELLOW);
                break;
            default: // Print some text
                GRRLIB_Printf(5, 25, tex_BMfont2, GRRLIB_WHITE, 1, "TEXT DEMO");

                GRRLIB_Printf(5, 100, tex_BMfont4, GRRLIB_WHITE, 1, "TO QUIT PRESS THE HOME BUTTON.");
                GRRLIB_Printf(5, 140, tex_BMfont4, GRRLIB_YELLOW, 1, "USE + AND - TO MOVE ACROSS PAGES.");
                GRRLIB_Printf(5, 180, tex_BMfont4, GRRLIB_GREEN, 1, "USE THE D-PAD TO MOVE STUFF.");
                GRRLIB_Printf(left, top+250, tex_BMfont1, GRRLIB_WHITE, 1, "X VALUE: %d", (int)ir1.sx);
                GRRLIB_Printf(left, top+300, tex_BMfont3, GRRLIB_WHITE, 1, "Y VALUE: %d", (int)ir1.sy);
        }
        GRRLIB_Render();

        if(wpaddown & WPAD_BUTTON_HOME) {
            exit(0);
        }
        if(wpadheld & WPAD_BUTTON_LEFT) {
            if(wpadheld & WPAD_BUTTON_B || page == 1)
                left -= 2;
            else
                left--;
            direction_new = TILE_LEFT;  // for tile example
        }
        if(wpadheld & WPAD_BUTTON_RIGHT) {
            if(wpadheld & WPAD_BUTTON_B || page == 1)
                left += 2;
            else
                left++;
            direction_new = TILE_RIGHT;  // for tile example
        }
        if(wpadheld & WPAD_BUTTON_UP) {
            if(wpadheld & WPAD_BUTTON_B || page == 1)
                top -= 2;
            else
                top--;
            direction_new = TILE_UP;  // for tile example
        }
        if(wpadheld & WPAD_BUTTON_DOWN) {
            if(wpadheld & WPAD_BUTTON_B || page == 1)
                top += 2;
            else
                top++;
            direction_new = TILE_DOWN;  // for tile example
        }
        if(wpaddown & WPAD_BUTTON_MINUS) {
            page--;
            left = 0;
            top = 0;
            if(page < 0) page = 2;
        }
        if(wpaddown & WPAD_BUTTON_PLUS) {
            page++;
            left = 0;
            top = 0;
            if(page > 2) page = 0;
        }
    }
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB
    // Free some textures
    free(tex_test_jpg.data);
    free(tex_link_png.data);
    free(tex_BMfont1.data);
    free(tex_BMfont2.data);
    free(tex_BMfont3.data);
    free(tex_BMfont4.data);
    return 0;
}
