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

#include <stdlib.h>
#include <stdio.h>
#include "aggBMP.h"

int main(int argc, const char * argv[])
{
    // These values store some important bitmap information. just keep them around
    struct aggBMP_header fileHeader;
    struct aggDIB_header dibHeader;
    struct aggBMP_pixeldata fileData;

    // This function will fill in all the data in those structs, based on what you want your image to be.
    aggBMP_CreateBitmapImage( &fileHeader, &dibHeader, &fileData, 128, 128, AGG_DEFAULT_DEPTH);

    // Fill all the pixels with some color. In this case, red.
    for ( int x = 0; x < dibHeader.dib_width; x ++ ) {
        for ( int y = 0; y < dibHeader.dib_height; y ++ ) {
            aggBMP_SetPixelColor( &fileData, x, y, (struct aggBMP_color){1.0f,0.0f,0.0f});
        }
    }
    
    // output the finished file by giving it a stream. in this case, a file stream.
    FILE* outputFile;
    outputFile = fopen( "MyNewFile.bmp", "wb" );
    aggBMP_OutputBMPFile( outputFile, &fileHeader, &dibHeader, &fileData );
    fclose( outputFile );
    
    // The file should exist now in the project directory!
    
    return 0;
}

