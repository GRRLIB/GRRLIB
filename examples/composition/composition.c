/*------------------------------------------------------------------------------
Copyright (c) 2009 The GRRLIB Team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
------------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
// Test Harness for alpha composition functions
// The composition function and this demo were both written by BlueChip
/*
  PaintShopPro_X2 readings:
      |        | Blue-over-Red | Red-over-Blue |
      |--------+---------------+---------------|
      | Normal | 0x2D'00'D2'D8 | 0x96'00'69'D8 |
      |        |               |               |
*/
//------------------------------------------------------------------------------

#include <stdlib.h>  // exit
#include <wiiuse/wpad.h>
#include <grrlib.h>

#include "red50.h"
#include "blue70.h"

//------------------------------------------------------------------------------
void  wscan(ir_t* ir) ;

// Wiimote overscan (to allow the cursor to move off the dge of the screen)
#define WOVERX (200)
#define WOVERY (300)

#define WMU_MAX (2)  // How many wiimotes to read

ir_t  ir[WMU_MAX];

//------------------------------------------------------------------------------
void  test_compose(void)
{
  int  redOnTop = 0;  // Red square starts behind the blue one

  // Our two translucent test images
  GRRLIB_texImg*  red = GRRLIB_LoadTexture(red50);
  GRRLIB_texImg*  blu = GRRLIB_LoadTexture(blue70);

  // The canvas we will be composing on
  GRRLIB_texImg*  cnv = GRRLIB_CreateEmptyTexture(400,400);

  // Loop until home key pressed
  do {
    // Scan the wiimotes
    wscan(ir);

    // 'A' toggles z-coord of red sqaure (on-top / behind)
    if (WPAD_ButtonsDown(0) & WPAD_BUTTON_A)  redOnTop ^= 1 ;

    // Clear canvas
    GRRLIB_ClearTex(cnv);

    // Compose canvas
    if (!redOnTop)
      GRRLIB_Compose(ir[0].x-180,ir[0].y-180, red, cnv, GRRLIB_COMPOSE_NORMAL);
    GRRLIB_Compose(125,125, blu, cnv, GRRLIB_COMPOSE_NORMAL) ;
    if ( redOnTop)
      GRRLIB_Compose(ir[0].x-180,ir[0].y-180, red, cnv, GRRLIB_COMPOSE_NORMAL);

    // Paint the screen black
    GRRLIB_FillScreen(0x000000FF);

    // Draw frame
    GRRLIB_Rectangle(38,38,404,404,0x808080FF,false);
    GRRLIB_Rectangle(39,39,402,402,0x808080FF,false);

    // Draw Test bar
    GRRLIB_Rectangle(283,50,20,380,0xFFFFFFFF, true);

    // Draw Composed Canvas
    GRRLIB_DrawImg(40,40, cnv,0.0, 1.0,1.0, 0xFFFFFFFF);

    // Test card (alpha performed by Wii)
    GRRLIB_Rectangle(570,50,20,450,0xFFFFFFFF, true);
    GRRLIB_DrawImg(450, 65, red,0.0, 1.0,1.0, 0xFFFFFFFF);
    GRRLIB_DrawImg(450,165, blu,0.0, 1.0,1.0, 0xFFFFFFFF);
    GRRLIB_DrawImg(450,265, red,0.0, 1.0,1.0, 0xFFFFFFFF);

    // Draw it [and workaround bug in GRRLIB]
    GRRLIB_Render();
    if (rmode->viTVMode &VI_NON_INTERLACE)  VIDEO_WaitVSync() ;

  } while(!(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME));  // Exit on home key

  // Free up the resources we are using to hold the canvas and layers
  GRRLIB_FreeTexture(blu);
  GRRLIB_FreeTexture(red);
  GRRLIB_FreeTexture(cnv);

}

//------------------------------------------------------------------------------
void  wscan(ir_t* ir)
{
  int  i;

  WPAD_ScanPads();

  for (i = 0;  i <  WMU_MAX;  i++) {
    WPAD_IR(i, &ir[i]);
    ir->x -= WOVERX/2;
    ir->y -= WOVERY/2;
  }
}

//------------------------------------------------------------------------------
int  main(int argc, char** argv)
{
  (void)argc;
  (void)argv;

  int  i;

  // Init graphics subsystem
  GRRLIB_Init();

  // Init the Wiimotes
  WPAD_Init();
  for (i = 0;  i < WMU_MAX;  i++) {
    WPAD_SetVRes(i, rmode->fbWidth+WOVERX, rmode->xfbHeight+WOVERY);
    WPAD_SetDataFormat(i, WPAD_FMT_BTNS_ACC_IR);
  }

  // Call the demo
  test_compose();

  // Clear up memory
  GRRLIB_Exit();

  exit(0);
}
