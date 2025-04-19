#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>

/*
Program that uses Coleman-Liau index to output reading level of
any given text by user.
*/

void updateReadabilityInfo(char *text, double *l, double *s);
int getReadabilityGradeLevel(double l, double s);
int isLetter(char c);
int isSentence(char c);

int main(void) {
    char *text = get_string("Text: ");
    double l;
    double s;

    updateReadabilityInfo(text, &l, &s);
    int index = getReadabilityGradeLevel(l, s);

    if (index <= 0)
        printf("Before Grade 1\n");
    else if (index >= 16)
        printf("Grade 16+\n");
    else
        printf("Grade %d\n", index);
}

// Updates L and S - letters per word and sentences pers word
void updateReadabilityInfo(char *text, double *l, double *s) {
    double letters = 0, sentences = 0, words = 0;
    int size = (int)strlen(text);
    int newWord = 1;

    for (int i = 0; i < size; i++) {
        if (isLetter(text[i]))
            letters++;
        if (isSentence(text[i]))
            sentences++;
        if (newWord == 1 && isLetter(text[i])) {
            words++;
            newWord = 0;
        }
        if (newWord == 0 && text[i] == ' ')
            newWord = 1;
    }

    *l = (letters / words * 100);
    *s = (sentences / words * 100.0);
}

// calculates Grade Level index using Coleman-Liau index
int getReadabilityGradeLevel(double l, double s) {
    return (int)round(0.0588 * l - 0.296 * s - 15.8);
}

// Identifies of char is alphabetic
int isLetter(char c) {
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) ? 1 : 0;
}

// Identifies if char signal end of sentence
int isSentence(char c) {
    return ((c == '.' || c == '!' || c == '?')) ? 1 : 0;
}
