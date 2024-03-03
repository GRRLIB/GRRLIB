/********************************************************************************************

PNGU Version : 0.2a

Coder : frontier

More info : http://frontier-dev.net

********************************************************************************************/
#ifndef __PNGU__
#define __PNGU__

#include <stdint.h>

// Return codes
#define PNGU_OK							0
#define PNGU_ODD_WIDTH					1
#define PNGU_ODD_STRIDE					2
#define PNGU_INVALID_WIDTH_OR_HEIGHT	3
#define PNGU_FILE_IS_NOT_PNG			4
#define PNGU_UNSUPPORTED_COLOR_TYPE		5
#define PNGU_NO_FILE_SELECTED			6
#define PNGU_CANT_OPEN_FILE				7
#define PNGU_CANT_READ_FILE				8
#define PNGU_LIB_ERROR					9

// Color types
#define PNGU_COLOR_TYPE_GRAY			1
#define PNGU_COLOR_TYPE_GRAY_ALPHA		2
#define PNGU_COLOR_TYPE_PALETTE			3
#define PNGU_COLOR_TYPE_RGB				4
#define PNGU_COLOR_TYPE_RGB_ALPHA		5
#define PNGU_COLOR_TYPE_UNKNOWN 		6


#ifdef __cplusplus
	extern "C" {
#endif

typedef struct
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} PNGUCOLOR;

typedef struct
{
	uint32_t imgWidth; // In pixels
	uint32_t imgHeight; // In pixels
	uint32_t imgBitDepth; // In bitx
	uint32_t imgColorType; // PNGU_COLOR_TYPE_*
	uint32_t validBckgrnd; // Non zero if there is a background color
	PNGUCOLOR bckgrnd; // Background color
	uint32_t numTrans; // Number of transparent colors
	PNGUCOLOR *trans; // Transparent colors
} PNGUPROP;

// Image context, always initialize with SelectImageFrom* and free with ReleaseImageContext
struct _IMGCTX;
typedef struct _IMGCTX *IMGCTX;


/****************************************************************************
*							 Pixel conversion								*
****************************************************************************/

// Macro to convert RGB8 values to RGB565
#define PNGU_RGB8_TO_RGB565(r,g,b) ( ((((uint16_t) r) & 0xF8U) << 8) | ((((uint16_t) g) & 0xFCU) << 3) | (((uint16_t) b) >> 3) )

// Macro to convert RGBA8 values to RGB5A3
#define PNGU_RGB8_TO_RGB5A3(r,g,b,a)	(uint16_t) (((a & 0xE0U) == 0xE0U) ? \
										(0x8000U | ((((uint16_t) r) & 0xF8U) << 7) | ((((uint16_t) g) & 0xF8U) << 2) | (((uint16_t) b) >> 3)) : \
										(((((uint16_t) a) & 0xE0U) << 7) | ((((uint16_t) r) & 0xF0U) << 4) | (((uint16_t) g) & 0xF0U) | ((((uint16_t) b) & 0xF0U) >> 4)))

// Function to convert two RGB8 values to YCbYCr
uint32_t PNGU_RGB8_TO_YCbYCr (uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2);

// Function to convert an YCbYCr to two RGB8 values.
void PNGU_YCbYCr_TO_RGB8 (uint32_t ycbycr, uint8_t *r1, uint8_t *g1, uint8_t *b1, uint8_t *r2, uint8_t *g2, uint8_t *b2);


/****************************************************************************
*							 Image context handling							*
****************************************************************************/

// Selects a PNG file, previosly loaded into a buffer, and creates an image context for subsequent procesing.
IMGCTX PNGU_SelectImageFromBuffer (const void *buffer);

// Selects a PNG file, from any devoptab device, and creates an image context for subsequent procesing.
IMGCTX PNGU_SelectImageFromDevice (const char *filename);

// Frees resources associated with an image context. Always call this function when you no longer need the IMGCTX.
void PNGU_ReleaseImageContext (IMGCTX ctx);


/****************************************************************************
*							 Miscelaneous									*
****************************************************************************/

// Retrieves info from selected PNG file, including image dimensions, color format, background and transparency colors.
int PNGU_GetImageProperties (IMGCTX ctx, PNGUPROP *fileproperties);


/****************************************************************************
*							 Image conversion								*
****************************************************************************/

// Expands selected image into an YCbYCr buffer. You need to specify context, image dimensions,
// destination address and stride in pixels (stride = buffer width - image width).
int PNGU_DecodeToYCbYCr (IMGCTX ctx, uint32_t width, uint32_t height, void *buffer, uint32_t stride);

// Macro for decoding an image inside a buffer at given coordinates.
#define PNGU_DECODE_TO_COORDS_YCbYCr(ctx,coordX,coordY,imgWidth,imgHeight,bufferWidth,bufferHeight,buffer)	\
																											\
		PNGU_DecodeToYCbYCr (ctx, imgWidth, imgHeight, ((void *) buffer) + (coordY) * (bufferWidth) * 2 +	\
							(coordX) * 2, (bufferWidth) - (imgWidth))

// Expands selected image into a linear RGB565 buffer. You need to specify context, image dimensions,
// destination address and stride in pixels (stride = buffer width - image width).
int PNGU_DecodeToRGB565 (IMGCTX ctx, uint32_t width, uint32_t height, void *buffer, uint32_t stride);

// Macro for decoding an image inside a buffer at given coordinates.
#define PNGU_DECODE_TO_COORDS_RGB565(ctx,coordX,coordY,imgWidth,imgHeight,bufferWidth,bufferHeight,buffer)	\
																											\
		PNGU_DecodeToRGB565 (ctx, imgWidth, imgHeight, ((void *) buffer) + (coordY) * (bufferWidth) * 2 +	\
							(coordX) * 2, (bufferWidth) - (imgWidth))

// Expands selected image into a linear RGBA8 buffer. You need to specify context, image dimensions,
// destination address, stride in pixels and default alpha value, which is used if the source image
// doesn't have an alpha channel.
int PNGU_DecodeToRGBA8 (IMGCTX ctx, uint32_t width, uint32_t height, void *buffer, uint32_t stride, uint8_t default_alpha);

// Macro for decoding an image inside a buffer at given coordinates.
#define PNGU_DECODE_TO_COORDS_RGBA8(ctx,coordX,coordY,imgWidth,imgHeight,default_alpha,bufferWidth,bufferHeight,buffer)	\
																											\
		PNGU_DecodeToRGBA8 (ctx, imgWidth, imgHeight, ((void *) buffer) + (coordY) * (bufferWidth) * 2 +	\
							(coordX) * 2, (bufferWidth) - (imgWidth), default_alpha)

// Expands selected image into a 4x4 tiled RGB565 buffer. You need to specify context, image dimensions
// and destination address.
int PNGU_DecodeTo4x4RGB565 (IMGCTX ctx, uint32_t width, uint32_t height, void *buffer);

// Expands selected image into a 4x4 tiled RGB5A3 buffer. You need to specify context, image dimensions,
// destination address and default alpha value, which is used if the source image doesn't have an alpha channel.
int PNGU_DecodeTo4x4RGB5A3 (IMGCTX ctx, uint32_t width, uint32_t height, void *buffer, uint8_t default_alpha);

// Expands selected image into a 4x4 tiled RGBA8 buffer. You need to specify context, image dimensions,
// destination address.
uint8_t * PNGU_DecodeTo4x4RGBA8 (IMGCTX ctx, uint32_t width, uint32_t height, int * dstWidth, int * dstHeight);

// Encodes an YCbYCr image in PNG format and stores it in the selected device or memory buffer. You need to
// specify context, image dimensions, destination address and stride in pixels (stride = buffer width - image width).
int PNGU_EncodeFromYCbYCr (IMGCTX ctx, uint32_t width, uint32_t height, void *buffer, uint32_t stride);

int PNGU_EncodeFromRGB (IMGCTX ctx, uint32_t width, uint32_t height, void *buffer, uint32_t stride);
int PNGU_EncodeFromGXTexture (IMGCTX ctx, uint32_t width, uint32_t height, void *buffer, uint32_t stride);
int PNGU_EncodeFromEFB (IMGCTX ctx, uint32_t width, uint32_t height, uint32_t stride);

// Macro for encoding an image stored into an YCbYCr buffer at given coordinates.
#define PNGU_ENCODE_TO_COORDS_YCbYCr(ctx,coordX,coordY,imgWidth,imgHeight,bufferWidth,bufferHeight,buffer)	\
																											\
		PNGU_EncodeFromYCbYCr (ctx, imgWidth, imgHeight, ((void *) buffer) + (coordY) * (bufferWidth) * 2 +	\
							(coordX) * 2, (bufferWidth) - (imgWidth))

#ifdef __cplusplus
	}
#endif

#endif
