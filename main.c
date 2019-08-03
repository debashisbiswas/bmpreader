#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#pragma pack(1) // no padding on structs, need exact sizes

// image offsets based on header type
#define BITMAPCOREHEADER_IMAGE_OFFSET 26
#define BITMAPINFOHEADER_IMAGE_OFFSET 54
#define BITMAPV4HEADER_IMAGE_OFFSET 122
#define BITMAPV5HEADER_IMAGE_OFFSET 138

#define MAXPADDINGSIZE 3

typedef struct // first 14 bytes of every bitmap file
{
    uint16_t type;             // identifier, two ASCII characters, usually "BM"
    uint32_t size;             // file size (bytes)
    uint16_t reserved1;        // unused
    uint16_t reserved2;        // unused
    uint32_t image_offset;     // offset to image data in bytes from origin of file
} BITMAPFILEHEADER;

typedef struct 
{
    uint32_t header_size;      // this header's size
    int32_t  width_px;         // image width
    int32_t  height_px;        // image height
    uint16_t num_planes;       // number of color planes
    uint16_t bits_per_pixel;   // bits per pixel
} BITMAPCOREHEADER;

// altered from https://engineering.purdue.edu/ece264/17au/hw/HW15
typedef struct // 40 bytes, for total offset of 54 bytes
{
    uint32_t header_size;  // DIB header size in bytes (40 bytes)
    int32_t  width_px;         // image width
    int32_t  height_px;        // image height
    uint16_t num_planes;       // number of color planes
    uint16_t bits_per_pixel;   // bits per pixel
    uint32_t compression;      // compression type
    uint32_t image_size_bytes; // image size (bytes)
    int32_t  x_resolution_ppm; // pixels per meter
    int32_t  y_resolution_ppm; // pixels per meter
    uint32_t num_colors;       // number of colors  
    uint32_t important_colors; // Important colors
} BITMAPINFOHEADER;

// https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-wmf/21164882-3cb3-49d6-8133-74396704f14d
typedef struct
{
    uint32_t ciexyzX; // A 32-bit 2.30 fixed point type that defines the x chromaticity value.
    uint32_t ciexyzY; // A 32-bit 2.30 fixed point type that defines the y chromaticity value.
    uint32_t ciexyzZ; // A 32-bit 2.30 fixed point type that defines the z chromaticity value.
} CIEXYZ;

// https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-wmf/07b63c50-c07b-44bc-9523-be4e90d20a8e
typedef struct
{
    CIEXYZ ciexyzRed;   // A 96-bit CIEXYZ Object that defines the red chromaticity values.
    CIEXYZ ciexyzGreen; // A 96-bit CIEXYZ Object that defines the green chromaticity values.
    CIEXYZ ciexyzBlue;  // A 96-bit CIEXYZ Object that defines the blue chromaticity values.
} CIEXYZTRIPLE;

// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapv4header
typedef struct // 108 bytes, for total offset of 122 bytes
{
    uint32_t     bV4Size;
    int32_t      bV4Width;
    int32_t      bV4Height;
    uint16_t     bV4Planes;
    uint16_t     bV4BitCount;
    uint32_t     bV4V4Compression;
    uint32_t     bV4SizeImage;
    int32_t      bV4XPelsPerMeter;
    int32_t      bV4YPelsPerMeter;
    uint32_t     bV4ClrUsed;
    uint32_t     bV4ClrImportant;
    uint32_t     bV4RedMask;
    uint32_t     bV4GreenMask;
    uint32_t     bV4BlueMask;
    uint32_t     bV4AlphaMask;
    uint32_t     bV4CSType;
    CIEXYZTRIPLE bV4Endpoints;
    uint32_t     bV4GammaRed;
    uint32_t     bV4GammaGreen;
    uint32_t     bV4GammaBlue;
} BITMAPV4HEADER;

// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapv5header
typedef struct // 124 bytes, for a total offset of 138 bytes
{
    uint32_t     bV5Size;
    int32_t      bV5Width;
    int32_t      bV5Height;
    uint16_t     bV5Planes;
    uint16_t     bV5BitCount;
    uint32_t     bV5Compression;
    uint32_t     bV5SizeImage;
    int32_t      bV5XPelsPerMeter;
    int32_t      bV5YPelsPerMeter;
    uint32_t     bV5ClrUsed;
    uint32_t     bV5ClrImportant;
    uint32_t     bV5RedMask;
    uint32_t     bV5GreenMask;
    uint32_t     bV5BlueMask;
    uint32_t     bV5AlphaMask;
    uint32_t     bV5CSType;
    CIEXYZTRIPLE bV5Endpoints;
    uint32_t     bV5GammaRed;
    uint32_t     bV5GammaGreen;
    uint32_t     bV5GammaBlue;
    uint32_t     bV5Intent;
    uint32_t     bV5ProfileData;
    uint32_t     bV5ProfileSize;
    uint32_t     bV5Reserved;
} BITMAPV5HEADER;

typedef struct
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} BitmapColor;

// ---------- HEADER READ FUNCTIONS ----------

BITMAPFILEHEADER readBitmapFileHeader( FILE* aFile )
{
    BITMAPFILEHEADER theHeader;
    if( aFile )
    {
        fread( &theHeader, sizeof( BITMAPFILEHEADER ), 1, aFile );
    }
    return theHeader;
}

BITMAPCOREHEADER readBitmapCoreHeader( FILE* aFile )
{
    BITMAPCOREHEADER theHeader;
    if( aFile )
    {
        fread( &theHeader, sizeof( BITMAPCOREHEADER ), 1, aFile );
    }
    return theHeader;
}

BITMAPINFOHEADER readBitmapInfoHeader( FILE* aFile )
{
    BITMAPINFOHEADER theHeader;
    if( aFile )
    {
        fread( &theHeader, sizeof( BITMAPINFOHEADER ), 1, aFile );
    }
    return theHeader;
}

BITMAPV4HEADER readBitmapV4Header( FILE* aFile )
{
    BITMAPV4HEADER theHeader;
    if( aFile )
    {
        fread( &theHeader, sizeof( BITMAPV4HEADER ), 1, aFile );
    }
    return theHeader;
}

BITMAPV5HEADER readBitmapV5Header( FILE* aFile )
{
    BITMAPV5HEADER theHeader;
    if( aFile )
    {
        fread( &theHeader, sizeof( BITMAPV5HEADER ), 1, aFile );
    }
    return theHeader;
}

// ---------- HEADER WRITE FUNCTIONS ----------

void writeBitmapFileHeader( BITMAPFILEHEADER aHeader, FILE* aFile )
{
    if( aFile )
    {
        fwrite( &aHeader, sizeof( BITMAPFILEHEADER ), 1, aFile );
    }
}

void writeBitmapCoreHeader( BITMAPCOREHEADER aHeader, FILE* aFile )
{
    if( aFile )
    {
        fwrite( &aHeader, sizeof( BITMAPCOREHEADER ), 1, aFile );
    }
}

void writeBitmapInfoHeader( BITMAPINFOHEADER aHeader, FILE* aFile )
{
    if( aFile )
    {
        fwrite( &aHeader, sizeof( BITMAPINFOHEADER ), 1, aFile );
    }
}

void writeBitmapV4Header( BITMAPV4HEADER aHeader, FILE* aFile )
{
    if( aFile )
    {
        fwrite( &aHeader, sizeof( BITMAPV4HEADER ), 1, aFile );
    }
}

void writeBitmapV5Header( BITMAPV5HEADER aHeader, FILE* aFile )
{
    if( aFile )
    {
        fwrite( &aHeader, sizeof( BITMAPV5HEADER ), 1, aFile );
    }
}

// ---------- IMAGE DATA FUNCTIONS ----------

int findNextMultipleOf4( int aNum )
{
    return ( aNum + 3 ) & ~0x03;
}

int calculatePaddingSize( int32_t aImageWidth, uint16_t aBitsPerPixel )
{
    // number of pixels in a row, divide by 8 to convert bits to bytes 
    int thePixelDataPerRow = aBitsPerPixel * aImageWidth / 8;

    // padding must bring row size to next multiple of 4
    return findNextMultipleOf4( thePixelDataPerRow ) - thePixelDataPerRow;
}

BitmapColor** readImageData( int32_t aImageWidth, int32_t aImageHeight, uint16_t aBitsPerPixel, FILE* aFile )
{
    BitmapColor** theImageData = ( BitmapColor** )malloc( aImageWidth * sizeof( BitmapColor* ) );
    for( int i = 0; i < aImageWidth; i++ )
    {
        theImageData[ i ] = ( BitmapColor* )malloc( aImageHeight * sizeof( BitmapColor ) );
    }

    int thePaddingSize = calculatePaddingSize( aImageWidth, aBitsPerPixel );
    // memory does not need to be initialized as it will be used to temporarily store data while reading
    
    // loops through every row (from bottom to top)
    for( int y = 0; y < aImageHeight; y++ )
    {
        // loops within the row, less than or equal to accomodate padding at the end of each line
        for( int x = 0; x <= aImageWidth; x++ )
        {
            if( x < aImageWidth )
            {
                fread( &( theImageData[ x ][ y ] ), sizeof( BitmapColor ), 1, aFile );
            }
            else
            {
                // skip over the padding
                fseek( aFile, sizeof( uint8_t ) * thePaddingSize, SEEK_CUR );
            }
        }
    }

    return theImageData;
}

void freeImageData( int32_t aImageWidth, int32_t aImageHeight, BitmapColor** aImageData )
{
    for( int i = 0; i < aImageWidth; i++ )
    {
        free( aImageData[ i ] );
    }
    free( aImageData );
}

void writeImageData( int32_t aImageWidth, int32_t aImageHeight, uint32_t aBitsPerPixel, BitmapColor** aImageData, FILE* aFile )
{
    int thePaddingSize = calculatePaddingSize( aImageWidth, aBitsPerPixel );
    uint8_t thePadding[ MAXPADDINGSIZE ];
    
    for( int i = 0; i < MAXPADDINGSIZE; i++ )
    {
        thePadding[ i ] = 0;
    }

    for( int y = 0; y < aImageHeight; y++ )
    {
        for( int x = 0; x < aImageWidth; x++ )
        {
            fwrite( &( aImageData[ x ][ y ] ), sizeof( aImageData[ x ][ y ] ), 1, aFile );
        }
        
        // write padding at the end of the row
        fwrite( thePadding, sizeof( uint8_t ), thePaddingSize, aFile );
    }
}

// ---------- IMAGE MANIPULATION FUNCTIONS ----------

void invertImage( uint32_t aImageWidth, uint32_t aImageHeight, BitmapColor** aImage )
{
    for( int y = 0; y < aImageHeight; y++ )
    {
        for( int x = 0; x < aImageWidth; x++ )
        {
            aImage[ x ][ y ].blue = UINT8_MAX - aImage[ x ][ y ].blue;
            aImage[ x ][ y ].green = UINT8_MAX - aImage[ x ][ y ].green;
            aImage[ x ][ y ].red = UINT8_MAX - aImage[ x ][ y ].red;
        }
    }
}

void grayscaleImageBlue( uint32_t aImageWidth, uint32_t aImageHeight, BitmapColor** aImage )
{
    for( int y = 0; y < aImageHeight; y++ )
    {
        for( int x = 0; x < aImageWidth; x++ )
        {
            aImage[ x ][ y ].blue = aImage[ x ][ y ].blue;
            aImage[ x ][ y ].green = aImage[ x ][ y ].blue;
            aImage[ x ][ y ].red = aImage[ x ][ y ].blue;
        }
    }
}

void grayscaleImageGreen( uint32_t aImageWidth, uint32_t aImageHeight, BitmapColor** aImage )
{
    for( int y = 0; y < aImageHeight; y++ )
    {
        for( int x = 0; x < aImageWidth; x++ )
        {
            aImage[ x ][ y ].blue = aImage[ x ][ y ].green;
            aImage[ x ][ y ].green = aImage[ x ][ y ].green;
            aImage[ x ][ y ].red = aImage[ x ][ y ].green;
        }
    }
}

void grayscaleImageRed( uint32_t aImageWidth, uint32_t aImageHeight, BitmapColor** aImage )
{
    for( int y = 0; y < aImageHeight; y++ )
    {
        for( int x = 0; x < aImageWidth; x++ )
        {
            aImage[ x ][ y ].blue = aImage[ x ][ y ].red;
            aImage[ x ][ y ].green = aImage[ x ][ y ].red;
            aImage[ x ][ y ].red = aImage[ x ][ y ].red;
        }
    }
}

int main( int argc, char* argv[] )
{
    if( argc == 3 )
    {
        const char* theOriginalFilename = argv[ 1 ];
        const char* theNewFilename = argv[ 2 ];
        FILE* theFile = fopen( theOriginalFilename, "r" );
        if( theFile )
        {
            BITMAPFILEHEADER theBitmapFileHeader = readBitmapFileHeader( theFile );
            
            switch( theBitmapFileHeader.image_offset )
            {
                case BITMAPCOREHEADER_IMAGE_OFFSET:
                {
                    BITMAPCOREHEADER theInformationHeader = readBitmapCoreHeader( theFile );
                    BitmapColor** theImageData = readImageData( theInformationHeader.width_px,
                        theInformationHeader.height_px,
                        theInformationHeader.bits_per_pixel,
                        theFile );

                    FILE* theNewFile = fopen( theNewFilename, "w" );

                    invertImage( theInformationHeader.width_px, theInformationHeader.height_px, theImageData );
                        
                    writeBitmapFileHeader( theBitmapFileHeader, theNewFile );
                    writeBitmapCoreHeader( theInformationHeader, theNewFile );
                    writeImageData( theInformationHeader.width_px,
                        theInformationHeader.height_px,
                        theInformationHeader.bits_per_pixel,
                        theImageData,
                        theNewFile );

                    freeImageData( theInformationHeader.width_px, theInformationHeader.height_px, theImageData );
                    fclose( theNewFile );
                    break;
                }
                case BITMAPINFOHEADER_IMAGE_OFFSET:
                {
                    BITMAPINFOHEADER theInformationHeader = readBitmapInfoHeader( theFile );
                    BitmapColor** theImageData = readImageData( theInformationHeader.width_px,
                        theInformationHeader.height_px,
                        theInformationHeader.bits_per_pixel,
                        theFile );
                    
                    FILE* theNewFile = fopen( theNewFilename, "w" );

                    invertImage( theInformationHeader.width_px, theInformationHeader.height_px, theImageData );

                    writeBitmapFileHeader( theBitmapFileHeader, theNewFile );
                    writeBitmapInfoHeader( theInformationHeader, theNewFile );
                    writeImageData( theInformationHeader.width_px,
                        theInformationHeader.height_px,
                        theInformationHeader.bits_per_pixel,
                        theImageData,
                        theNewFile );

                    freeImageData( theInformationHeader.width_px, theInformationHeader.height_px, theImageData );
                    fclose( theNewFile );
                    break;
                }
                case BITMAPV4HEADER_IMAGE_OFFSET:
                {
                    BITMAPV4HEADER theInformationHeader = readBitmapV4Header( theFile );
                    BitmapColor** theImageData = readImageData( theInformationHeader.bV4Width,
                        theInformationHeader.bV4Height,
                        theInformationHeader.bV4BitCount,
                        theFile );

                    FILE* theNewFile = fopen( theNewFilename, "w" );

                    invertImage( theInformationHeader.bV4Width, theInformationHeader.bV4Height, theImageData );
                        
                    writeBitmapFileHeader( theBitmapFileHeader, theNewFile );
                    writeBitmapV4Header( theInformationHeader, theNewFile );
                    writeImageData( theInformationHeader.bV4Width,
                        theInformationHeader.bV4Height,
                        theInformationHeader.bV4BitCount,
                        theImageData,
                        theNewFile );

                    freeImageData( theInformationHeader.bV4Width, theInformationHeader.bV4Height, theImageData );
                    fclose( theNewFile );
                    break;
                }
                case BITMAPV5HEADER_IMAGE_OFFSET:
                {
                    BITMAPV5HEADER theInformationHeader = readBitmapV5Header( theFile );
                    BitmapColor** theImageData = readImageData( theInformationHeader.bV5Width, 
                        theInformationHeader.bV5Height, 
                        theInformationHeader.bV5BitCount, 
                        theFile );
                    
                    invertImage( theInformationHeader.bV5Width, theInformationHeader.bV5Height, theImageData );
                    
                    FILE* theNewFile = fopen( theNewFilename, "w" );

                    writeBitmapFileHeader( theBitmapFileHeader, theNewFile );
                    writeBitmapV5Header( theInformationHeader, theNewFile );
                    writeImageData( theInformationHeader.bV5Width,
                        theInformationHeader.bV5Height,
                        theInformationHeader.bV5BitCount,
                        theImageData,
                        theNewFile );

                    freeImageData( theInformationHeader.bV5Width, theInformationHeader.bV5Height, theImageData );
                    fclose( theNewFile );
                    break;
                }
            }

            fclose( theFile );
        }
        else
        {
            printf( "%s does not exist!\n", theOriginalFilename );
        }
    }
    else
    {
        printf( "Usage: %s [original BMP filename] [new BMP filename]\n", argv[ 0 ] );
    }
    
    return 0;
}