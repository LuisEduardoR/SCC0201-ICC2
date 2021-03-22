#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <stdio.h>
#define STR_BUFFER_SIZE 1025
typedef struct node Node;
typedef Node* Dictionary;
Dictionary create_dictionary(FILE* file);
int find_in_dict(Dictionary dict, char* word);
void free_dictionary(Dictionary* target);
#endif