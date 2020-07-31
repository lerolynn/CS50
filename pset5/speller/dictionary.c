// Implements a dictionary's functionality
// 2019 PS answers
#include <ctype.h> // tolower
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> // free
#include <string.h> // strcpy

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
// Type node consists of space for a word and pointer to the next node
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Hashtable with 26 buckets (A - Z)
node *hashtable[N];

// Hash function
// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

unsigned long word_count = 0;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize all elements in hash table of size 26 to NULL
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    // Check if dictionary exists
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word LENGTH = 45 -> maximum length of a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {

        // Convert all words in dictionary to lower case
        char load_word[strlen(word) + 1];
        strcpy(load_word, word);
        // Convert word to lowercase
        for (char *p = load_word; *p ; ++p)
        {
            *p = tolower(*p);
        }

        // Hash the word
        int hash_value = hash(word);

        // Malloc a node for it
        node *new_node = malloc(sizeof(node));
        //check that pointer to node does not return NULL
        if (new_node == NULL)
        {
            unload();
            return false;
        }
        // Put copy word into node
        strcpy(new_node->word, word);
        // Point new node to first element of linked list
        new_node->next = hashtable[hash_value];
        // Point head to new node
        hashtable[hash_value] = new_node;
        word_count++;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char check_word[strlen(word) + 1];
    strcpy(check_word, word);
    // Convert word to lowercase
    for (char *p = check_word; *p ; ++p)
    {
        *p = tolower(*p);
    }
    // Hash the word
    int hash_value = hash(check_word);
    node *hash_ptr = hashtable[hash_value];

    // Loop through linked list untill end of linked list
    while (hash_ptr != NULL)
    {
        if (strcmp(check_word, hash_ptr->word) == 0)
        {
            return true;
        }
        hash_ptr = hash_ptr->next;
    }
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // loop through hash table
    for (int i = 0; i < N; i++)
    {
        node *hash_ptr = hashtable[i];
        // Loop through each node in hash table
        while (hash_ptr != NULL)
        {
            node *word_ptr = hash_ptr;
            hash_ptr = hash_ptr->next;
            free(word_ptr);
        }
    }
    return true;
}
