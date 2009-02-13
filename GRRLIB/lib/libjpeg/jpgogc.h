/****************************************************************************
* libjpeg - 6b wrapper
*
* The version of libjpeg used in libOGC has been modified to include a memory
* source data manager (jmemsrc.c).
*
* softdev November 2006
****************************************************************************/
#ifndef __JPGLIB__
#define __JPGLIB__

#include <jpeglib.h>

typedef struct {
    char           *inbuffer;
    char           *outbuffer;
    int             inbufferlength;
    int             outbufferlength;
    int             width;
    int             height;
    int             num_colours;
    int             dct_method;
    int             dither_mode;
    int             greyscale;
} JPEGIMG;

int             JPEG_Decompress(JPEGIMG * jpgimg);

#endif
