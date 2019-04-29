/*===========================================
        GRRLIB (GX Version)
        - Example Code -

        How To use Bitmap Fonts
============================================*/
#include <grrlib.h>

#include <ogc/lwp_watchdog.h>   // Needed for gettime and ticks_to_millisecs
#include <stdlib.h>
#include <wiiuse/wpad.h>
#include <fat.h>

#include "gfx/BMfont1.h"
#include "gfx/BMfont2.h"
#include "gfx/BMfont3.h"
#include "gfx/BMfont4.h"
#include "gfx/BMfont5.h"
#include "gfx/test_jpg.h"
#include "gfx/test_bmp.h"
#include "gfx/sprite.h"
#include "ocean_bmf.h"
#include "frontal_bmf.h"

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
#define GRRLIB_BLACK   0x000000FF
#define GRRLIB_MAROON  0x800000FF
#define GRRLIB_GREEN   0x008000FF
#define GRRLIB_OLIVE   0x808000FF
#define GRRLIB_NAVY    0x000080FF
#define GRRLIB_PURPLE  0x800080FF
#define GRRLIB_TEAL    0x008080FF
#define GRRLIB_GRAY    0x808080FF
#define GRRLIB_SILVER  0xC0C0C0FF
#define GRRLIB_RED     0xFF0000FF
#define GRRLIB_LIME    0x00FF00FF
#define GRRLIB_YELLOW  0xFFFF00FF
#define GRRLIB_BLUE    0x0000FFFF
#define GRRLIB_FUCHSIA 0xFF00FFFF
#define GRRLIB_AQUA    0x00FFFFFF
#define GRRLIB_WHITE   0xFFFFFFFF

static u8 CalculateFrameRate();

int main() {
    s32 left = 0, top = 0, page = 0, frame = TILE_DOWN + 1;
    u32 wait = TILE_DELAY, direction = TILE_DOWN, direction_new = TILE_DOWN;
    u8 FPS = 0;

    ir_t ir1;
    u32 wpaddown, wpadheld;
    guVector triangle[] = {{400,200,0.0f}, {500,400,0.0f}, {300,400,0.0f}};
    u32 trianglecolor[] = {GRRLIB_GREEN, GRRLIB_RED, GRRLIB_BLUE};

    GRRLIB_Init();

    WPAD_Init();
    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);

    GRRLIB_texImg *tex_test_jpg = GRRLIB_LoadTexture(test_jpg);
    GRRLIB_texImg *tex_test_bmp = GRRLIB_LoadTexture(test_bmp);

    GRRLIB_bytemapFont *bmf_Font1 = GRRLIB_LoadBMF(ocean_bmf);
    GRRLIB_bytemapFont *bmf_Font2 = GRRLIB_LoadBMF(frontal_bmf);

    GRRLIB_texImg *tex_sprite_png = GRRLIB_LoadTexture(sprite);
    GRRLIB_InitTileSet(tex_sprite_png, 24, 32, 0);

    GRRLIB_texImg *tex_BMfont1 = GRRLIB_LoadTexture(BMfont1);
    GRRLIB_InitTileSet(tex_BMfont1, 32, 32, 32);

    GRRLIB_texImg *tex_BMfont2 = GRRLIB_LoadTexture(BMfont2);
    GRRLIB_InitTileSet(tex_BMfont2, 16, 16, 32);

    GRRLIB_texImg *tex_BMfont3 = GRRLIB_LoadTexture(BMfont3);
    GRRLIB_InitTileSet(tex_BMfont3, 32, 32, 32);

    GRRLIB_texImg *tex_BMfont4 = GRRLIB_LoadTexture(BMfont4);
    GRRLIB_InitTileSet(tex_BMfont4, 16, 16, 32);

    GRRLIB_texImg *tex_BMfont5 = GRRLIB_LoadTexture(BMfont5);
    GRRLIB_InitTileSet(tex_BMfont5, 8, 16, 0);

    while(1) {
        WPAD_SetVRes(0, 640, 480);
        WPAD_ScanPads();
        wpaddown = WPAD_ButtonsDown(0);
        wpadheld = WPAD_ButtonsHeld(0);

        WPAD_IR(WPAD_CHAN_0, &ir1);

        GRRLIB_FillScreen(GRRLIB_BLACK);    // Clear the screen
        WPAD_Rumble(WPAD_CHAN_0, 0);
        switch(page)
        {
            case 1:   // Draw images
                GRRLIB_Printf(5, 25, tex_BMfont2, GRRLIB_WHITE, 1, "IMAGES DEMO");

                GRRLIB_DrawImg(10, 50, tex_test_jpg, 0, 1, 1, GRRLIB_WHITE);  // Draw a jpeg
                GRRLIB_DrawImg(350, 50, tex_test_bmp, 0, 4, 4, GRRLIB_WHITE); // Draw a bitmap

                // Draw a sprite
                GRRLIB_DrawTile(600, 400, tex_sprite_png, 0, 2, 2, GRRLIB_WHITE, 12*4); // Rupee
                GRRLIB_DrawTile(320+left, 240+top, tex_sprite_png, 0, 2, 2, GRRLIB_WHITE, frame);
                if(GRRLIB_RectOnRect(320+left, 240+top, 48, 64, 618, 434, 12, 30))
                {
                    WPAD_Rumble(WPAD_CHAN_0, 1);
                }
                if(direction_new != direction) {
                    // Direction has changed, modify frame immediately
                    direction = direction_new;
                    frame = direction;
                    wait = 0;
                }
                wait++;
                if(wait > TILE_DELAY) {
                    // wait is needed for the number of frame per second to be OK
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
                GRRLIB_Line(100, 100, 350, 200, GRRLIB_SILVER);
                GRRLIB_NGoneFilled(triangle, trianglecolor, 3);
                GRRLIB_Rectangle(left + 150, top + 150, 200, 200, 0x0000FFC8, 1); // Blue with alpha
                GRRLIB_Circle(left + 300, top + 300, 50, GRRLIB_OLIVE, 1);

                // Draw a yellow four pixel dot where the Wii Remote is pointing
                GRRLIB_Plot(ir1.sx, ir1.sy, GRRLIB_YELLOW);
                GRRLIB_Plot(ir1.sx + 1, ir1.sy, GRRLIB_YELLOW);
                GRRLIB_Plot(ir1.sx, ir1.sy + 1, GRRLIB_YELLOW);
                GRRLIB_Plot(ir1.sx + 1, ir1.sy + 1, GRRLIB_YELLOW);
                break;
            default: // Print some text
                GRRLIB_Printf(5, 25, tex_BMfont2, GRRLIB_WHITE, 1, "GRRLIB %s TEXT DEMO", GRRLIB_VER_STRING);

                GRRLIB_Printf(5, 100, tex_BMfont4, GRRLIB_WHITE, 1, "TO QUIT PRESS THE HOME BUTTON.");
                GRRLIB_Printf(5, 140, tex_BMfont4, GRRLIB_YELLOW, 1, "USE + OR - TO MOVE ACROSS PAGES.");
                GRRLIB_Printf(5, 180, tex_BMfont4, GRRLIB_GREEN, 1, "USE THE D-PAD TO MOVE STUFF.");
                GRRLIB_Printf(left, top+250, tex_BMfont1, GRRLIB_WHITE, 1, "IR X VALUE: %d", (int)ir1.x);
                GRRLIB_Printf(left, top+300, tex_BMfont3, GRRLIB_WHITE, 1, "IR Y VALUE: %d", (int)ir1.y);
                GRRLIB_Printf(left, top+350, tex_BMfont3, 0XFFFFFF50, 1, "TEXT WITH ALPHA");
                GRRLIB_Printf(left, top+400, tex_BMfont5, GRRLIB_LIME, 1, "This font has the 128 ASCII characters");
                GRRLIB_PrintBMF(left, top+420, bmf_Font2, "%s", bmf_Font2->name);
        }
        GRRLIB_Printf(500, 27, tex_BMfont5, GRRLIB_WHITE, 1, "Current FPS: %d", FPS);

        if(wpaddown & WPAD_BUTTON_HOME) {
            break;
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
        if(wpadheld & WPAD_BUTTON_1 && wpadheld & WPAD_BUTTON_2) {
            WPAD_Rumble(WPAD_CHAN_0, 1); // Rumble on
            GRRLIB_ScrShot("sd:/grrlib.png");
            WPAD_Rumble(WPAD_CHAN_0, 0); // Rumble off
        }

        GRRLIB_Render();
        FPS = CalculateFrameRate();
    }
    // Free some textures
    GRRLIB_FreeTexture(tex_test_jpg);
    GRRLIB_FreeTexture(tex_test_bmp);
    GRRLIB_FreeTexture(tex_sprite_png);
    GRRLIB_FreeTexture(tex_BMfont1);
    GRRLIB_FreeTexture(tex_BMfont2);
    GRRLIB_FreeTexture(tex_BMfont3);
    GRRLIB_FreeTexture(tex_BMfont4);
    GRRLIB_FreeTexture(tex_BMfont5);
    GRRLIB_FreeBMF(bmf_Font1);
    GRRLIB_FreeBMF(bmf_Font2);
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB
    return 0;
}

/**
 * This function calculates the number of frames we render each second.
 * @return The number of frames per second.
 */
static u8 CalculateFrameRate() {
    static u8 frameCount = 0;
    static u32 lastTime;
    static u8 FPS = 0;
    u32 currentTime = ticks_to_millisecs(gettime());

    frameCount++;
    if(currentTime - lastTime > 1000) {
        lastTime = currentTime;
        FPS = frameCount;
        frameCount = 0;
    }
    return FPS;
}

