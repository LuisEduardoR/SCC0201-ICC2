#include "dictionary.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Node of the prefix tree.
typedef struct node {
  char cur;
  int size;
  Node** child;
}Node;

// Insert a new character in the tree.
int insert_in_dict(Node* target, char* word, int pos) {

    // If the end of the string is found it means all nodes where created succesfully.
    if(target->cur == '\0') return 0;

    // Converts current character to lower case.
    char c = word[pos];
    if(c >= 65 && c <= 90)
        c+=32;

    // Looks if the cuurent char is already on the tree, and if it is, proceeds to the next one.
    for(int i = 0; i < target->size; i++) {

        if(target->child[i]->cur == c)
            return insert_in_dict(target->child[i], word, pos + 1);
    }

    // Add the current char to tree and proceeds to the next.

    // Allocates the necessary memory.
    target->size ++;
    target->child = (Node**)realloc(target->child, target->size * sizeof(Node*));
    if(target->child == NULL) {
        fprintf(stderr,"Error allocating memory to expand current node child list!\n");
        return 1;
    }

    target->child[target->size - 1] = (Node*)malloc(sizeof(Node));
    if(target->child == NULL) {
        fprintf(stderr,"Error allocating for new node!\n");
        return 1;
    }

    // Sets the node of the new char.
    target->child[target->size - 1]->size = 0;
    target->child[target->size - 1]->child = NULL;
    target->child[target->size - 1]->cur = c;

    // Insert the next one.
    insert_in_dict(target->child[target->size - 1], word, pos + 1);

}

// Creates the temp_dictionary from reading words out of a file.
Dictionary create_dictionary(FILE* file) {

    // Allocates memory for the dictionary.
    Dictionary dictionary = NULL;
    dictionary = (Dictionary)malloc(sizeof(Node));
    if(dictionary == NULL)
        return NULL;

    // Initializes the first node of the dictionary.
    dictionary->cur = '*';
    dictionary->size = 0;
    dictionary->child = NULL;

    // Temporally stores a string. 
    char buffer[STR_BUFFER_SIZE];

    // Reads the entire file.
    while(fscanf(file, " %[a-zA-Z]", buffer) != EOF) {

        // Inset the current word in buffer on the dictionary.
        int err = insert_in_dict(dictionary, buffer, 0);

        // Returns an error if something bad occurs.
        if(err != 0)
            return NULL;

        // Ignores invalid characters.
        fscanf(file, " %[^a-zA-Z]", buffer);
    }

    return dictionary;

}

// Uses recursion to free all the tree nodes.
void recursive_free(Node* target) {

    // Free the child nodes.
    for(int i = 0; i < target->size; i++)
        recursive_free(target->child[i]);
    // Free the current node.
    free(target->child);
    free(target);

}

// Frees the dictionary's memory.
void free_dictionary(Dictionary* target) {

    recursive_free(*target);
    *target = NULL;

}

// Recursively searches a word in the prefix tree.
int search_in_dict(Dictionary dictionary, char* word, int pos) {

    // If the end of the string is found it means there is a match.
    if(dictionary->cur == '\0') return 1;

    // Converts current character to lower case.
    char c = word[pos];
    if(c >= 65 && c <= 90)
        c+=32;

    // If the word hasn't ended, looks for the next char in the child of the current node.
    for(int i = 0; i < dictionary->size; i++) {

        if(dictionary->child[i]->cur == c) {
            return search_in_dict(dictionary->child[i], word, pos + 1);
        }
    }

    // If the char isn't on any of the child nodes it means it doesn't exist in the tree.
    return 0;

}

// Wrapper for the search function.
int find_in_dict(Dictionary dictionary, char* word) { return search_in_dict(dictionary, word, 0); }