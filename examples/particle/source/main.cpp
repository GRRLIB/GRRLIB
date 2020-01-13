/*===========================================
        GRRLIB (GX Version)
        Example code by Xane

        This example shows a basic particle
        engine creating a Smokebomb.
============================================*/
#include <grrlib.h>

#include <cstdlib>
#include <wiiuse/wpad.h>
#include <cmath>
#include <ogc/lwp_watchdog.h>
#include <vector>

// Include Graphics
#include "RGFX_Background_jpg.h"
#include "RGFX_Crosshair_png.h"
#include "RGFX_Smoke_png.h"
#include "RGFX_Font_png.h"


// Define Effects
#define EFFECT_SMOKEBOMB    1

// Random Number (0 - 1) in float
#define RANDOM   ((((float)(rand() % 12))/12)-0.5)

// Basic structure to hold particle data
typedef struct Particle {
    u8 id;
    float x, y;
    float sx, sy;
    u16 rot;
    u8 frame, framecnt, framedelay;
    u8 red, green, blue;
    float scale, alpha;
    float sscale, salpha;
    float scolor;
    GRRLIB_texImg *tex;
} Particle;

// Vector used as a container to iterate through all members of GRRLIB_texImg
static std::vector<GRRLIB_texImg *> TextureList;
static std::vector<Particle *> ParticleList;
static std::vector<Particle *> ParticleListTmp;

// Declare static functions
static void ExitGame();
static void createEffect( u8 id, int _x, int _y );
static void createParticle( u8 _id, int _x, int _y, float _scale, float _alpha, u8 _red, u8 _green, u8 _blue );
static bool updateParticle( Particle *part );
static u8 CalculateFrameRate();
static u8 ClampVar8 (f32 Value);

// Prepare Graphics
GRRLIB_texImg *GFX_Background;
GRRLIB_texImg *GFX_Crosshair;
GRRLIB_texImg *GFX_Smoke;
GRRLIB_texImg *GFX_Font;


int main() {
    ir_t P1Mote;

    u8 FPS = 0;

    // Init GRRLIB & WiiUse
    GRRLIB_Init();
    u16 WinW = rmode->fbWidth;
    u16 WinH = rmode->efbHeight;
    WPAD_Init();
    WPAD_SetIdleTimeout( 60 * 10 );
    WPAD_SetDataFormat( WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR );

    // Load textures
    GFX_Background = GRRLIB_LoadTextureJPG(RGFX_Background_jpg);
    GFX_Crosshair  = GRRLIB_LoadTexturePNG(RGFX_Crosshair_png);
    GFX_Smoke      = GRRLIB_LoadTexturePNG(RGFX_Smoke_png);
    GFX_Font       = GRRLIB_LoadTexturePNG(RGFX_Font_png);
    GRRLIB_InitTileSet( GFX_Font, 8, 16, 32 );

    // Set handles
    GRRLIB_SetMidHandle( GFX_Crosshair, true );
    GRRLIB_SetMidHandle( GFX_Smoke, true );

    // Feed the vector with the textures
    TextureList = { GFX_Background, GFX_Crosshair, GFX_Smoke, GFX_Font };

    while (true) {
        WPAD_ScanPads();
        u32 WPADKeyDown = WPAD_ButtonsDown(WPAD_CHAN_0);
        WPAD_SetVRes(WPAD_CHAN_0, WinW, WinH);
        WPAD_IR(WPAD_CHAN_0, &P1Mote);

        // Resetting Vars
        GRRLIB_SetBlend( GRRLIB_BLEND_ALPHA );
        u32 ParticleCnt = 0;

        // WiiMote IR Viewport correction
        int P1MX = P1Mote.sx - 150;
        int P1MY = P1Mote.sy - 150;

        // Drawing Background
        GRRLIB_DrawImg( 0, 0, GFX_Background, 0, 1, 1, RGBA(255, 255, 255, 255) );

        // Add any pending objects into the main container
        if(ParticleListTmp.empty() == false) {
            ParticleList.insert(ParticleList.end(), ParticleListTmp.begin(), ParticleListTmp.end());
            ParticleListTmp.clear();
        }

        // Update and draw all particles
        for (auto PartIter = ParticleList.begin(); PartIter != ParticleList.end();) {
            if (updateParticle((*PartIter)) == true) {
                GRRLIB_DrawImg( (*PartIter)->x, (*PartIter)->y, (*PartIter)->tex, (*PartIter)->rot, (*PartIter)->scale, (*PartIter)->scale, RGBA( (*PartIter)->red, (*PartIter)->green, (*PartIter)->blue, ClampVar8((*PartIter)->alpha*255) ) );
            } else {
                free( (*PartIter) );
                ParticleList.erase(PartIter);
                continue;
            }
            ParticleCnt++;
            PartIter++;
        }

        // Draw Crosshair
        GRRLIB_DrawImg( P1MX, P1MY, GFX_Crosshair, 0, 1, 1, RGBA(255, 255, 255, 255) );

        // Draw Text
        GRRLIB_Rectangle( 28, 28, 280, 20, RGBA(0, 0, 0, 160), 1 );
        GRRLIB_Printf   ( 32, 32, GFX_Font, 0xFFFFFFFF, 1, "Point your WiiMote on the screen." );
        GRRLIB_Rectangle( 28, 48, 200, 16, RGBA(0, 0, 0, 160), 1 );
        GRRLIB_Printf   ( 32, 48, GFX_Font, 0xFFFFFFFF, 1, "Number of Particle: %d", ParticleCnt );
        GRRLIB_Rectangle( 28, 64, 64, 16, RGBA(0, 0, 0, 160), 1 );
        GRRLIB_Printf   ( 32, 64, GFX_Font, 0xFFFFFFFF, 1, "FPS: %d", FPS );

        // Renders the Scene
        GRRLIB_Render();
        FPS = CalculateFrameRate();

        if (WPADKeyDown & WPAD_BUTTON_B) {
            createEffect( EFFECT_SMOKEBOMB, P1MX, P1MY );
        }
        if (WPADKeyDown & WPAD_BUTTON_HOME) {
            break;
        }
    }
    ExitGame();
    return 0;
}

static void createEffect( u8 id, int _x, int _y ) {
    u8 _ColorAdd = 0;
    switch (id) {
        case EFFECT_SMOKEBOMB:
            for (u8 i = 0; i < 5; i++) {
                createParticle( 1, (_x + (RANDOM * 10)), (_y + (RANDOM * 10)), (1.4f+(RANDOM*0.20)), 1.0f, 64, 64, 64 );
            }
            for (u8 i = 0; i < 20; i++) {
                createParticle( 3, (_x + (RANDOM * 50)), (_y + (RANDOM * 50)), 1.25f, 1.5f, 92, 92, 92 );
            }
            for (u8 i = 0; i < 5; i++) {
                _ColorAdd = (RANDOM * 75);
                createParticle( 2, (_x + (RANDOM * 40)), (_y + (RANDOM * 40)), (1.0f+(RANDOM*0.20)), 1.0f, 128+_ColorAdd, 128+_ColorAdd, 128+_ColorAdd );
            }
            break;
    }
}

static void createParticle( u8 _id, int _x, int _y, float _scale, float _alpha, u8 _red, u8 _green, u8 _blue ) {
    Particle *part = (struct Particle *)calloc(1, sizeof(Particle));
    part->id  = _id;
    part->x   = _x;
    part->y   = _y;
    part->rot   = rand() % 360;
    part->red   = _red;
    part->green = _green;
    part->blue  = _blue;
    part->scale = _scale;
    part->alpha = _alpha;

    part->tex = GFX_Smoke;
    part->sy = RANDOM;
    part->sx = RANDOM;
    part->sscale = 0.9985;
    part->salpha = 0.985;
    switch (part->id) {
        case 1:
            part->sy = RANDOM * 0.5;
            part->sx = RANDOM * 0.5;
            part->sscale = 0.999;
            part->salpha = 0.992;
            part->framedelay = 10;
            part->framecnt = 2;
            break;
        case 2:
            part->scolor = 0.98;
            part->salpha = 0.95;
            break;
        case 3:
            part->sy = (RANDOM * 8);
            part->sx = (RANDOM * 8);
            part->salpha = 0.85;
            part->scolor = 0.95;
            break;
    }
    ParticleListTmp.push_back( part );
}

static bool updateParticle( Particle *part ) {
    if (part->alpha < 0.05) { part->alpha -= 0.001; }
    if (part->alpha < 0.1) { part->alpha -= 0.001; }

    part->x += part->sx;
    part->y += part->sy;
    part->scale *= part->sscale;
    part->alpha *= part->salpha;
    switch (part->id) {
        case 1:
            if (part->alpha < 0.25) { part->alpha -= 0.001; }
            if (part->framecnt == 0) {
                part->framecnt = 20;
                part->red   -= 1;
                part->green -= 1;
                part->blue  -= 1;
            }
            part->framecnt -= 1;
            break;
        case 2:
        case 3:
            part->red   *= part->scolor;
            part->green *= part->scolor;
            part->blue  *= part->scolor;
            break;
    }
    if ((part->scale < 0) || (part->alpha < 0)) {
        return false;
    }
    return true;
}

static void ExitGame() {
    // Free all memory used by textures.
    for (auto &TexIter : TextureList) {
        GRRLIB_FreeTexture(TexIter);
    }
    TextureList.clear();

    // Deinitialize GRRLIB & Video
    GRRLIB_Exit();

    // Exit application
    exit(0);
}

/**
 * This function calculates the number of frames we render each second.
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

/**
 * A helper function for the YCbCr -> RGB conversion.
 * Clamps the given value into a range of 0 - 255 and thus preventing an overflow.
 * @param Value The value to clamp. Using float to increase the precision. This makes a full spectrum (0 - 255) possible.
 * @return Returns a clean, clamped unsigned char.
 */
static u8 ClampVar8 (f32 Value) {
    Value = std::roundf(Value);
    if (Value < 0) {
        Value = 0;
    }
    else if (Value > 255) {
        Value = 255;
    }

    return (u8)Value;
}
