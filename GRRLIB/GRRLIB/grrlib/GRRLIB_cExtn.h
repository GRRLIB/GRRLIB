/*------------------------------------------------------------------------------
Copyright (c) 2010 The GRRLIB Team

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
 * @file GRRLIB_cExtn.h
 * Inline functions to offer additional C primitives.
 */

#include <math.h>

/**
 * A helper function for the YCbCr -> RGB conversion.
 * Clamps the given value into a range of 0 - 255 and thus preventing an overflow.
 * @param Value The value to clamp. Using float to increase the precision. This makes a full spectrum (0 - 255) possible.
 * @return Returns a clean, clamped unsigned char.
 */
INLINE
u8  GRRLIB_ClampVar8 (f32 Value) {
    Value = roundf(Value);
    if      (Value < 0)    Value = 0;
    else if (Value > 255)  Value = 255;

    return (u8)Value;
}
