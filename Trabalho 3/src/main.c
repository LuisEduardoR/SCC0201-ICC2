#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"
#include "error_detection.h"

int main(int argc, char* argv[]) {

    // Reads the name of the dictionary and target files.
    char dictionary_name[STR_BUFFER_SIZE];
    char target_text_name[STR_BUFFER_SIZE];

    scanf(" %s", dictionary_name);
    scanf(" %s", target_text_name);


    // Tries opening both files and give an error if one of them fails to be read.
    FILE* dictionary_file = NULL;
    FILE* target_text_file = NULL;

    dictionary_file = fopen(dictionary_name, "r");
    if(dictionary_file == NULL)
        fprintf(stderr, "Error opening \"%s\"!\n", dictionary_name);

    target_text_file = fopen(target_text_name, "r");
    if(target_text_file == NULL)
        fprintf(stderr, "Error opening \"%s\"!\n", target_text_name);

    if(dictionary_file == NULL || target_text_file == NULL)
        return -1;

    //Creates the dictionary.
    Dictionary dictionary = create_dictionary(dictionary_file);
    if(dictionary == NULL)
        fprintf(stderr,"Error creating dictionary!\n");

    // Closes the dictionary file when it's not needed anymore.
    fclose(dictionary_file);

    // Search for errors in target text printing words that does not appear in the dictionary.
    search_for_errors(target_text_file, dictionary);

    // Closes the target text file.
    fclose(target_text_file);

    // Frees dictionary memory.
    free_dictionary(&dictionary);

    return 0;
}