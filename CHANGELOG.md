# Changelog

All notable changes to this project will be documented in this file.

## [4.5.0][] - 2024-02-11

- Fixed compatibility issues with devkitPPC release 39.
- Fixed linking problem in examples with FreeType 2.13.2 (pkgrel 1 and 2).
- Added `GRRLIB_LoadTTFFromFile()` to load a TTF from a file.
- Added `GRRLIB_Ellipse()` to draw an ellipse.
- Changed `GRRLIB_PrintfTTF()` and `GRRLIB_PrintfTTFW()` so they use the alpha channel from the `color` parameter.
- Changed function arguments types in a few functions.
- Fixed documentation for `GRRLIB_Camera3dSettings()`, `GRRLIB_Screen2Texture()` and `GRRLIB_CompoEnd()`.

## [4.4.1][] - 2021-03-05

- Patched widescreen on Wii U.

## [4.4.0][] - 2020-01-12

- Removed libpng, zlib, libfreetype and libjpeg from the project. These libraries should now be installed in devkitPro with pacman.
- GRRLIB and pngu are now installed into the _portlibs_ folder instead of the _libogc_ folder.
- Removed `uint` declaration. The `u32` type should be used instead.
- The `GRRLIB_CreateEmptyTexture()` function is not inline anymore.

## [4.3.2][] - 2012-08-12

- libpng was updated to version 1.5.12
- zlib was updated to version 1.2.7
- libjpeg was updated to version 8d
- FreeType updated to version 2.4.10
- `GRRLIB_ClampVar8()` was removed from GRRLIB
- Removed warnings from `GRRLIB_LoadBMF()`

## [4.3.1][] - 2010-10-22

- libpng was updated to version 1.4.4
- FreeType updated to version 2.4.3
- GRRLIB is compatible with libogc 1.8.4

## [4.3.0][] - 2010-06-28

- libpng was updated to version 1.4.2
- libjpeg was updated to version 8b
- zlib was updated to version 1.2.5
- FreeType 2.3.12 support (first support)
- 3D primitive drawing: torus, sphere, cube, cylinder and cone
- Light functions (diffuse, specular, spot) + ambient
- Split functions for rotation/translation/scaling 3d object.
- Here is the list a new added samples demo code:
  - 3d_light1 -> Simple Diffuse light sample code
  - 3d_light2 -> Simple Lights and `GRRLIB_ObjectViewInv()` sample
  - 3d_light3 -> A little Specular light sample code
  - 3d_light4 -> Spot Light Sample Code
  - 3D_sample5 -> Simple demo of rotation/translation/scaling 3d object.
  - ttf -> TrueType Font demo

## [4.2.0][] - 2009-12-16

- First support to 3D functions
- `GRRLIB_CompoStart()` and `GRRLIB_CompoEnd()` for real GX compositing with transparency support
- `GRRLIB_Screen2Texture()` is now fully optimized
- USB_Gecko output facilities
- `GRRLIB_Compose()` was deleted since it was not fully using GX
- `GRRLIB_GetColor()` was deleted, the RGBA macro should be used instead
- Lot of new sample code:
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

## [4.1.1][] - 2009-11-24

- Fully compatible with devkitPro release 18 and 19 (code and examples)
- libpng was updated to version 1.2.40
- Support for MS-Windows Bitmap format uncompressed (1-bit, 4-bit, 8-bit, 24-bit and 32-bit)
- New function called `GRRLIB_DrawPart()` to draw a specific part of a texture
- Extra parameters to `GRRLIB_Screen2Texture()`
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

- Changed color format to RGBA for ALL GRRLib functions to fit to `GXColor` format and use `GX_Color1u32`
- `GRRLIB_LoadTexture()` now auto detect PNG or JPEG
- GRRLib introduce a new texture structure (easier to handle texture width, height, etc ...)
- Add `GRRLIB_InitTileSet()` to initialize a tile set
- `GRRLIB_DrawImg()` recoded for simpler use
- `GRRLIB_DrawTile()` recoded for simpler use
- `InitVideo()` and `GRRLIB_Start()` merge into `GRRLIB_Init()`.
- Add `GRRLIB_PtInRect()`, `GRRLIB_RectInRect()` and `GRRLIB_RectOnRect()` to detect hot zone
- `GRRLIB_GetPixelFromtexImg()` and `GRRLIB_SetPixelTotexImg()` to directly read/write in texture
- `GRRLIB_CreateEmptyTexture()` and `GRRLIB_FlushTex()`
- New Bitmap FX
- Add `GRRLIB_Exit()` to free the memory allocated by GRRLIB

[4.5.0]: https://github.com/GRRLIB/GRRLIB/compare/v4.4.1...v4.5.0
[4.4.1]: https://github.com/GRRLIB/GRRLIB/compare/v4.4.0...v4.4.1
[4.4.0]: https://github.com/GRRLIB/GRRLIB/compare/4.3.2...v4.4.0
[4.3.2]: https://github.com/GRRLIB/GRRLIB/compare/4.3.1...4.3.2
[4.3.1]: https://github.com/GRRLIB/GRRLIB/compare/4.3.0...4.3.1
[4.3.0]: https://github.com/GRRLIB/GRRLIB/compare/4.2.0...4.3.0
[4.2.0]: https://github.com/GRRLIB/GRRLIB/compare/4.1.1...4.2.0
[4.1.1]: https://github.com/GRRLIB/GRRLIB/compare/4.1.0...4.1.1
