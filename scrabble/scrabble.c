#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
Greats a game a scrable where scores are weighted
between two players and prints winner
*/

// List of points assigned for A-Z or a-z
int POINTS[] = {1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10};

int calculatePoints(char* word);

int main(void) {
    char* p1Word = get_string("Player 1: ");
    char* p2Word = get_string("Player 2: ");

    int p1Score = calculatePoints(p1Word);
    int p2Score = calculatePoints(p2Word);

    printf("Score1: %d\n", p1Score);
    printf("Score2: %d\n", p2Score);

    if(p1Score > p2Score)
        printf("Player 1 wins!");
    else if (p1Score < p2Score)
        printf("Player 2 wins!");
    else
        printf("Tie!");
}

// calculates points given a string
int calculatePoints(char* word) {
    char c;
    int score = 0;

    for (int i = 0; i < strlen(word); i++) {
        c = word[i];
        if (isupper(c) && c >= 'A' && c <= 'Z')
            score += POINTS[c - 'A'];
        else if (islower(c) && c >= 'a' && c <= 'z')
            score += POINTS[c - 'a'];

        printf("%d\n", score);
    }

    return score;
}


