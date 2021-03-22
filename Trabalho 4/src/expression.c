#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "expression.h"
#include "stack.h"

#define NaN 0.0/0.0

// Enum used to express the type of a node.
typedef enum { Value = 0, Operation = 1 } NodeType;

// Node of the tree.
typedef struct node TreeNode;
struct node {

    // Type of the node.
    NodeType type;

    // Previous node.
    TreeNode* prev;

    // Following nodes.
    TreeNode* right;
    TreeNode* left;

    // Value of the node.
    double value;
    // Operator of the node.
    char operator;

};

// Checks for a openrand in a list, where the closest to the start have a greater priority, and returns it's priority.
int get_priority(char op, char* priority) {

    // Unary operators are executed with the highest priority.
    if(op == 'l' || op == 's' || op == 'e')
        return 100;

    // Binary operator priorities come from the priority list.
    int p = 0;
    for(int i = 0; i < 5; i++) {
        if(op == priority[i])
            return (5-i);
    }

    return -1;

}

// Converts the expression to posfix notation.
char* to_posfix(char* expression, char* priority) {

    // Allocates memory for the converted expression.
    char* converted = NULL;
    converted = (char*)malloc(2048*sizeof(char));
    if(converted == NULL)
        return NULL;

    // Store the positions being used in the expressions.
    unsigned pos = 0;
    unsigned conv_pos = 0;

    // Stack to be used by the conversion process.
    Stack* operators = create_stack();

    // Reads and converts the expression to the end.
    while(expression[pos] != '\0') {

        // Prints operands to the new string.
        if(expression[pos] >= 0x30 && expression[pos] < 0x3a) {

            while((expression[pos] >= 0x30 && expression[pos] < 0x3a) || expression[pos] == '.') {
                converted[conv_pos] = expression[pos];
                pos++;
                conv_pos++;
            }

            converted[conv_pos] = ',';
            conv_pos++;

        } else { // Deals with operators and parenthesis.

            // Deals with binary operators.
            if(expression[pos] == '+' || expression[pos] == '-' || expression[pos] == '/' || (expression[pos] == '*' && expression[pos+1] != '*')) {

                // Checks if the next char is valid.
                if ((expression[pos+1] <= 0x30 || expression[pos+1] > 0x3a) 
                && expression[pos+1] != '(' && expression[pos+1] != '[' && expression[pos+1] != '{'
                && expression[pos+1] != ',' && expression[pos+1] != ';' 
                && expression[pos+1] != 'l' && expression[pos+1] != 's' && expression[pos+1] != 'e') {
                    delete_stack(operators);
                    free(converted);
                    return NULL;
                }

                while(stack_size(operators) > 0 && get_priority(expression[pos], priority) <= get_priority(top(operators), priority)) {

                    converted[conv_pos] = pop(operators);
                    converted[conv_pos+1] = ',';
                    conv_pos+=2;
                        
                }

                push(operators, expression[pos]); // Put the current char on the stack.
                
            } else if(expression[pos] == '*' && expression[pos+1] == '*') {

                if ((expression[pos+2] <= 0x30 || expression[pos+2] > 0x3a) 
                && expression[pos+2] != '(' && expression[pos+2] != '[' && expression[pos+2] != '{'
                && expression[pos+2] != ',' && expression[pos+2] != ';' 
                && expression[pos+2] != 'l' && expression[pos+2] != 's' && expression[pos+2] != 'e') {
                    delete_stack(operators);
                    free(converted);
                    return NULL;
                }

                while(stack_size(operators) > 0 && get_priority('^', priority) <= get_priority(top(operators), priority)) {

                    converted[conv_pos] = pop(operators);
                    converted[conv_pos+1] = ',';
                    conv_pos+=2;
                        
                }

                push(operators, '^'); // Put the current char on the stack.
                pos++;  // Used to compensate the fact that this character is double lenght.

            } else if(expression[pos] == 'l') {
                push(operators, 'l');
                pos+=2;
            } else if(expression[pos] == 's') {
                push(operators, 's');
                pos+=3;
            } else if(expression[pos] == 'e') {
                push(operators, 'e');
                pos+=2;
            } else if(expression[pos] == '(' || expression[pos] == '[' || expression[pos] == '{') { // Opens a parenthesis.
                push(operators, expression[pos]);
            } else if(expression[pos] == ')' || expression[pos] == ']' || expression[pos] == '}') { // Closes a parenthesis.

                char closing = expression[pos];

                while(top(operators) != '(' && top(operators) != '[' && top(operators) != '{'){ 

                    if(stack_size(operators) <= 0) {
                        delete_stack(operators);
                        free(converted);
                        return NULL;
                    }

                    converted[conv_pos] = pop(operators);
                    converted[conv_pos+1] = ',';
                    conv_pos+=2;                    

                }

                if((closing == ')' && top(operators) == '(') 
                || (closing == ']' && top(operators) == '[') 
                || (closing == '}' && top(operators) == '{')) {
                    pop(operators);
                } else {
                    delete_stack(operators);
                    free(converted);
                    return NULL;
                }

            }

            pos++;

        }
    }

    // Deals with remaining operators in the stack.
    while(stack_size(operators) > 0) {
        if(top(operators) != '(' && top(operators) != '[' && top(operators) != '{') {
            converted[conv_pos] = pop(operators);
            converted[conv_pos+1] = ',';
            conv_pos+=2;
        } else {
            delete_stack(operators);
            free(converted);
            return NULL;
        }
    }

    // Free the stack.
    delete_stack(operators);

    // Puts a \0 in the end of the converted expression and returns it.
    converted[conv_pos] = '\0';
    return converted;

}

// Builds the tree from the posfix expression.
TreeNode* build_expression(TreeNode* prev, char* posfix, char* priority, unsigned ini, unsigned end) {

    // Allocates memory for the new node.
    TreeNode* node = NULL;    
    node = (TreeNode*)malloc(sizeof(TreeNode));
    if(node == NULL)
        return NULL;

    // Sets the "father" node of the new node.
    node->prev = prev;

    // Initializes child nodes.
    node->left = NULL;
    node->right = NULL;

    // Gets the content of the new node;
    unsigned pos = end-1;
    while(pos > ini) {
        if(posfix[pos] == ',')
            break;

        pos--;
    }

    // Checks if the content is an operator and sets the node type.
    if(posfix[pos+1] == '+' || posfix[pos+1] == '-' || posfix[pos+1] == '/' || posfix[pos+1] == '*' 
    || posfix[pos+1] == '^' || posfix[pos+1] == 'l' || posfix[pos+1] == 's' || posfix[pos+1] == 'e') {
        node->type = Operation;
        node->operator = posfix[pos+1];
    } else { // Else, scans for the operand and sets the node type.
        node->type = Value;
        sscanf(&(posfix[pos]), "%lf", &(node->value));
        return node; // If the current node is a value, there is no child nodes.
    }
    
    // If the node is an operator, builds the child nodes.
    if(node->operator != 'l' && node->operator != 's' && node->operator != 'e') {
        
        // Finds the positions of the child nodes on the string.
        unsigned elements_back = 1;
        pos--;
        while(pos >= ini && elements_back > 0) {

            if(posfix[pos] == ',')
                elements_back--;

            if(elements_back == 0)
                    break;
            else if(posfix[pos] == '+' || posfix[pos] == '-' || posfix[pos] == '/' || posfix[pos] == '*' || posfix[pos] == '^')
                elements_back+=2;
            else if(posfix[pos] == 'l' || posfix[pos] == 's' || posfix[pos] == 'e')
                elements_back++;

            pos--;

        }
    }

    // Builds both sides of a binary operation.
    if(node->operator != 'l' && node->operator != 's' && node->operator != 'e') {
        node->right = build_expression(node, posfix, priority, pos+1, end-2);
        node->left = build_expression(node, posfix, priority, ini, pos);
    } else  // If there is a unary operation, only one child node is needed.
        node->right = build_expression(node, posfix, priority, ini, end-2);

    return node;

}

// Start solving a built expression tree.
double solve(TreeNode* node) {
    
    // If this node is a value returns it.
    if(node->type == Value) {
        double v = node->value;
        free(node); // Frees the memory of this node.
        return v;
    }

    // Deals with binary operations.
    if(node->operator != 'l' && node->operator != 's' && node->operator != 'e') {

        double a, b;

        // Gets a value from the left node.
        a = solve(node->left);
        // Gets a value from the right node.
        b = solve(node->right);
        
        // Returns the result of the current operation and frees this node.
        switch(node->operator) {
            case '+':
                free(node);
                return a+b;        
                break;
            case '-':
                free(node);
                return a-b;
                break;
            case '*':
                free(node);
                return a*b;
                break;
            case '/':
                free(node);
                return a/b;
                break;
            case '^':
                free(node);
                return pow(a,b);
                break;
        }

    } else { // Deals with unary operations.

        double u;

        // Gets a value from the right node.
        u = solve(node->right);
        
        // Returns the result of the current operation and frees this node.
        switch(node->operator) {
            case 'l':
                free(node);
                return log2(u);        
                break;
            case 's':
                free(node);
                return sqrt(u);
                break;
            case 'e':
                free(node);
                return exp(u);
                break;

        }
    }
}

// Solves the expression returning the result, if there is an error returns NaN.
float solve_expression(char* expression, char* priority) {

    // Converts the expression to posfix notation.
    char* posfix = NULL;
    posfix = to_posfix(expression, priority);
    if(posfix == NULL)
        return NaN;


    // Creates a expression tree from the posfix expression.
    TreeNode* tree;
    tree = NULL;
    tree = build_expression(NULL, posfix, priority, 0, strlen(posfix) - 1);
    if(tree == NULL)
        return NaN;
    // Frees the posfix expression memory.
    free(posfix);

    // Solves the expression tree and returns the result.
    return solve(tree);

}