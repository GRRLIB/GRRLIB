/*------------------------------------------------------------------------------
Copyright (c) 2009-2024 The GRRLIB Team

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

#include <gccore.h>
#include <stdarg.h>
#include <stdio.h>

static bool geckoinit = false;

/**
 * Initialize USB Gecko.
 * @return Returns @c true if everything worked, @c false if problems occurred.
 */
bool GRRLIB_GeckoInit(void) {
    const s32 geckoattached = usb_isgeckoalive(EXI_CHANNEL_1);
    if (geckoattached) {
        usb_flush(EXI_CHANNEL_1);
        geckoinit = true;
        return true;
    }
    else {
        return false;
    }
}

/**
 * Print Gecko.
 * @param text Text to print.
 * @param ... Optional arguments.
 */
void  GRRLIB_GeckoPrintf (const char *text, ...) {
    char tmp[1024];

    if (geckoinit == false) {
        return;
    }

    va_list argp;
    va_start(argp, text);
    const int size = vsnprintf(tmp, sizeof(tmp), text, argp);
    va_end(argp);

    usb_sendbuffer_safe(1, tmp, size);
}
