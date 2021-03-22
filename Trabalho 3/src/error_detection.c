#include "error_detection.h"
#include <string.h>

// Creates the temp_dictionary from reading words out of a file.
void search_for_errors(FILE* file, Dictionary dictionary) {

    // Store if any tweet has been found on the file.
    int tweet_found = 0;
    // Store if an error has been found in the current text.
    int found_error = 0;

    // Temporally stores a string. 
    char buffer[STR_BUFFER_SIZE];

    // Reads the entire file.
    while(fscanf(file, "%[a-zA-Z\"]", buffer) != EOF) {
        
        // Detects the star of a tweet.
        if(strcmp(buffer, "\"text\"\0") == 0) {
            tweet_found = 1;

            // Reads valid text.
            while(fscanf(file, "%[a-zA-Z]", buffer) != EOF) {

                // Detects the end of the tweet,
                if(strcmp(buffer, "truncated\0") == 0) {
                    found_error = 0;
                    putchar('\n');
                    break;
                }

                // Detect if a word is worong i.e. does not apear in the dictionary.
                if(strcmp(buffer, "\"text\"\0") != 0 && !find_in_dict(dictionary, buffer)) {
                    printf("%s ", buffer);
                    found_error = 1;
                }

                // Deals with invalid characters.
                fscanf(file, "%[^a-zA-Z]", buffer);
            }
        }

        // Deals with invalid characters.
        fscanf(file, "%[^a-zA-Z\"]", buffer);
        
    }

    // Prints if no tweet was found.
    if(!tweet_found)
        printf("No tweets to check\n");

}