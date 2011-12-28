/*------------------------------------------------------------------------------
Copyright (c) 2012 The GRRLIB Team

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

/*
 * @file GRRLIB_private.h
 * The symbols declared in this file are PRIVATE.
 * They are not part of the GRRLIB public
 * interface, and are not recommended for use by regular applications.
 * Some of them may become public in the future; others may stay private,
 * change in an incompatible way, or even disappear.
 */

#ifndef __GRRLIB_PRIVATE_H__
#define __GRRLIB_PRIVATE_H__

#include <ogc/libversion.h>

/**
 * Used for version checking.
 * @param a Major version number.
 * @param b Minor version number.
 * @param c Revision version number.
 */
#define GRRLIB_VERSION(a,b,c) ((a)*65536+(b)*256+(c))

//------------------------------------------------------------------------------
// GRRLIB_ttf.c - FreeType function for GRRLIB
int GRRLIB_InitTTF();
void GRRLIB_ExitTTF();

#endif // __GRRLIB_PRIVATE_H__
