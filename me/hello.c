#include <stdio.h>
#include <cs50.h>

/*
Prompts for name and prints "Hello, {name}"
*/

int main(void) {
    char *ans = get_string("What is your name?\n");
    printf("Hello, %s\n", ans);
}
