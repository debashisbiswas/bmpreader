#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#pragma pack(1) // tell compiler not to pad struct, need exact sizes

// found at https://engineering.purdue.edu/ece264/17au/hw/HW15
typedef struct
{                               // Total: 54 bytes
    uint16_t  type;             // Magic identifier: 0x4d42
    uint32_t  size;             // File size in bytes
    uint16_t  reserved1;        // Not used
    uint16_t  reserved2;        // Not used
    uint32_t  offset;           // Offset to image data in bytes from beginning of file (54 bytes)
    uint32_t  dib_header_size;  // DIB Header size in bytes (40 bytes)
    int32_t   width_px;         // Width of the image
    int32_t   height_px;        // Height of image
    uint16_t  num_planes;       // Number of color planes
    uint16_t  bits_per_pixel;   // Bits per pixel
    uint32_t  compression;      // Compression type
    uint32_t  image_size_bytes; // Image size in bytes
    int32_t   x_resolution_ppm; // Pixels per meter
    int32_t   y_resolution_ppm; // Pixels per meter
    uint32_t  num_colors;       // Number of colors  
    uint32_t  important_colors; // Important colors
} BMPHeader;

typedef struct
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} BMPColor;

typedef struct 
{
    BMPHeader header;
    BMPColor** pixelData; // 2d array, with coordinates for pixels 
} BMPImage;

void invertImage( BMPImage* aImage )
{
    for( int y = 0; y < aImage->header.height_px; y++ )
    {
        for( int x = 0; x < aImage->header.width_px; x++ )
        {
            aImage->pixelData[ x ][ y ].blue = UCHAR_MAX - aImage->pixelData[ x ][ y ].blue;
            aImage->pixelData[ x ][ y ].green = UCHAR_MAX - aImage->pixelData[ x ][ y ].green;
            aImage->pixelData[ x ][ y ].red = UCHAR_MAX - aImage->pixelData[ x ][ y ].red;
        }
    }
}

void grayscaleImage( BMPImage* aImage )
{
    for( int y = 0; y < aImage->header.height_px; y++ )
    {
        for( int x = 0; x < aImage->header.width_px; x++ )
        {
            // aImage->pixelData[ x ][ y ].blue = aImage->pixelData[ x ][ y ].blue;
            aImage->pixelData[ x ][ y ].green = aImage->pixelData[ x ][ y ].blue;
            aImage->pixelData[ x ][ y ].red = aImage->pixelData[ x ][ y ].blue;
        }
    }
}

void writeBMP( BMPImage* aImage, char* aFilename )
{
    FILE* newFile;
    newFile = fopen( aFilename, "w" );
    fwrite( &( aImage->header ), sizeof( aImage->header ), 1, newFile );

    for( int y = 0; y < aImage->header.height_px; y++ )
    {
        for( int x = 0; x < aImage->header.width_px; x++ )
        {
            fwrite( &( aImage->pixelData[ x ][ y ] ), sizeof( aImage->pixelData[ x ][ y ] ), 1, newFile );
        }
        
        // write padding at the end of the row
        uint16_t thePadding = 0;
        fwrite( &thePadding, sizeof( uint16_t ), 1, newFile );
    }

    printf( "New file written as %s\n", aFilename );

    fclose( newFile );
}

int main( int argc, char* argv[] )
{
    if( argc == 2 )
    {
        FILE* theFile;
        const char* filename = argv[ 1 ];
        if( ( theFile = fopen( filename, "r" ) ) )
        {
            BMPImage theOriginalImage;
            fread( &(theOriginalImage.header), sizeof( BMPHeader ), 1, theFile );
            theOriginalImage.pixelData = 0;
            theOriginalImage.pixelData = ( BMPColor** )malloc( theOriginalImage.header.width_px * sizeof( BMPColor** ) );
            for( int i = 0; i < theOriginalImage.header.width_px; i++ )
            {
                theOriginalImage.pixelData[ i ] = ( BMPColor* )malloc( theOriginalImage.header.height_px * sizeof( BMPColor* ) );
            }

            // print out header information
            printf( "HEADER INFO FOR %s:\n", filename );
            printf( "Type: %x\n", theOriginalImage.header.type );
            printf( "File size (bytes): %d\n", theOriginalImage.header.size );
            printf( "Offset to image data: %d\n", theOriginalImage.header.offset );
            printf( "Image width: %d\n", theOriginalImage.header.width_px );
            printf( "Image height: %d\n", theOriginalImage.header.height_px );
            printf( "Bits per pixel: %d\n", theOriginalImage.header.bits_per_pixel );
            printf( "Compression type: %d\n", theOriginalImage.header.compression );
            printf( "Image size (bytes): %d\n", theOriginalImage.header.image_size_bytes );
            printf( "Horizontal pixels per meter: %d\n", theOriginalImage.header.x_resolution_ppm );
            printf( "Vertical pixels per meter: %d\n", theOriginalImage.header.y_resolution_ppm );
            printf( "Number of colors: %d\n", theOriginalImage.header.num_colors );
            
            int rowSize = (int)( floor( ( theOriginalImage.header.bits_per_pixel * theOriginalImage.header.width_px + 31 ) / 32 ) * 4 );
            printf( "Calculated - bytes per pixel: %d\n", theOriginalImage.header.bits_per_pixel / 8 );
            printf( "Calculated - row size: %d\n", rowSize );
            printf( "Calculated - file size: %d\n", rowSize * theOriginalImage.header.height_px + theOriginalImage.header.offset );

            printf( "-----------------\n" );

            // loops through every row (from bottom to top)
            for( int y = 0; y < theOriginalImage.header.height_px; y++ )
            {
                // loops within the row, less than or equal to accomodate padding at the end of each line
                for( int x = 0; x <= theOriginalImage.header.width_px; x++ )
                {
                    if( x < theOriginalImage.header.width_px )
                    {
                        fread( &( theOriginalImage.pixelData[ x ][ y ] ), sizeof( BMPColor ), 1, theFile );
                    }
                    else
                    {
                        uint16_t thePadding;
                        fread( &thePadding, sizeof( uint16_t ), 1, theFile );
                    }
                }
            }

            //BMPImage theNewImage;
            //memcpy( &theNewImage, &theOriginalImage, sizeof( BMPImage ) );

            // invertImage( &theOriginalImage );
            writeBMP( &theOriginalImage, "remade.bmp" );

            // freeing memory
            for( int i = 0; i < theOriginalImage.header.width_px; i++ )
            {
                free( theOriginalImage.pixelData[ i ] );
            }
            free( theOriginalImage.pixelData );
            fclose( theFile );
        }
        else
        {
            printf( "%s does not exist!\n", filename );
        }
    }
    else
    {
        printf( "Usage: %s [path to bmp file]\n", argv[ 0 ] );
    }
    
    return 0;
}