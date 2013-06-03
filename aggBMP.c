//==============================================================================
/*
  https://github.com/andrewgotow/aggBMP

  Copyright (C) 2013, Andrew Gotow <andrewgotow@gmail.com>

  License: The MIT License (http://www.opensource.org/licenses/mit-license.php)

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
//==============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "aggBMP.h"
#include <string.h>

void aggBMP_CreateBitmapImage( struct aggBMP_header* bmpHeader, struct aggDIB_header* dibHeader, struct aggBMP_pixeldata* data, int width, int height, int depth )
{
    // Allocate the pixel buffer.
    // we need to add padding if the alignment is off.
    int pad = ((depth/8) * width) % 4;
    int pbuff_size = ((depth/8) * (width+pad) ) * height;
    
    data->bmp_pdata = (char*)malloc( pbuff_size );
    memset( data->bmp_pdata, 0, pbuff_size );
    data->bmp_pwidth = width;
    data->bmp_pheight = height;
    data->bmp_pdepth = depth;
    data->bmp_pdataLen = pbuff_size;
    
    // HEADER IS IN LITTLE ENDIAN FORMAT. BITS MUST ALL BE SWAPPED.
    // Setup the BMP header so that it contains the appropriate values.
    bmpHeader->bmp_hfield = 19778;//0x424D;
    bmpHeader->bmp_fsize = (int)(sizeof(struct aggBMP_header) + sizeof(struct aggDIB_header) + pbuff_size);
    bmpHeader->bmp_appdata = 0;
    bmpHeader->bmp_poffset = (int)(sizeof(struct aggBMP_header) + sizeof(struct aggDIB_header))-2;
    
    // Setup the DIB header so that it contains the proper values.
    dibHeader->dib_hsize = 40;
    dibHeader->dib_width = width;
    dibHeader->dib_height = height;
    dibHeader->dib_cplanes = 1;
    dibHeader->dib_depth = depth;
    dibHeader->dib_comp = 0;
    dibHeader->dib_isize = pbuff_size;
    dibHeader->dib_hres = AGG_DEFAULT_RESOLUTION_X;
    dibHeader->dib_vres = AGG_DEFAULT_RESOLUTION_Y;
    dibHeader->dib_ncolors = 0;
    dibHeader->dib_nicolors = 0;
}

void aggBMP_SetPixelColor( struct aggBMP_pixeldata* data, int x, int y, struct aggBMP_color color )
{
    // make 2 byte color values. (REQUIRES 24 bit precision).
    char redVal = (char)(255.0f * color.red);
    char greenVal = (char)(255.0f * color.green);
    char blueVal = (char)(255.0f * color.blue);
    
    // determine byte padding so we can index at the correct position.
    int pad = ((data->bmp_pdepth/8) * data->bmp_pwidth) % 4;
    int index = ((data->bmp_pwidth+pad) * y + x) * (data->bmp_pdepth/8);

    // set the values.
    data->bmp_pdata[index+0] = blueVal;
    data->bmp_pdata[index+1] = greenVal;
    data->bmp_pdata[index+2] = redVal;
}

struct aggBMP_color aggBMP_GetPixelColor( struct aggBMP_pixeldata* data, int x, int y )
{
    struct aggBMP_color returnColor;
    
    // determine byte padding so we can index at the correct position.
    int pad = ((data->bmp_pdepth/8) * data->bmp_pwidth) % 4;
    int index = ((data->bmp_pwidth+pad) * y + x) * (data->bmp_pdepth/8);
    
    // set the values.
    returnColor.blue = data->bmp_pdata[index+0] / 255.0f;
    returnColor.green = data->bmp_pdata[index+1] / 255.0f;
    returnColor.red = data->bmp_pdata[index+2] / 255.0f;
    
    return returnColor;
}

void aggBMP_OutputBMPFile( FILE* fileStream, struct aggBMP_header* bmpHeader, struct aggDIB_header* dibHeader, struct aggBMP_pixeldata* data )
{
    // For some reason byte packing issues came up (stupid compiler :P), because of this, values in the headers are written by hand.
    //fwrite( (char*)bmpHeader, 1, sizeof(struct aggBMP_header), fileStream);
    fwrite( &bmpHeader->bmp_hfield, sizeof(unsigned short), 1, fileStream);
    fwrite( &bmpHeader->bmp_fsize, sizeof(unsigned int), 1, fileStream);
    fwrite( &bmpHeader->bmp_appdata, sizeof(unsigned int), 1, fileStream);
    fwrite( &bmpHeader->bmp_poffset, sizeof(unsigned int), 1, fileStream);
    
    fwrite( (char*)dibHeader, 1, sizeof(struct aggDIB_header), fileStream);
    fwrite( (char*)data->bmp_pdata , 1, data->bmp_pdataLen, fileStream);
}