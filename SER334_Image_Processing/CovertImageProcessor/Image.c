/**
* PixelProcessor source - shifts the color of the image specified by the user
*
* Completion time: 15h
*
* @author James Covert
* @version 1.0
*/

#include "Image.h"

/* Creates a new image and returns it.
*
 * @param  pArr: Pixel array of this image.
 * @param  width: Width of this image.
 * @param  height: Height of this image.
 * @return A pointer to a new image.
*/
Image* image_create(struct Pixel** pArr, int width, int height){
    struct Image *img;
    img = malloc(sizeof(Image));
    img->pArr = pArr;
    img->width = width;
    img->height = height;
    return img;
}

/* Converts the image to grayscale.
*
 * @param  img: the image.
*/
void image_apply_bw(Image* img){

    for(int i = 0; i < img->height; i++){

        for(int j = 0; j < img->width; j++){

            float grayscale = (0.299 * img->pArr[i][j].red) +
                              (0.587 * img->pArr[i][j].green) +
                              (0.114 * img->pArr[i][j].blue);
            img->pArr[i][j].red = grayscale;
            img->pArr[i][j].green = grayscale;
            img->pArr[i][j].blue = grayscale;
        }
    }
}

/* Destroys an image. Does not deallocate internal pixel array.
*
 * @param  img: the image to destroy.
*/
void image_destroy(Image** img){
    free(img);
}

/* Returns a double pointer to the pixel array.
*
 * @param  img: the image.
*/
struct Pixel** image_get_pixels(Image* img){
    return img->pArr;
}

/* Returns the width of the image.
*
 * @param  img: the image.
*/
int image_get_width(Image* img){
    return img->width;
}

/* Returns the height of the image.
*
 * @param  img: the image.
*/
int image_get_height(Image* img){
    return img->height;
}

void image_apply_colorshift(struct Image* img, int rShift, int gShift, int bShift){

    for(int i = 0; i < img->height; i++){

        for(int j = 0; j < img->width; j++){

            if (img->pArr[i][j].red + rShift > 255){

                img->pArr[i][j].red = 255;

            } else if (img->pArr[i][j].red + rShift < 0){

                img->pArr[i][j].red = 0;

            } else {

                img->pArr[i][j].red += rShift;
            }

            if (img->pArr[i][j].green + gShift > 255){

                img->pArr[i][j].green = 255;

            } else if (img->pArr[i][j].green + gShift < 0){

            img->pArr[i][j].green = 0;

            } else {

                img->pArr[i][j].green += gShift;
            }

            if (img->pArr[i][j].blue + bShift > 255) {

                img->pArr[i][j].blue = 255;

            } else if (img->pArr[i][j].blue + bShift < 0) {

                img->pArr[i][j].blue = 0;

            } else {

                img->pArr[i][j].blue += bShift;
            }
        }
    }
}

/* Scales the image. If the scaling factor is less than 1 the new image will be
 * smaller, if it is larger than 1, the new image will be larger.
 *
 * @param  img: the image.
 * @param  factor: the scaling factor
*/
void image_apply_resize(Image* img, float factor){

    struct Pixel** newPArr = (struct Pixel**)malloc(sizeof(struct Pixel*) * img->height * factor);
    for (int i = 0; i < img->height; i++) {
        newPArr[i] = (struct Pixel*)malloc(sizeof(struct Pixel) * img->width);
    }
    int newWidth = img->width * factor;
    int newHeight = img->height * factor;
    Image* newImg = image_create(newPArr, newWidth, newHeight);

    for(int i = 0; i < newHeight; i++){

        for(int j = 0; j < newWidth; j++){

            for(int k = 0; k < factor; k++) {
                newImg->pArr[i][j+k].red = img->pArr[i][j].red;
                newImg->pArr[i][j+k].green = img->pArr[i][j].green;
                newImg->pArr[i][j+k].blue = img->pArr[i][j].blue;
            }
        }
    }
    struct Image* temp = img;
    img = newImg;
//    image_destroy(temp);
}

