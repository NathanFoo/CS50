#include <cs50.h>
#include <stdio.h>

/*
Program that prints about mario like blocks at
a height based on user input
*/


int main(void) {
    int height;
    int gap = 2;
    do {
        height = get_int("Height:");
    } while(height <= 0 || 9 <= height);
    int count = 0;
    int width = (height * 2) + 2;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if ((j - (height - 1)) >= -i && j < height)  printf("#");
            else if ((j - (height + gap)) <= i && j > (height + (gap - 1))) printf("#");
            else if (j < height + gap)  printf(" "); // added conditional to stop trailing spaces
        }
    printf("\n");
    }
}
