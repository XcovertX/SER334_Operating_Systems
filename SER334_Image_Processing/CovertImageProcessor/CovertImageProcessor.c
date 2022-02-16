#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Image.h"
#include "BMPHandler.h"

int imageWidth;
int imageHeight;
Image *img;

int main(int arg_count, char** args) {

    // desired input and output file types
    int bmp = 1;
    int inputFileType = bmp;
    int outputFileType = bmp;

    // input file name
    char* inputName = "";

    // output file name
    char* outputName = "";

    // desired color shifts
    int rShift = 0;
    int gShift = 0;
    int bShift = 0;
    float scale  = 0.0;
    bool grayScale = false;

    for(int i = 0; i < arg_count; i++) {

        if(strcmp(args[i], "-i") == 0) {

            inputName = args[i+1];
            printf("inputName is: %s\n", inputName);

        } else if(strcmp(args[i], "-o") == 0) {

            outputName = args[i + 1];
            printf("outputName is: %s\n", outputName);

        } else if(strcmp(args[i], "-w") == 0) {

            grayScale = true;

        } else if(strcmp(args[i], "-r") == 0){

            rShift = atoi(args[i+1]);
            printf("Red will be shifted by %d.\n", rShift);

        } else if(strcmp(args[i], "-g") == 0){

            gShift = atoi(args[i+1]);
            printf("Green will be shifted by %d.\n", gShift);

        } else if(strcmp(args[i], "-b") == 0){

                bShift = atoi(args[i+1]);
                printf("Blue will be shifted by %d.\n", bShift);

        } else if(strcmp(args[i], "-s") == 0) {

                scale = atoi(args[i+1]);
                printf("Image will be scaled by %f.\n", scale);
        }
    }

    //read file
    FILE* readfile = fopen(inputName, "rb");

    struct BMP_Header* bmp_header;
    bmp_header = (struct BMP_Header*)malloc(sizeof(struct BMP_Header));

    struct DIB_Header* dib_header;
    dib_header = (struct DIB_Header*)malloc(sizeof(struct DIB_Header));

    struct Pixel** pArr;

    if (inputFileType == bmp) {

        readBMPHeader(readfile, bmp_header);
        readDIBHeader(readfile, dib_header);

        pArr = (struct Pixel**)malloc(sizeof(struct Pixel*) * dib_header->height);
        for (int i = 0; i < dib_header->height; i++) {
            pArr[i] = (struct Pixel*)malloc(sizeof(struct Pixel) * dib_header->width);
        }

        readPixelsBMP(readfile, pArr, dib_header->width, dib_header->height);

        imageHeight = dib_header->height;
        imageWidth = dib_header->width;
        img = image_create(pArr, imageHeight, imageWidth);

    }

    // apply image modifications /////////////////////////////////////////
    if(grayScale) {
        image_apply_bw(img);
    }

    if(rShift != 0 || gShift != 0 || bShift != 0) {
        image_apply_colorshift(img, gShift, rShift, bShift);
    }

    if(scale > 1 || scale < 1){
        image_apply_resize(img, scale);
        pArr = img->pArr;
        dib_header->size = img->width * img->height;
        dib_header->height = img->height;
        dib_header->width = img->width;
        imageHeight = img->height;
        imageWidth = img->width;
    }

    // Write new file ////////////////////////////////////////////////////
    if(outputFileType == bmp){

        if(strcmp(outputName, "") == 0){
            // input file name w/o file type
            int tempNameLength = strlen(inputName);
            char* tempName = inputName;
            tempName[tempNameLength - 4] = '\0';
            // output file name
            outputName = strcat(tempName, "_copy.bmp");
        }
        //write file
        FILE* writefile = fopen(outputName, "wb");
        writeBMPHeader(writefile, bmp_header);
        writeDIBHeader(writefile, dib_header);
        writePixelsBMP(writefile, img->pArr, dib_header->width, dib_header->height);

        fclose(writefile);

        for (int i = 0; i < dib_header->height; i++) {
            free(pArr[i]);
        }

        free(pArr);
        free(dib_header);
        free(bmp_header);

        printf("Completed BMP file build.\n");
    }
    return 0;
}
