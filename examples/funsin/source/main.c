/*===========================================
    NoNameNo simple Gradient Sinusoid
        A good start to code a nice plasma
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <math.h>
#include <wiiuse/wpad.h>


int main() {
    int offset1, offset2, offset3, offset4;
    int periode1, periode2, periode3, periode4;
    int length1, length2, length3, length4;
    int amp1, amp2, amp3, amp4;
    int origine1, origine2, origine3, origine4;
    int adc1, adc2, adc3, adc4;
    float siny1, siny2, siny3, siny4;
    int x;
    float pas1, pas2, pas3, pas4;

    // Initialise the Graphics & Video subsystem
    GRRLIB_Init();

    // Initialise the Wiimotes
    WPAD_Init();


    adc1=0;
    offset1=0;
    origine1=0;
    length1=1280;
    amp1=100;
    periode1=1;
    pas1=(periode1*360.0F)/length1;
    siny1 = offset1*pas1;

    adc2=1;
    offset2=0;
    origine2=0;
    length2=1280;
    amp2=40;
    periode2=2;
    pas2=(periode2*360.0F)/length2;
    siny2 = offset2*pas2;

    adc3=-3;
    offset3=0;
    origine3=0;
    length3=1280;
    amp3=30;
    periode3=1;
    pas3=(periode3*360.0F)/length3;
    siny3 = offset3*pas3;

    adc4=-7;
    offset4=0;
    origine4=0;
    length4=1280;
    amp4=70;
    periode4=1;
    pas4=(periode4*360.0F)/length4;
    siny4 = offset4*pas4;


    while (1) {
        GRRLIB_FillScreen(0x000000FF);
        WPAD_ScanPads();  // Scan the Wiimotes
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME)  break;
        float old1=siny1;
        float old2=siny2;
        float old3=siny3;
        float old4=siny4;


        for (x=0; x<=640; x++) {
            siny1+=pas1;
            siny2+=pas2;
            siny3+=pas3;
            siny4+=pas4;

            GX_Begin(GX_LINES, GX_VTXFMT0, 2);
                GX_Position3f32(x, 0, 0);
                GX_Color1u32(0x000000FF);
                GX_Position3f32(x, (sin(DegToRad(siny1))*amp1+origine1)+(sin(DegToRad(siny2))*amp2+origine2)+(sin(DegToRad(siny3))*amp3+origine3)+(sin(DegToRad(siny4))*amp4+origine4)+240,  0);
                GX_Color1u32(0xFF00007F);
            GX_End();
            GX_Begin(GX_LINES, GX_VTXFMT0, 2);
                GX_Position3f32(x, (sin(DegToRad(siny1))*amp1+origine1)+(sin(DegToRad(siny2))*amp2+origine2)+(sin(DegToRad(siny3))*amp3+origine3)+(sin(DegToRad(siny4))*amp4+origine4)+240,  0);
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
