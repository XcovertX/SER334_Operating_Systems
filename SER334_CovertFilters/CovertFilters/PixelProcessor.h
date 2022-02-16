/**
* A utility functions for doing pixel transformations.
*
* Completion time: -1 minutes
*
* @author Vatrcia Edgar
* @version 1.0
*/

#ifndef PixelProcessor_H
#define PixelProcessor_H 1
struct Pixel{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
    unsigned char token;
};

//NOT NEEDED FOR THREADING HW.
void colorShiftPixels(struct Pixel** pArr, int width, int height, int rShift, int gShift, int bShift);
#endif