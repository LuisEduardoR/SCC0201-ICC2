#include "stack.h"
#include <stdlib.h>

struct stack {

    unsigned size;
    char content[1024];

};

// Adds an element to the stack.
char push(Stack* stck, char c) {

    if(stck->content == NULL)
    return 1;

    stck->size++;

    stck->content[stck->size - 1] = c;
    return 0;

}

// Returns the top of the stack.
char top(Stack* stck) {

    char c = '\0';

    if(stck->size > 0)
        c = stck->content[stck->size - 1];

    return c;

}

// Removes an element from the stack and returns it.
char pop(Stack* stck) {

    char c = '\0';

    if(stck->size > 0) {
        c = stck->content[stck->size - 1];
        stck->size--;
    }

    return c;

}

// Gets the stack size.
unsigned stack_size(Stack* stck) {

    return stck->size;

}

// Create a stack and initializes it.
Stack* create_stack() {

    Stack* stck = (Stack*)malloc(sizeof(Stack));

    stck->size = 0;

    return stck;

}

// Deletes a stack.
Stack* delete_stack(Stack* stck) {

    free(stck);
    return NULL;

}