#include <math.h>
#include <stdio.h>
#include "helpers.h"

/*
    subset of programs that will alter images to a filter
    type of grayscale, horizontal reflect, blur, or edges
*/



void RGBcpy(int height, int width, RGBTRIPLE dest[height][width], RGBTRIPLE src[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            dest[i][j] = src[i][j];
        }
    }
    return;
}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double avr = 0;
            avr += image[i][j].rgbtBlue;
            avr += image[i][j].rgbtGreen;
            avr += image[i][j].rgbtRed;
            avr /= 3.0;
            avr = round(avr);
            image[i][j].rgbtBlue = avr;
            image[i][j].rgbtGreen = avr;
            image[i][j].rgbtRed = avr;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width / 2; j++) {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][(width - 1) - j];
            image[i][(width - 1) - j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double avrB = 0, avrG = 0, avrR = 0;
            int count = 0;

            for (int l = (i - 1); l < (i + 2); l++) {
                for (int k = (j - 1); k < (j + 2); k++) {
                    if (l < 0 || l >= height) {
                        continue;
                    }
                    if (k < 0 || k >= width) {
                        continue;
                    }
                    printf("(%d, %d) ", l, k);

                    avrB += image[l][k].rgbtBlue;
                    avrG += image[l][k].rgbtGreen;
                    avrR += image[l][k].rgbtRed;
                    count++;
                }
            }

            avrB /= count;
            avrG /= count;
            avrR /= count;
            temp[i][j].rgbtBlue = round(avrB);
            temp[i][j].rgbtGreen = round(avrG);
            temp[i][j].rgbtRed = round(avrR);
        }
    }
    RGBcpy(height, width, image, temp);
    return;
}



// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    int gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}};
    int gy[3][3] = {
        {-1, -2, -1},
        {0, 0, 0,},
        {1, 2, 1}};
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double gxBlue = 0, gyBlue = 0;
            double gxGreen = 0, gyGreen = 0;
            double gxRed = 0, gyRed = 0;

            for (int l = (i - 1); l < (i + 2); l++) {
                for (int k = (j - 1); k < (j + 2); k++) {
                    if (l < 0 || l >= height)
                        continue;
                    if (k < 0 || k >= width)
                        continue;

                    gxBlue += image[l][k].rgbtBlue * gx[l - i + 1][k - j + 1];
                    gxGreen += image[l][k].rgbtGreen * gx[l - i + 1][k - j + 1];
                    gxRed += image[l][k].rgbtRed * gx[l - i + 1][k - j + 1];
                    gyBlue += image[l][k].rgbtBlue * gy[l - i + 1][k - j + 1];
                    gyGreen += image[l][k].rgbtGreen * gy[l - i + 1][k - j + 1];
                    gyRed += image[l][k].rgbtRed * gy[l - i + 1][k - j + 1];
                }
            }
            int gBlue = round(sqrt(pow(gxBlue, 2.0) + pow(gyBlue, 2.0)));
            int gGreen = round(sqrt(pow(gxGreen, 2.0) + pow(gyGreen, 2.0)));
            int gRed = round(sqrt(pow(gxRed, 2.0) + pow(gyRed, 2.0)));

            temp[i][j].rgbtBlue = (gBlue > 255) ? 255 : gBlue;
            temp[i][j].rgbtGreen = (gGreen > 255) ? 255 : gGreen;
            temp[i][j].rgbtRed = (gRed > 255) ? 255 : gRed;
        }
    }
    RGBcpy(height, width, image, temp);
    return;
}
