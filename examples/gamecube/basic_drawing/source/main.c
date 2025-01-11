/*===========================================
        GRRLIB (GX Version)
        - Example Code -

        How To use Bitmap Fonts
============================================*/
#include <grrlib.h>

#include <ogc/lwp_watchdog.h>   // Needed for gettime and ticks_to_millisecs
#include <stdlib.h>

#include "BMfont1_png.h"
#include "BMfont2_png.h"
#include "BMfont3_png.h"
#include "BMfont4_png.h"
#include "BMfont5_png.h"
#include "test_jpg_jpg.h"
#include "test_bmp_bmp.h"
#include "sprite_png.h"
#include "ocean_bmf.h"
#include "frontal_bmf.h"

// Tile stuff
#define TILE_DELAY  10
#define TILE_UP     12 * 0
#define TILE_RIGHT  12 * 1
#define TILE_DOWN   12 * 2
#define TILE_LEFT   12 * 3
#define TILE_UP2    12 * 4 + 9
#define TILE_RIGHT2 12 * 5 + 9
#define TILE_DOWN2  12 * 6 + 9
#define TILE_LEFT2  12 * 7 + 9

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

static u8 CalculateFrameRate(void);

int main() {
    s32 left = 0, top = 0, page = 0, frame = TILE_DOWN + 1;
    u32 wait = TILE_DELAY, direction = TILE_DOWN, direction_new = TILE_DOWN;
    u8 FPS = 0;

    guVector triangle[] = {{400,200,0.0f}, {500,400,0.0f}, {300,400,0.0f}};
    u32 trianglecolor[] = {GRRLIB_GREEN, GRRLIB_RED, GRRLIB_BLUE};

    GRRLIB_Init();

    PAD_Init();

    GRRLIB_texImg *tex_test_jpg = GRRLIB_LoadTexture(test_jpg_jpg);
    GRRLIB_texImg *tex_test_bmp = GRRLIB_LoadTexture(test_bmp_bmp);

    GRRLIB_bytemapFont *bmf_Font1 = GRRLIB_LoadBMF(ocean_bmf);
    GRRLIB_bytemapFont *bmf_Font2 = GRRLIB_LoadBMF(frontal_bmf);

    GRRLIB_texImg *tex_sprite_png = GRRLIB_LoadTexture(sprite_png);
    GRRLIB_InitTileSet(tex_sprite_png, 24, 32, 0);

    GRRLIB_texImg *tex_BMfont1 = GRRLIB_LoadTexture(BMfont1_png);
    GRRLIB_InitTileSet(tex_BMfont1, 32, 32, 32);

    GRRLIB_texImg *tex_BMfont2 = GRRLIB_LoadTexture(BMfont2_png);
    GRRLIB_InitTileSet(tex_BMfont2, 16, 16, 32);

    GRRLIB_texImg *tex_BMfont3 = GRRLIB_LoadTexture(BMfont3_png);
    GRRLIB_InitTileSet(tex_BMfont3, 32, 32, 32);

    GRRLIB_texImg *tex_BMfont4 = GRRLIB_LoadTexture(BMfont4_png);
    GRRLIB_InitTileSet(tex_BMfont4, 16, 16, 32);

    GRRLIB_texImg *tex_BMfont5 = GRRLIB_LoadTexture(BMfont5_png);
    GRRLIB_InitTileSet(tex_BMfont5, 8, 16, 0);

    while(1) {
        PAD_ScanPads();
        const u32 paddown = PAD_ButtonsDown(0);
        const u32 padheld = PAD_ButtonsHeld(0);

        GRRLIB_FillScreen(GRRLIB_BLACK);    // Clear the screen
        switch(page)
        {
            case 1:   // Draw images
                GRRLIB_Printf(5, 25, tex_BMfont2, GRRLIB_WHITE, 1, "IMAGES DEMO");

                GRRLIB_DrawImg(10, 50, tex_test_jpg, 0, 1, 1, GRRLIB_WHITE);  // Draw a jpeg
                GRRLIB_DrawImg(350, 50, tex_test_bmp, 0, 4, 4, GRRLIB_WHITE); // Draw a bitmap

                // Draw a sprite
                GRRLIB_DrawTile(600, 400, tex_sprite_png, 0, 2, 2, GRRLIB_WHITE, 12*4); // Rupee
                GRRLIB_DrawTile(320+left, 240+top, tex_sprite_png, 0, 2, 2, GRRLIB_WHITE, frame);
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
                    if(padheld & PAD_BUTTON_LEFT || padheld & PAD_BUTTON_RIGHT ||
                        padheld & PAD_BUTTON_UP || padheld & PAD_BUTTON_DOWN) {
                        frame++;
                    }
                    else {
                        frame = direction + 1;  // Not moving
                        wait = TILE_DELAY;      // Ready to move
                    }
                    if(frame > direction + 2)
                        frame = direction;
                }
                break;
            case 2:   // Draw shapes
                GRRLIB_Printf(5, 25, tex_BMfont2, GRRLIB_WHITE, 1, "SHAPES DEMO");

                GRRLIB_Rectangle(100, 100, 200, 100, GRRLIB_RED, 1);
                GRRLIB_Line(100, 100, 350, 200, GRRLIB_SILVER);
                GRRLIB_NGoneFilled(triangle, trianglecolor, 3);
                GRRLIB_Rectangle(left + 150, top + 150, 200, 200, 0x0000FFC8, 1); // Blue with alpha
                GRRLIB_Circle(left + 300, top + 300, 50, GRRLIB_OLIVE, 1);
                break;
            default: // Print some text
                GRRLIB_Printf(5, 25, tex_BMfont2, GRRLIB_WHITE, 1, "GRRLIB %s TEXT DEMO", GRRLIB_VER_STRING);

                GRRLIB_Printf(5, 100, tex_BMfont4, GRRLIB_WHITE, 1, "TO QUIT PRESS THE START/PAUSE BUTTON.");
                GRRLIB_Printf(5, 140, tex_BMfont4, GRRLIB_YELLOW, 1, "USE Y OR X TO MOVE ACROSS PAGES.");
                GRRLIB_Printf(5, 180, tex_BMfont4, GRRLIB_GREEN, 1, "USE THE D-PAD TO MOVE STUFF.");
                GRRLIB_Printf(left, top+350, tex_BMfont3, 0XFFFFFF50, 1, "TEXT WITH ALPHA");
                GRRLIB_Printf(left, top+400, tex_BMfont5, GRRLIB_LIME, 1, "This font has the 128 ASCII characters");
                GRRLIB_PrintBMF(left, top+420, bmf_Font2, "%s", bmf_Font2->name);
        }
        GRRLIB_Printf(500, 27, tex_BMfont5, GRRLIB_WHITE, 1, "Current FPS: %d", FPS);

        if(paddown & PAD_BUTTON_START) {
            break;
        }
        if(padheld & PAD_BUTTON_LEFT) {
            if(padheld & PAD_BUTTON_B || page == 1)
                left -= 2;
            else
                left--;
            direction_new = TILE_LEFT;  // for tile example
        }
        if(padheld & PAD_BUTTON_RIGHT) {
            if(padheld & PAD_BUTTON_B || page == 1)
                left += 2;
            else
                left++;
            direction_new = TILE_RIGHT;  // for tile example
        }
        if(padheld & PAD_BUTTON_UP) {
            if(padheld & PAD_BUTTON_B || page == 1)
                top -= 2;
            else
                top--;
            direction_new = TILE_UP;  // for tile example
        }
        if(padheld & PAD_BUTTON_DOWN) {
            if(padheld & PAD_BUTTON_B || page == 1)
                top += 2;
            else
                top++;
            direction_new = TILE_DOWN;  // for tile example
        }
        if(paddown & PAD_BUTTON_Y) {
            page--;
            left = 0;
            top = 0;
            if(page < 0)
                page = 2;
        }
        if(paddown & PAD_BUTTON_X) {
            page++;
            left = 0;
            top = 0;
            if(page > 2)
                page = 0;
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
static u8 CalculateFrameRate(void) {
    static u8 frameCount = 0;
    static u32 lastTime;
    static u8 FPS = 0;
    const u32 currentTime = ticks_to_millisecs(gettime());

    frameCount++;
    if(currentTime - lastTime > 1000) {
        lastTime = currentTime;
        FPS = frameCount;
        frameCount = 0;
    }
    return FPS;
}
