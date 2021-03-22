#ifndef STACK_H
#define STACK_H
typedef struct stack Stack;

// Adds an element to the stack.
char push(Stack* stck, char c);

// Returns the top of the stack.
char top(Stack* stck);

// Removes an element from the stack and returns it.
char pop(Stack* stck);

// Create a stack and initializes it.
Stack* create_stack();

// Gets the size of the stack.
unsigned stack_size(Stack* stck);

// Deletes a stack.
Stack* delete_stack(Stack* stck);
#endif