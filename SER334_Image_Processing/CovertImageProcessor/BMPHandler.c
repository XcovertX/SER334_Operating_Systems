/**
* BmpProcessor source - This read/ writes bmp headers, read/write DIB headers, constructs new bmp and dib headers
* reads/ writes pixels of bmp image files
*
* Completion time: 15h
*
* @author James Covert
* @version 1.0
*/

#include <stdio.h>
#include "Image.h"
#include "BMPHandler.h"


void readBMPHeader(FILE* file, struct BMP_Header* header){

    fread(&header->signature, sizeof(char)*2, 1, file);
    fread(&header->size, sizeof(int), 1, file);
    fread(&header->reserved_1, sizeof(short), 1, file);
    fread(&header->reserved_2, sizeof(short), 1, file);
    fread(&header->offset_pixel_array, sizeof(int), 1, file);
}

void writeBMPHeader(FILE* file, struct BMP_Header* header){

    fwrite(&header->signature, sizeof(char)*2, 1, file);
    fwrite(&header->size, sizeof(int), 1, file);
    fwrite(&header->reserved_1, sizeof(short), 1, file);
    fwrite(&header->reserved_2, sizeof(short), 1, file);
    fwrite(&header->offset_pixel_array, sizeof(int), 1, file);
}

/**
 * read DIB header from a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read
 * @param  header: Pointer to the destination DIB header
 */
void readDIBHeader(FILE* file, struct DIB_Header* header){

    fread(&header->DIBHeaderSize, sizeof(int), 1, file);
    fread(&header->width, sizeof(int), 1, file);
    fread(&header->height, sizeof(int), 1, file);
    fread(&header->Planes, sizeof(short), 1, file);
    fread(&header->BPP, sizeof(short), 1, file);
    fread(&header->Compression, sizeof(int), 1, file);
    fread(&header->size, sizeof(int), 1, file);
    fread(&header->XPPM, sizeof(int), 1, file);
    fread(&header->YPPM, sizeof(int), 1, file);
    fread(&header->CCT, sizeof(int), 1, file);
    fread(&header->ICC, sizeof(int), 1, file);
}

/**
 * write DIB header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being written
 * @param  header: The header to write to the file
 */
void writeDIBHeader(FILE* file, struct DIB_Header* header){

    fwrite(&header->DIBHeaderSize, sizeof(int), 1, file);
    fwrite(&header->width, sizeof(int), 1, file);
    fwrite(&header->height, sizeof(int), 1, file);
    fwrite(&header->Planes, sizeof(short), 1, file);
    fwrite(&header->BPP, sizeof(short), 1, file);
    fwrite(&header->Compression, sizeof(int), 1, file);
    fwrite(&header->size, sizeof(int), 1, file);
    fwrite(&header->XPPM, sizeof(int), 1, file);
    fwrite(&header->YPPM, sizeof(int), 1, file);
    fwrite(&header->CCT, sizeof(int), 1, file);
    fwrite(&header->ICC, sizeof(int), 1, file);
}

/**
 * make BMP header based on width and height.
 * The purpose of this is to create a new BMPHeader struct using the information
 * from a PPMHeader when converting from PPM to BMP.
 *
 * @param  header: Pointer to the destination DIB header
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void makeBMPHeader(struct BMP_Header* header, int width, int height){

    header->signature[0] = 'B';
    header->signature[1] = 'M';
    header->reserved_1 = 0;
    header->reserved_2 = 0;
    header->offset_pixel_array = sizeof(struct BMP_Header) + sizeof(struct DIB_Header) - 2;
    header->size = header->offset_pixel_array + width * height * 24 / 8;
}


/**
* Makes new DIB header based on width and height. Useful for converting files from PPM to BMP.
*
* @param  header: Pointer to the destination DIB header
* @param  width: Width of the image that this header is for
* @param  height: Height of the image that this header is for
*/
void makeDIBHeader(struct DIB_Header* header, int width, int height){

    header->DIBHeaderSize = sizeof(struct DIB_Header);
    header->height = height;
    header->width = width;
    header->size = width * height * 24 / 8;
    header->Planes = 1;
    header->BPP = 24;
    header->CCT = 0;
    header->Compression = 0;
    header->ICC = 0;
    header->XPPM = 3780;
    header->YPPM = 3780;
}


/**
 * read Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read
 * @param  pArr: Pixel array to store the pixels being read
 * @param  width: Width of the pixel array of this image
 * @param  height: Height of the pixel array of this image
 */
void readPixelsBMP(FILE* file, struct Pixel** pArr, int width, int height){

    for(int i = 0; i < height; i++){

        for(int j = 0; j < width; j++){

            fread(&pArr[i][j].red, sizeof(char), 1, file);
            fread(&pArr[i][j].blue, sizeof(char), 1, file);
            fread(&pArr[i][j].green, sizeof(char), 1, file);
        }
    }
}


/**
 * write Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image to write to the file
 * @param  width: Width of the pixel array of this image
 * @param  height: Height of the pixel array of this image
 */
void writePixelsBMP(FILE* file, struct Pixel** pArr, int width, int height){

    for(int i = 0; i < height; i++){

        for(int j = 0; j < width; j++){

            fwrite(&pArr[i][j].red, sizeof(char), 1, file);
            fwrite(&pArr[i][j].blue, sizeof(char), 1, file);
            fwrite(&pArr[i][j].green, sizeof(char), 1, file);
        }
    }
}

