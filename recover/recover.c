#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define blockSize 512

typedef uint8_t  BYTE;

BYTE isJPG(BYTE sig[4]);

/*
A program that recovers images in JPEG format
after deletion and creating new .jpg files
*/

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: ./recover FILE.raw\n");
        return 1;
    }

    BYTE buffer[blockSize];
    FILE *card = fopen(argv[1], "r");
    FILE *jpg;
    int count = 0;
    char fileName[8];

    while(fread(buffer, blockSize, 1, card) == 1) {
        BYTE temp[4] = {
            buffer[0], buffer[1],
            buffer[2], buffer[3]};

        if (isJPG(temp)) {
            // create new jpg
            if (count > 0)
                fclose(jpg);

            sprintf(fileName, "%03i.jpg", count);
            jpg = fopen(fileName, "w");
            count++;
        }
        if (count > 0)
            fwrite(buffer, blockSize, 1, jpg);
    }
    fclose(card);
    fclose(jpg);
}

BYTE isJPG(BYTE sig[4]) {
    if (sig[0] != 0xff)
        return 0;
    if (sig[1] != 0xd8)
        return 0;
    if (sig[2] != 0xff)
        return 0;
    if ((sig[3] & 0xf0) != 0xe0)
        return 0;

    return 1;
}
