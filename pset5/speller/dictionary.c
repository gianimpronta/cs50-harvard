/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dictionary.h"

// defining trie node
typedef struct trieNode
{
        bool is_word;
        struct trieNode *children[27];
}
trieNode;
trieNode *root;

// initializing word counter
unsigned int wordcount = 0;

// getting int values for letters 
int a_int = (int)'a';
int z_int = (int)'b';

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    trieNode *cursor = root;
    int i = 0;
    while(word[i] != '\0')
    {
        char c = word[i];
        
        if (c == '\'')
        {
            c = z_int + 1;
        }
        
        int trieIndex = tolower(c) - a_int;
        if (cursor->children[trieIndex] == NULL)
        {
            return false;
        }
        else
        {
            cursor = cursor->children[trieIndex];
            i++;
        }
    }
    if (cursor->is_word == true){
        return true;        
    }
    else
    {
        return false;
    }
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // allocating memory for root
    root = malloc(sizeof(trieNode));

    // sanity check
    if (root == NULL)
    {
        unload();
        return false;
    }
    
    // opening file
    FILE *fp = fopen(dictionary, "r");
    
    // sanity check
    if (fp == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        unload();
        return false;
    }

    // creating cursor
    trieNode *cursor = NULL;
    
    // running through every character in dictionary
    for (int c = fgetc(fp); c != EOF; c = fgetc(fp))
    {
        // pointing cursor to root node
        cursor = root;

        // iterating through each letter of word
        for (; c != '\n'; c = fgetc(fp))
        {
            // storing apostrophe
            if (c == '\'')
            {
                c = z_int + 1;
            }
            
            // defining index for legibility
            int trieIndex = c - a_int;

            // if c is not in trie, create
            if (cursor->children[trieIndex] == NULL)
            {
                cursor->children[trieIndex] = (trieNode*) malloc(sizeof(trieNode));
                cursor = cursor->children[trieIndex];
            }
            else
            {
                cursor = cursor->children[trieIndex];
            }
        }
        cursor->is_word = true;
        wordcount++;
    }
    
    fclose(fp);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return wordcount;
}

/**
 * Recursive function to unload data 
 */

void recurFreeNode(trieNode *cursor)
{

    for (int i = 0; i < 27; i++)
    {
        if (cursor->children[i] != NULL)
        {
            recurFreeNode(cursor->children[i]);
        }
    }
    free(cursor);
    return;
}
/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    recurFreeNode(root);
    return true;
}

