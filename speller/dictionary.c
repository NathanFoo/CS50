// Implements a dictionary's functionality
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <strings.h>

#include "dictionary.h"


// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Choose number of buckets in hash table
const unsigned int N = 1000;

static int count = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int h = hash(word);
    node *temp = table[h];

    while(temp != NULL) {
        int i = 0;
        if (strcasecmp(word, temp->word) == 0) {
            return true;
        }
        temp = temp->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // used djb2 hash method by Dan Bernstein
    unsigned hash = 5381;
    int c;

    while ((c = toupper(*word++)))
        hash = ((hash << 5) + hash) + c;

    return (hash % N);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *dic = fopen(dictionary, "r");
    if (dic == NULL)
        return false;

    // Input into hashtable
    char buf[LENGTH + 1];
    while (fscanf(dic, "%s", buf) == 1) {
        insertIntoHash(buf);
        count++;
    }
    fclose(dic);

    return true;
}

// Inserts str into hashtable
void insertIntoHash(const char* word) {
    unsigned h = hash(word);
    node *newNode = malloc(sizeof(node));

    int i = 0;
    while (word[i] != '\0') {
        newNode->word[i] = word[i];
        i++;
    }
    newNode->word[i] = '\0';

    if (table[h] == NULL) {
        table[h] = newNode;
        table[h]->next = NULL;
    }
    else {
        node *temp = table[h];

        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->next = NULL;
    }
    return;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++) {
        node *temp = table[i];

        if (temp == NULL)
            continue;

        while(temp->next != NULL) {
            node *prev = temp;
            temp = temp->next;
            free(prev);
            prev = NULL;
        }
        free(temp);
        temp = NULL;
    }
    return true;
}
