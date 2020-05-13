// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> // tolower
#include <string.h> //strlen

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    // Indicates if its the last character of a word
    bool is_word;
    // Array of node pointers A-Z and '\'
    // Each element in array points to another node
    struct node *children[N];
}
node;

// Represents a trie
node *root;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }

    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];
    int hash;

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        if (strlen(word) > LENGTH)
        {
            continue;
        }
        // Set pointer to the root node at the start of every word
        node *ptr = root;
        // Iterate through word
        for (int i = 0; i < strlen(word); i++)
        {
            // Find index of alphabet in node
            hash = (word[i] == '\'') ? (N - 1) : tolower(word[i]) - 'a';

            // If pointer at location of alphabet is NULL
            if (ptr->children[hash] == NULL)
            {
                // Allocate a new node to point to,
                node *new_node = malloc(sizeof(node));

                // If there is not enough space in memory
                if (new_node == NULL)
                {
                    unload();
                    return false;
                }

                new_node->is_word = false;
                // Iterate through new_node and set all values to NULL
                for (int j = 0; j < N; j++)
                {
                    new_node->children[j] = NULL;
                }
                // Set pointer at alphabet location to point to new_node
                ptr->children[hash] = new_node;

                // If at root node, need to change pointer of root node also
                if (i == 0)
                {
                    root->children[hash] = new_node;
                }
            }

            // Move ptr to position of alphabet
            ptr = ptr->children[hash];
        }

        // Set ptr to true at end of word
        ptr->is_word = true;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // If root is not initialized, dictionary is not loaded
    if (!root)
    {
        return 0;
    }

    int count = 0;

    // Initialize a pointer, point it to root node
    node *ptr = root;

    // If node is a word, add to word count
    if (ptr->is_word)
    {
        count++;
    }

    for (int i = 0; i < N; i++)
    {
        root = ptr->children[i];

        count += size();
    }
    root = ptr;
    return count;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // If root is not initialized, dictionary is not loaded
    if (!root)
    {
        return false;
    }

    // Check if word is < 45 characters
    if (strlen(word) > LENGTH)
    {
        return false;
    }

    // Initialize a pointer, point it to root node
    node *ptr = root;
    int hash;

    // For each letter in input word
    for (int i = 0; i < strlen(word); i++)
    {
        // Find index of alphabet in node
        hash = (word[i] == '\'') ? (N - 1) : tolower(word[i]) - 'a';

        // If NULL, word does not exist in dictionary
        if (!ptr->children[hash])
        {
            return false;
        }

        // Follow pointer to next node
        ptr = ptr->children[hash];
    }

    // At the end of the word input, check is_word to check if word exists
    return ptr->is_word;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    if (!root)
    {
        return false;
    }

    node *ptr = root;

    for (int i = 0; i < N; i++)
    {
        root = ptr->children[i];
        unload();
    }
    // Free memory
    free(ptr);

    root = ptr;

    return true;
}
