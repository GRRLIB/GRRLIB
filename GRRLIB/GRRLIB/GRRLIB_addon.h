/*===========================================
        GRRLIB (GX version) 4.0.0 addon
        Code     : NoNameNo
        Additional Code : Crayon & Xane
        GX hints : RedShade
===========================================*/

#ifndef __GRRLIB_ADDON__
#define __GRRLIB_ADDON__

/**
 * @file GRRLIBaddon.h
 * GRRLIB library.
 */


#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

void GRRLIB_addon_Init();
void GRRLIB_addon_Exit();
GRRLIB_texImg *GRRLIB_LoadTextureFromFile(const char *filename);
bool GRRLIB_ScrShot(const char*);
void GRRLIB_addon_Button(int indice, int x,int y,u32 col, int wpadx, int wpady, u32 WPADDown, u32 WPADHeld, int but, int *resdown, int *resheld, char toto[]);


#ifdef __cplusplus
   }
#endif /* __cplusplus */

#endif

