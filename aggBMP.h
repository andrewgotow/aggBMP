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

#ifndef aggBMP_aggBMP_h
#define aggBMP_aggBMP_h

#define AGG_DEPTH_8                 8
#define AGG_DEPTH_16                16
#define AGG_DEPTH_24                24

#define AGG_DEFAULT_DEPTH           AGG_DEPTH_24
#define AGG_DEFAULT_RESOLUTION_X    2835 
#define AGG_DEFAULT_RESOLUTION_Y    2835


// The bmp file format is fairly standard, this library simply provides a number of functions that allow for
// quick and easy writing of bmp files.
// http://en.wikipedia.org/wiki/BMP_file_format

/*This block of bytes is at the start of the file and is used to identify the file. A typical application reads
 this block first to ensure that the file is actually a BMP file and that it is not damaged. The first two bytes
 of the BMP file format are the character 'B' then the character 'M' in 1-byte ASCII encoding. All of the integer
 values are stored in little-endian format (i.e. least-significant byte first).*/
struct aggBMP_header {
    unsigned short  bmp_hfield;     // The header field, (usually set to 'BM')
    unsigned int    bmp_fsize;       // the size of the file, in bytes
    unsigned int    bmp_appdata;   // application specific space.
    unsigned int    bmp_poffset;    // the starting address of the pixel data.
};

/*This block of bytes tells the application detailed information about the image, which will be used to display
 the image on the screen. The block also matches the header used internally by Windows and OS/2 and has several
 different variants. All of them contain a dword (32 bit) field, specifying their size, so that an application 
 can easily determine which header is used in the image. The reason that there are different headers is that Microsoft 
 extended the DIB format several times. The new extended headers can be used with some GDI functions instead of the 
 older ones, providing more functionality. Since the GDI supports a function for loading bitmap files, typical Windows 
 applications use that functionality. One consequence of this is that for such applications, the BMP formats that they 
 support match the formats supported by the Windows version being run. See the table below for more information.*/
struct aggDIB_header {
    unsigned int    dib_hsize;      // the size of the header (40 bytes)
    int             dib_width;      // the width of the image
    int             dib_height;     // the height of the image
    unsigned short  dib_cplanes;    // the number of color planes being used (must be 1)
    unsigned short  dib_depth;      // bits per pixel, typical values are (1,4,8,16,24,32)
    unsigned int    dib_comp;       // compression method being used. usually set to 0, (BI_RGB)
    unsigned int    dib_isize;      // the size of the raw bitmap data. not file size
    int             dib_hres;       // horizontal resolution of image. (pixels per meter)
    int             dib_vres;       // vertical resolution of image. (pixels per meter)
    unsigned int    dib_ncolors;    // number of colors in the color pallette (usually set to 0)
    unsigned int    dib_nicolors;   // number of important colors in the color pallette (usually 0, and ignored)
};

// This structure just contains pixel data information for the bitmap object.
struct aggBMP_pixeldata {
    int bmp_pwidth;
    int bmp_pheight;
    int bmp_pdepth;
    int bmp_pdataLen;
    char* bmp_pdata;
};

// from 0 to 1.
struct aggBMP_color {
    float red;
    float green;
    float blue;
};

// initializes a bitmap object
void aggBMP_CreateBitmapImage( struct aggBMP_header* bmpHeader, struct aggDIB_header* dibHeader, struct aggBMP_pixeldata* data, int width, int height, int depth );
// sets a single pixel in the bitmap object.
void aggBMP_SetPixelColor( struct aggBMP_pixeldata* data, int x, int y, struct aggBMP_color color );
// returns a single pixel's color in the bitmap object.
struct aggBMP_color aggBMP_GetPixelColor( struct aggBMP_pixeldata* data, int x, int y );
// outputs a BMP file to a given stream
void aggBMP_OutputBMPFile( FILE* fileStream, struct aggBMP_header* bmpHeader, struct aggDIB_header* dibHeader, struct aggBMP_pixeldata* data );

#
#endif
