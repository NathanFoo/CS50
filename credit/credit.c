#include <cs50.h>
#include <stdio.h>

/*
program takes a card number and identifies card type and
validity of it
*/


struct CardInfo {
    long cardNum;
    int digitCount;
    int firstTwoDigits;
    int checkSum;
    char* cardType;
};


void updateCountAndCheckSum(struct CardInfo *Card);
void updateFirstTwoDigits(struct CardInfo *Card);
void updateCardType(struct CardInfo *Card);
int isAmericanExpress(int digitCount, int firstTwoDigits);
int isMasterCard(int digitCount, int firstTwoDigits);
int isVisa(int digitCount, int firstTwoDigits);



int main(void) {
    struct CardInfo Card;
    Card.cardNum = get_long("Number: ");
    updateCountAndCheckSum(&Card);
    updateFirstTwoDigits(&Card);
    updateCardType(&Card);
    printf("%s\n", Card.cardType);
}



void updateCardType(struct CardInfo *Card) { // updates cardType based on digitCount, CheckSum, and FirstTwoDigits
    if(isAmericanExpress(Card->digitCount, Card->firstTwoDigits))   {Card->cardType = "AMEX";}
    else if (isMasterCard(Card->digitCount, Card->firstTwoDigits))  {Card->cardType = "MASTERCARD";}
    else if (isVisa(Card->digitCount, Card->firstTwoDigits))        {Card->cardType = "VISA";}
    else                                                            {Card->cardType = "INVALID";}

    if (Card->checkSum % 10 != 0)
        Card->cardType = "INVALID";
}

void updateCountAndCheckSum(struct CardInfo *Card) { // updates Count and CheckSum based on cardNum
    long tempCardNum = Card->cardNum;
    int tempNum;
    int alt = 0;
    int sum = 0;

    while (tempCardNum != 0) {
        if (alt == 0) {
            sum += tempCardNum % 10;
            alt = 1;
        }
        else {
            tempNum = 2 * (tempCardNum % 10);
            while(tempNum != 0) {
                sum += tempNum % 10;
                tempNum /= 10;
            }
            alt = 0;
        }
        tempCardNum /= 10;
        Card->digitCount++;
    }
    Card->checkSum = sum;
}

void updateFirstTwoDigits(struct CardInfo *Card) { // updates first 2 digits based on cardSum and digitCount
    long tempCardNum = Card->cardNum;
    for (int i = 0; i < Card->digitCount - 2; i++) {
        tempCardNum /= 10;
    }
    Card->firstTwoDigits = (int)tempCardNum;
}

int isAmericanExpress(int digitCount, int firstTwoDigits) { // returns true or false based on if meets American Express card requisites
    if (digitCount == 15 && (firstTwoDigits == 34 || firstTwoDigits == 37))
        return 1;
    else
        return 0;
}

int isMasterCard(int digitCount, int firstTwoDigits) { // returns true or false based on if meets MasterCard requisites
    if (digitCount == 16 && (firstTwoDigits >= 51 && firstTwoDigits <= 55))
        return 1;
    else
        return 0;
}

int isVisa(int digitCount, int firstTwoDigits) { // returns true or false based on if mees Visa card requisites
    if ((digitCount == 13 || digitCount == 16) && (firstTwoDigits >= 40 && firstTwoDigits <= 49))
        return 1;
    else
        return 0;
}
