#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/*
A program that encrypts alphabetic text using substitution
given a command line arguement of at least 26 chars, all alphabetic
and, no char copies.
*/

int validCommandLineArg(int argc, char* argv);
int isKeyValid(char* arg);
char* encryptText(char* text, char* arg);
int isAlphabet(char c);


int main(int argc, char* argv[]) {
    printf("%d\n", argc);

    if (!validCommandLineArg(argc, argv[1])) {
        return 1;
    }

    char* planeText = get_string("plaintext: ");
    char* cypherText = encryptText(planeText, argv[1]);

    printf("ciphertext: %s\n", cypherText);
    return 0;
}


int validCommandLineArg(int argc, char* arg) {
    int errors = 0;

    if (argc < 2) {
        printf("Program cannot run without a command line arguement.\n");
        errors++;
    }
    else if (argc > 2) {
        printf("Program can only run with one command line arguement.\n");
        errors++;
    }
    else if (!isKeyValid(arg))
        errors++;

    if (errors > 0) {
        return 0;
    }
    else
        return 1;
}

int isKeyValid(char* arg) {
    int checkList[26] = {0};
    int count = 0;
    for (int i = 0; i < strlen(arg); i++) {
        if (!isAlphabet(arg[i])) {
            printf("Error non-alphabetic character.\n");
            return 0;
        }
        if (isupper(arg[i])) {
            if (checkList[arg[i] - 'A'] == 0)
                checkList[arg[i] - 'A'] = 1;
            else {
                printf("Error repeated characters.\n");
                return 0;
            }
        }
        else if (islower(arg[i])) {
            if (checkList[arg[i] - 'a'] == 0)
                checkList[arg[i] - 'a'] = 1;
            else {
                printf("Error repeated characters.\n");
                return 0;
            }
        }
        count++;
    }
    if (count == 26)
        return 1;
    else {
        printf("Command line arg must have 26 chars as arguement.\n");
        return 0;
    }
}

char* encryptText(char* text, char* key) {
    int len = strlen(text);
    char *cipher = malloc(sizeof(char) * (len));
    for (int i = 0; i < len; i++) {
        if (isAlphabet(text[i])) {
            if (isupper(text[i]) && isupper(key[text[i] - 'A']))
                cipher[i] = key[text[i] - 'A'];
            else if (isupper(text[i]) && islower(key[text[i] - 'A']))
                cipher[i] = (key[text[i] - 'A'] - 32);
            else if (islower(text[i]) && isupper(key[text[i] - 'a']))
                cipher[i] = (key[text[i] - 'a'] + 32);
            else if (islower(text[i]) && islower(key[text[i] - 'a']))
                cipher[i] = key[text[i] - 'a'];
        }
        else {
            cipher[i] = text[i];
        }
    }
    return cipher;

}

int isAlphabet(char c) {
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) ? 1 : 0;
}
