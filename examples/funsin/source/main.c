/*===========================================
    NoNameNo simple Gradient Sinusoid
        A good start to code a nice plasma
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <math.h>
#include <wiiuse/wpad.h>


int main() {
    // Initialise the Graphics & Video subsystem
    GRRLIB_Init();

    // Initialise the Wii Remotes
    WPAD_Init();


    const int adc1=0;
    const int offset1=0;
    const int origin1=0;
    const int length1=1280;
    const int amp1=100;
    const int period1=1;
    const float pas1=(period1*360.0f)/length1;
    float siny1 = offset1*pas1;

    const int adc2=1;
    const int offset2=0;
    const int origin2=0;
    const int length2=1280;
    const int amp2=40;
    const int period2=2;
    const float pas2=(period2*360.0f)/length2;
    float siny2 = offset2*pas2;

    const int adc3=-3;
    const int offset3=0;
    const int origin3=0;
    const int length3=1280;
    const int amp3=30;
    const int period3=1;
    const float pas3=(period3*360.0f)/length3;
    float siny3 = offset3*pas3;

    const int adc4=-7;
    const int offset4=0;
    const int origin4=0;
    const int length4=1280;
    const int amp4=70;
    const int period4=1;
    const float pas4=(period4*360.0f)/length4;
    float siny4 = offset4*pas4;


    while (1) {
        GRRLIB_FillScreen(0x000000FF);
        WPAD_ScanPads();  // Scan the Wii Remotes
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME)  break;
        const float old1=siny1;
        const float old2=siny2;
        const float old3=siny3;
        const float old4=siny4;


        for (u16 x=0; x<=640; x++) {
            siny1+=pas1;
            siny2+=pas2;
            siny3+=pas3;
            siny4+=pas4;

            GX_Begin(GX_LINES, GX_VTXFMT0, 2);
                GX_Position3f32(x, 0, 0);
                GX_Color1u32(0x000000FF);
                GX_Position3f32(x, (sin(DegToRad(siny1))*amp1+origin1)+(sin(DegToRad(siny2))*amp2+origin2)+(sin(DegToRad(siny3))*amp3+origin3)+(sin(DegToRad(siny4))*amp4+origin4)+240,  0);
                GX_Color1u32(0xFF00007F);
            GX_End();
            GX_Begin(GX_LINES, GX_VTXFMT0, 2);
                GX_Position3f32(x, (sin(DegToRad(siny1))*amp1+origin1)+(sin(DegToRad(siny2))*amp2+origin2)+(sin(DegToRad(siny3))*amp3+origin3)+(sin(DegToRad(siny4))*amp4+origin4)+240,  0);
                GX_Color1u32(0xFF00007F);
                GX_Position3f32(x, 480, 0);
                GX_Color1u32(0x000000FF);
            GX_End();

        }
        siny1=old1+(adc1*pas1);
        siny2=old2+(adc2*pas2);
        siny3=old3+(adc3*pas3);
        siny4=old4+(adc4*pas4);

        GRRLIB_Render();  // Render the frame buffer to the TV
    }

    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB

    exit(0);  // Use exit() to exit a program, do not use 'return' from main()
}
