==============================================================
  ________ __________ __________ .____     .___ __________ 
 /  _____/ \______   \\______   \|    |    |   |\______   \
/   \  ___  |       _/ |       _/|    |    |   | |    |  _/
\    \_\  \ |    |   \ |    |   \|    |___ |   | |    |   \
 \______  / |____|_  / |____|_  /|_______ \|___| |______  /
        \/         \/         \/         \/             \/ 
                   ________     _______   
                   \_____  \    \   _  \  
                     _(__  <    /  /_\  \ 
                    /       \   \  \_/   \
                   /______  / /\ \_____  /
                          \/  \/       \/ .5 ALPHA

===============================================================
Code : NoNameNo
Code Hints : RedShade

Info & Tutorial : http://grrlib.santo.fr
===============================================================

ChangeLog :

* Color format change for ALL GRRLib function (now its RGBA) to fit to GX_Color format and use GX_Color1u32
* added GRRLIB_LoadTextureFromFile  //---> Contribution by GRILLO
* GRRLib introduce a new texture structure (easier to handle texture width, height, etc ...):
	typedef struct GRRLIB_texImg{
		unsigned int w;
		unsigned int h;
		unsigned int tilew;
		unsigned int tileh;
		unsigned int nbtilew;
		unsigned int nbtileh;
		unsigned int tilestart;
		void *data;
	} GRRLIB_texImg;
* add void GRRLIB_InitTileSet(struct GRRLIB_texImg *tex, unsigned int tilew, unsigned int tileh, unsigned int tilestart);
* GRRLIB_Printf recoded to fit modification.
* GRRLIB_DrawImg recoded for simpler use
* GRRLIB_DrawTile recoded for simpler use + // --->Frame Correction by spiffen

have a look at the sample code to see how all this work ;)

Remember that 3.0.5 is a WIP preview for the soon coming GRRLIB 4.0 and it's not intend to be publicly released...
Contact me to provide me all your patch/addon/new functions...

NoNameNo.




