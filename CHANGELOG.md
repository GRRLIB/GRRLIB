# Change Log
All notable changes to this project will be documented in this file.

## [Unreleased]
- libpng was updated to version 1.6.34
- zlib was updated to version 1.2.11
- libjpeg was updated to version 9b (17-Jan-2016)
- FreeType updated to version 2.4.12
- Fully compatible with devkitPPC release 29

## [4.3.2] - 2012-08-12
- libpng was updated to version 1.5.12
- zlib was updated to version 1.2.7
- libjpeg was updated to version 8d
- FreeType updated to version 2.4.10
- GRRLIB_ClampVar8 was removed from GRRLIB
- Removed warnings from GRRLIB_LoadBMF 

## [4.3.1] - 2010-10-22
- libpng was updated to version 1.4.4
- FreeType updated to version 2.4.3
- GRRLIB is compatible with libogc 1.8.4 

## [4.3.0] - 2010-06-28
- libpng was updated to version 1.4.2
- libjpeg was updated to version 8b
- zlib was updated to version 1.2.5
- FreeType 2.3.12 support (first support)
- 3D primitive drawing: torus, sphere, cube, cylinder and cone
- Light functions (diffuse, specular, spot) + ambient
- Splitted functions for rotation/translation/scaling 3d object.
- Here is the list a new added samples demo code :
  - 3d_light1 -> Simple Diffuse light sample code
  - 3d_light2 -> Simple Lights and GRRLIB_ObjectViewInv sample
  - 3d_light3 -> A little Specular light sample code
  - 3d_light4 -> Spot Light Sample Code
  - 3D_sample5 -> Simple demo of rotation/translation/scaling 3d object.
  - ttf -> TrueType Font demo 

## [4.2.0] - 2009-12-16
- First support to 3D functions
- GRRLIB_CompoStart and GRRLIB_CompoEnd for real GX compositing with transparency support
- GRRLIB_Screen2Texture is now fully optimized
- USB_Gecko output facilities
- GRRLIB_Compose was deleted since it was not fully using GX
- GRRLIB_GetColor was deleted, the RGBA macro should be used instead
- Lot of new sample code :
  - 3D_CubedTileDemo (How to use dynamic texturing)
  - 3D_sample1 (A simple rotating flat cube)
  - 3D_sample2 (A simple rotating textured cube)
  - 3D_sample3 (A textured cube and compositing)
  - 3D_sample4 (A complex object rotating)
  - basic_drawing (How to use some basic GRRLIB functions)
  - bitmap_fx (Effects ShowRoom)
  - blending (How to use blending mode)
  - compositing (A simple compositing how to)
  - funsin (A gradient sinusoid dancing)
  - particle (A nice particle sample code)
  - template (Use this as a basis for your project)
  - TileDemo (This will show you how to use tiles/tileset and map)
  - unlimited2d (A faky technic for unlimited sprites)
  - unlimited3d (Same as above but with 3D) 

## [4.1.1] - 2009-11-24
- Fully compatible with devkitPro release 18 and 19 (code and examples)
- libpng was updated to version 1.2.40
- Support for MS-Windows Bitmap format uncompressed (1-bit, 4-bit, 8-bit, 24-bit and 32-bit)
- New function called GRRLIB_DrawPart to draw a specific part of a texture
- Extra parameters to GRRLIB_Screen2Texture
- Video is now initialized even without a SD card
- Fixed a problem with scaling images
- Documentation improvement 

## 4.1.0 - 2009-10-05
- Completely new file structure with sub-folders
- Installer for vendor libraries (jpeg, png, pngu)
- Makefile for GRRLIB
- Many (all suitable) functions are now inlined
- Support for the recent changes to libogc
- Alpha compositor function added
- Correct use of pointers (no more struct passing)
- Speed improvement, bug fixing and more... 

## 4.0.0 - 2009-03-05
- Color format changed for ALL GRRLib functions (now its RGBA) to fit to GX_Color format and use GX_Color1u32
- GRRLIB_LoadTexture now Auto Detect PNG or JPEG
- GRRLib introduce a new texture structure (easier to handle texture width, height, etc ...)
- Add void GRRLIB_InitTileSet(struct GRRLIB_texImg *tex, unsigned int tilew, unsigned int tileh, unsigned int tilestart);
- GRRLIB_DrawImg recoded for simpler use
- GRRLIB_DrawTile recoded for simpler use
- InitVideo() and GRRLIB_Start() merge into GRRLIB_Init().
- Add GRRLIB_PtInRect, GRRLIB_RectInRect and GRRLIB_RectOnRect. (to detect hot zone)
- GRRLIB_GetPixelFromtexImg and GRRLIB_SetPixelTotexImg (to directly read/write in texture)
- GRRLIB_CreateEmptyTexture and GRRLIB_FlushTex
- New Bitmap FX
- Add GRRLIB_Exit to free the memory allocated by GRRLIB 

[Unreleased]: https://github.com/GRRLIB/GRRLIB/compare/4.3.2...HEAD
[4.3.2]: https://github.com/GRRLIB/GRRLIB/compare/4.3.1...4.3.2
[4.3.1]: https://github.com/GRRLIB/GRRLIB/compare/4.3.0...4.3.1
[4.3.0]: https://github.com/GRRLIB/GRRLIB/compare/4.2.0...4.3.0
[4.2.0]: https://github.com/GRRLIB/GRRLIB/compare/4.1.1...4.2.0
[4.1.1]: https://github.com/GRRLIB/GRRLIB/compare/4.1.0...4.1.1
