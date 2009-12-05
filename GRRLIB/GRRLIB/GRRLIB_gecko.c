#include <gccore.h>
#include <stdarg.h>
#include <stdio.h>

bool geckoinit = false;

bool GRRLIB_GeckoInit()
{
	u32 geckoattached = usb_isgeckoalive(EXI_CHANNEL_1);
	if (geckoattached)
	{
		usb_flush(EXI_CHANNEL_1);
		geckoinit = true;
		return true;
	}
	else return false;
}

void  GRRLIB_GeckoPrintf (const char *text, ...) {
    int size;
    char tmp[1024];

    if (!(geckoinit))return;

    va_list argp;
    va_start(argp, text);
    size = vsprintf(tmp, text, argp);
    va_end(argp);

    usb_sendbuffer_safe(1,tmp,size);
}

