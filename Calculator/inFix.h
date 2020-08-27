#ifndef EX3_INFIX_H
#define EX3_INFIX_H
// ------------------------------ includes -----------------------------

#define MAX_LINE 100

#include "stack.h"

// ------------------------------ enum -----------------------------

/**
 * The type the program uses to calculate
 */
typedef enum
{
    plus = '+',
    minus = '-',
    mul = '*',
    division = '/',
    power = '^',
    rPar = ')',
    lPar = '(',
    operand = 'p',
} type;

// ------------------------------ structures -----------------------------

/**
* Represents a mathematical object from a mathematical expression
*/
typedef struct
{
    int value;
    char type;
} MathObject;

// ------------------------------ includes -----------------------------


/**
 *This function  is given mathematical expression presented by infix and converts it to
 * mathematical expression presented by postfix and returns it
 * @param infix mathematical expression to be converted to Postfix expression
 * @param len length of the expression
 * @return the mathematical expression in postfix
 */
MathObject *inToPost(MathObject *infix, int len);

#endif
