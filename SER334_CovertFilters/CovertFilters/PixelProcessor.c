/**
* A utility functions for doing pixel transformations.
*
* Completion time: 15h
*
* @author James Covert
* @version 1.0
*/

#include "PixelProcessor.h"

//NOT NEEDED FOR THREADING HW.
void colorShiftPixels(struct Pixel** pArr, int width, int height, int rShift, int gShift, int bShift){

    for(int i = 0; i < height; i++) {

        for (int j = 0; j < width; j++) {

            if (pArr[i][j].red + rShift > 255) {

                pArr[i][j].red = 255;

            } else if (pArr[i][j].red + rShift < 0) {

                pArr[i][j].red = 0;

            } else {

                pArr[i][j].red += rShift;
            }

            if (pArr[i][j].green + gShift > 255) {

                pArr[i][j].green = 255;

            } else if (pArr[i][j].green + gShift < 0) {

                pArr[i][j].green = 0;

            } else {

                pArr[i][j].green += gShift;
            }

            if (pArr[i][j].blue + bShift > 255) {

                pArr[i][j].blue = 255;

            } else if (pArr[i][j].blue + bShift < 0) {

                pArr[i][j].blue = 0;

            } else {

                pArr[i][j].blue += bShift;
            }
        }
    }
}


