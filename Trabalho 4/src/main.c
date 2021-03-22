#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "expression.h"

int main(void) {

    // Buffer to store the read expressions.
    char buffer[1024];

    // Stores the priority of the expressions.
    char expression_priority[5];

    for(int i = 0; i < 5; i++) {

        scanf(" %s", buffer);

        if(buffer[0] != '*') {
            expression_priority[i] = buffer[0];
        } else { // Converts the '**' operand to a single character '^' for easier use later.
            if(buffer[1] == '*') {
                expression_priority[i] = '^';
            } else {
                expression_priority[i] = '*';
            }
        }

    }

    // Stores the expressions in string format.
    char** expression_cache = NULL;
    // Stores the amount of expressions.
    unsigned expression_amount = 0;

    // Reads expressions.
    while(scanf(" %s", buffer) != EOF) {

        // Substitutes the last char with a '\0', the last char should always be either ',' or ';'.
        // It's stored because if it's a ';' the while loop should finish.
        char last_char = buffer[strlen(buffer) - 1];
        buffer[strlen(buffer) - 1] = '\0';
        
        // Increases expression counter.
        expression_amount++;

        // Allocates memory for the expression.
        expression_cache = (char**)realloc(expression_cache, expression_amount * sizeof(char*));
        if(expression_cache == NULL) {
            fprintf(stderr, "Error allocating memory for the expression cache!");
            return 1;
        }

        expression_cache[expression_amount - 1] = NULL;
        expression_cache[expression_amount - 1] = (char*)malloc((strlen(buffer) + 1) * sizeof(char)); 
        if(expression_cache[expression_amount - 1] == NULL) {
            fprintf(stderr, "Error allocating memory for the expression cache!");
            return 1;
        }

        
        // Copies the expression to the memory.
        strcpy(expression_cache[expression_amount - 1], buffer);
        
        // Ends the while if the last expression has been read.
        if(last_char == ';') break;

    }

    // Solves the expressions and gives the awnser.
    double solution;
    for(int i = 0; i < expression_amount; i++) {

        solution = solve_expression(expression_cache[i], expression_priority);

        // If there is a problem NaN will be returned.
        if(!isnan(solution))
            printf("%.2lf\n", solution);
        else
            printf("Expressao incorreta.\n");

    }

    // Frees the memory allocated to store the expressions.
    for(int i = 0; i < expression_amount; i++) {
        free(expression_cache[i]);
    }
    free(expression_cache);

    return 0;
}