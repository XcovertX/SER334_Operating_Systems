/**
* CovertFilters takes a BMP image and applies one of two filters (cheese or blur) depending on user input.
* It does this using multiple threads.
*
*
* Completion time: 15hrs
*
* @author James Covert
* @version 1.0
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "BmpProcessor.h"

////////////////////////////////////////////////////////////////////////////////
//MACRO DEFINITIONS

//problem assumptions
#define BMP_HEADER_SIZE 14
#define BMP_DIB_HEADER_SIZE 40
#define MAXIMUM_IMAGE_SIZE 4096
#define THREAD_COUNT 4

////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES

int imageWidth;
int imageHeight;

void cheese(struct Pixel** pArr) {

    colorShiftPixels(pArr, imageWidth, imageHeight, 0, 60, 60);

    for (int i = 0; i < imageHeight; i++) {
        for (int j = 0; j < imageWidth; j++) {
            if (pArr[i][j].token == 1) {
                pArr[i][j].blue = 0;
                pArr[i][j].green = 0;
                pArr[i][j].red = 0;
            }
        }
    }
}

void holes(struct Pixel** pArr, int width, int height) {
    int average_radius = 0;
    int length = 0;
    if (width < height) {
        average_radius = width * .08;
        length = width;
    }
    else {
        average_radius = height * .08;
        length = height;
    }

    srand(time(NULL));

    int medium_hole_count = average_radius / 2;
    int medium_radius = average_radius;
    int small_hole_count = (average_radius - medium_hole_count) / 2;
    int small_radius = average_radius / 3;
    int large_radius = average_radius * 2;

    printf("%d", medium_radius);

    int** center = (int**)malloc(sizeof(int*) * average_radius);
    for (int i = 0; i < average_radius; i++) {
        center[i] = (int*)malloc(sizeof(int) * 2);
    }

    for (int i = 0; i < average_radius; i++) {
        center[i][0] = rand() % width;
        center[i][1] = rand() % height;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < small_hole_count; k++) {
                if (sqrt((i - center[k][1]) *
                         (i - center[k][1]) +
                         (j - center[k][0]) *
                         (j - center[k][0])) < small_radius) {
                    pArr[i][j].token = 1;
                    break;
                }
            }

            for (int k = small_hole_count; k < small_hole_count + medium_hole_count; k++) {
                if (sqrt((i - center[k][1]) *
                         (i - center[k][1]) +
                         (j - center[k][0]) *
                         (j - center[k][0])) < medium_radius) {
                    pArr[i][j].token = 1;
                    break;
                }
            }

            for (int k = small_hole_count + medium_hole_count; k < average_radius; k++) {
                if (sqrt((i - center[k][1]) *
                         (i - center[k][1]) +
                         (j - center[k][0]) *
                         (j - center[k][0])) < large_radius) {
                    pArr[i][j].token = 1;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < average_radius; i++) {
        free(center[i]);
    }
    free(center);
}

void boxblur(void* arg) {
    struct Pixel** pArr = arg;

    int startCol = 0;
    int endCol = imageWidth;

    for (int i = 0; i < imageHeight; i++) {
        for (int j = startCol; j < endCol; j++) {

            int p1_r = 0, p1_g = 0, p1_b = 0;
            int p2_r = 0, p2_g = 0, p2_b = 0;
            int p3_r = 0, p3_g = 0, p3_b = 0;
            int p4_r = 0, p4_g = 0, p4_b = 0;
            int p5_r = 0, p5_g = 0, p5_b = 0;
            int p6_r = 0, p6_g = 0, p6_b = 0;
            int p7_r = 0, p7_g = 0, p7_b = 0;
            int p8_r = 0, p8_g = 0, p8_b = 0;
            int count = 1;

            if (i - 1 >= 0 && j - 1 >= 0) {
                p1_r = pArr[i - 1][j - 1].red;
                p1_g = pArr[i - 1][j - 1].green;
                p1_b = pArr[i - 1][j - 1].blue;
                count++;
            }
            if (i - 1 >= 0 && j >= 0) {
                p2_r = pArr[i - 1][j].red;
                p2_g = pArr[i - 1][j].green;
                p2_b = pArr[i - 1][j].blue;
                count++;
            }
            if (i - 1 >= 0 && j + 1 < imageWidth) {
                p3_r = pArr[i - 1][j + 1].red;
                p3_g = pArr[i - 1][j + 1].green;
                p3_b = pArr[i - 1][j + 1].blue;
                count++;
            }
            if (i >= 0 && j - 1 >= 0) {
                p4_r = pArr[i][j - 1].red;
                p4_g = pArr[i][j - 1].green;
                p4_b = pArr[i][j - 1].blue;
                count++;
            }
            if (i >= 0 && j + 1 < imageWidth) {
                p5_r = pArr[i][j + 1].red;
                p5_g = pArr[i][j + 1].green;
                p5_b = pArr[i][j + 1].blue;
                count++;
            }
            if (i +1 <imageHeight && j - 1 >= 0) {
                p6_r = pArr[i + 1][j - 1].red;
                p6_g = pArr[i + 1][j - 1].green;
                p6_b = pArr[i + 1][j - 1].blue;
                count++;
            }
            if (i + 1 <imageHeight && j >= 0) {
                p7_r = pArr[i + 1][j].red;
                p7_g = pArr[i + 1][j].green;
                p7_b = pArr[i + 1][j].blue;
                count++;
            }
            if (i + 1 < imageHeight && j + 1 < imageWidth) {
                p8_r = pArr[i + 1][j + 1].red;
                p8_g = pArr[i + 1][j + 1].green;
                p8_b = pArr[i + 1][j + 1].blue;
                count++;
            }

            pArr[i][j].red = (p1_r + p2_r + p3_r + p4_r + p5_r + p6_r + p7_r + p8_r + pArr[i][j].red ) / count;
            pArr[i][j].green = (p1_g + p2_g + p3_g + p4_g + p5_g + p6_g + p7_g + p8_g + pArr[i][j].green) / count;
            pArr[i][j].blue = (p1_b + p2_b + p3_b + p4_b + p5_b + p6_b + p7_b + p8_b + pArr[i][j].blue) / count;

        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//MAIN PROGRAM CODE

void main(int argc, char* argv[]) {

    int type = 0;
    char* inputName = "";
    char* outputName = "";

    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], "-i") == 0)
            inputName = argv[i+1];
        else if(strcmp(argv[i], "-o") == 0)
            outputName = argv[i+1];
        else if(strcmp(argv[i], "b") == 0)
            type = 1;
    }

    //read file
    FILE* readfile = fopen(inputName, "rb");

    struct BMP_Header* bmp_header;
    bmp_header = (struct BMP_Header*)malloc(sizeof(struct BMP_Header));
    struct DIB_Header* dib_header;
    dib_header = (struct DIB_Header*)malloc(sizeof(struct DIB_Header));

    readBMPHeader(readfile, bmp_header);
    readDIBHeader(readfile, dib_header);

    struct Pixel** pArr = (struct Pixel**)malloc(sizeof(struct Pixel*) * dib_header->height);
    for (int i = 0; i < dib_header->height; i++) {
        pArr[i] = (struct Pixel*)malloc(sizeof(struct Pixel) * dib_header->width);
    }

    readPixelsBMP(readfile, pArr, dib_header->width, dib_header->height);

    pthread_t threads[THREAD_COUNT];
    int threads_result[THREAD_COUNT];

    //divide pArr
    int division_width = dib_header->width / THREAD_COUNT;
    int remainder = dib_header->width % THREAD_COUNT;

    imageHeight = dib_header->height;
    imageWidth = division_width + remainder;

    //boxblur filter
    if(type == 1){
        struct Pixel*** divide_pArr = (struct Pixel***)malloc(sizeof(struct Pixel**) * THREAD_COUNT);
        for (int i = 0; i < THREAD_COUNT; i++) {
            divide_pArr[i] = (struct Pixel**)malloc(sizeof(struct Pixel*) * dib_header->height);
            for (int j = 0; j < dib_header->height; j++) {
                divide_pArr[i][j] = (struct Pixel*)malloc(sizeof(struct Pixel) * (division_width + remainder + 1));
            }
        }

        for (int i = 0; i < THREAD_COUNT; i++) {
            if (i == 0) {
                for (int j = 0; j < dib_header->height; j++) {
                    for (int k = 0; k < division_width + 1; k++) {
                        divide_pArr[i][j][k] = pArr[j][k];
                    }
                }
            }
            else if (i == THREAD_COUNT - 1) {
                for (int j = 0; j < dib_header->height; j++) {
                    for (int k = 0; k < division_width + 1 + remainder; k++) {
                        divide_pArr[i][j][k] = pArr[j][k + i * division_width - 1];
                    }
                }
            }
            else {
                for (int j = 0; j < dib_header->height; j++) {
                    for (int k = 0; k < division_width + 2; k++) {
                        divide_pArr[i][j][k] = pArr[j][k + i * division_width - 1];
                    }
                }
            }
        }

        //create threads
        for (int i = 0; i < THREAD_COUNT; i++) {

            threads_result[i] = pthread_create(&threads[i], NULL, boxblur, divide_pArr[i]);

            if (threads_result[i] != 0) {

                printf("Thread build FAIL: thread %d\n.", i);

            } else {

                printf("Thread build SUCCESS: thread %d\n", i);
            }
        }

        //waiting threads
        void* return_value;
        for (int i = 0; i < THREAD_COUNT; i++) {
            pthread_join(threads[i], &return_value);
        }

        //reassemble the image
        for (int i = 0; i < THREAD_COUNT; i++) {
            if (i == 0) {
                for (int j = 0; j < dib_header->height; j++) {
                    for (int k = 0; k < division_width + 1; k++) {
                        pArr[j][k] = divide_pArr[i][j][k];
                    }
                }
            }
            else if (i == THREAD_COUNT - 1) {
                for (int j = 0; j < dib_header->height; j++) {
                    for (int k = 0; k < division_width + remainder; k++) {
                        pArr[j][k + i * division_width] = divide_pArr[i][j][k+1];
                    }
                }
            }
            else {
                for (int j = 0; j < dib_header->height; j++) {
                    for (int k = 0; k < division_width; k++) {
                        pArr[j][k + i * division_width] = divide_pArr[i][j][k+1];
                    }
                }
            }
        }

        //cheese filter
    } else {

        holes(pArr, dib_header->width, dib_header->height);

        struct Pixel*** divide_pArr = (struct Pixel***)malloc(sizeof(struct Pixel**) * THREAD_COUNT);
        for (int i = 0; i < THREAD_COUNT; i++) {
            divide_pArr[i] = (struct Pixel**)malloc(sizeof(struct Pixel*) * dib_header->height);
            for (int j = 0; j < dib_header->height; j++) {
                divide_pArr[i][j] = (struct Pixel*)malloc(sizeof(struct Pixel) * (division_width + remainder));
            }
        }

        for (int i = 0; i < THREAD_COUNT; i++) {
            if (i == THREAD_COUNT - 1) {
                for (int j = 0; j < dib_header->height; j++) {
                    for (int k = 0; k < division_width + remainder; k++) {
                        divide_pArr[i][j][k] = pArr[j][k + i * division_width];
                    }
                }
            }
            else {
                for (int j = 0; j < dib_header->height; j++) {
                    for (int k = 0; k < division_width; k++) {
                        divide_pArr[i][j][k] = pArr[j][k + i * division_width];
                    }
                }
            }
        }

        for (int i = 0; i < THREAD_COUNT; i++) {

            threads_result[i] = pthread_create(&threads[i], NULL, cheese, divide_pArr[i]);

            if (threads_result[i] != 0) {

                printf("Thread build FAIL: thread %d\n.", i);

            } else {

                printf("Thread build SUCCESS: thread %d\n", i);
            }
        }

        //waiting threads
        void* return_value;
        for (int i = 0; i < THREAD_COUNT; i++) {
            pthread_join(threads[i], &return_value);
        }

        for (int i = 0; i < THREAD_COUNT; i++) {
            if (i == THREAD_COUNT - 1) {
                for (int j = 0; j < dib_header->height; j++) {
                    for (int k = 0; k < division_width + remainder; k++) {
                        pArr[j][k + i * division_width] = divide_pArr[i][j][k];
                    }
                }
            }
            else {
                for (int j = 0; j < dib_header->height; j++) {
                    for (int k = 0; k < division_width; k++) {
                        pArr[j][k + i * division_width] = divide_pArr[i][j][k];
                    }
                }
            }
        }
    }

    FILE* writefile = fopen(outputName, "wb");
    writeBMPHeader(writefile, bmp_header);
    writeDIBHeader(writefile, dib_header);
    writePixelsBMP(writefile, pArr, dib_header->width, dib_header->height);
    fclose(writefile);
    for (int i = 0; i < dib_header->height; i++) {
        free(pArr[i]);
    }
    free(pArr);
    free(dib_header);
    free(bmp_header);

    printf("%s is finished!\n", outputName);
}