// ------------------------------ includes -----------------------------

#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <errno.h>
#include "stack.h"
#include "inFix.h"
#include <assert.h>
#include "Tools.h"

// -------------------------- const definitions -------------------------

#define TRUE 1

// -------------------------- functions -------------------------

/**
 * This function pops the top of the stack until it ends and and each time it adds the top of the stack to
 * the given array of objects
 */
void appendFromStack(int counter, Stack *stack, MathObject *convert)
{
    char headData;
    while (!isEmptyStack(stack))
    {
        pop(stack, &headData);
        convert[counter].type = headData;
        counter++;
    }
}

/**
 *This function gets the top of the given stack 
 */
char getTopStack(Stack *stack, char headData)
{
    assert(!isEmptyStack(stack));
    pop(stack, &headData);
    push(stack, &headData);
    return headData;
}

/**
 * This function adds the given type and value to the given array of math object
 */
int appendOperand(int counter, MathObject *convert, char currType, int currValue)
{
    convert[counter].value = currValue;
    convert[counter].type = currType;
    counter++;
    return counter;
}

/**
 * This function is given two operators and returns true if the precedence of the first type is smaller or equal
 * to the second type
 */
int getPrecedence(char type1, char type2)
{
    if (type1 == mul || type1 == division)
    {
        return type2 != minus && type2 != plus;
    }
    else if (type1 == plus || type1 == minus)
    {
        return TRUE;
    }
    else
    {
        return type2 == power;
    }
}

/**
 * This function pops and add the top of the stack to the given array of object till the first of the
 * stack is either left parenthesis or the precedence of the head of the stack is bigger than the given type
 */
int popTillLeftPar(Stack *stack, MathObject *convert, char type, int counter)
{
    char headData;
    int isOperator = type != lPar && type != rPar;

    while (!isEmptyStack(stack))
    {
        pop(stack, &headData);
        if (headData == lPar || (isOperator && !getPrecedence(type, headData)))
        {
            if (isOperator)
            {
                push(stack, &headData);
            }
            break;
        }
        convert[counter].type = headData;
        counter++;
    }
    return counter;
}

/**
 * This function handles an operator in the progress of converting infix to post fix
 */
int handleOperator(int counter, Stack *stack, MathObject *convert, char headData, char currType)
{
    int isEmpty = isEmptyStack(stack);
    if (!isEmpty)
    {
        headData = getTopStack(stack, headData);
    }
    if (isEmpty || headData == lPar)
    {
        push(stack, &currType);
    }
    else
    {
        counter = popTillLeftPar(stack, convert, currType, counter);
        push(stack, &currType);
    }
    return counter;
}

/**
*This function  is given mathematical expression presented by infix and converts it to
* mathematical expression presented by postfix and returns it
* @param infix mathematical expression to be converted to Postfix expression
* @param len length of the expression
* @return the mathematical expression in postfix
*/
MathObject *inToPost(MathObject *infix, int len)
{
    int i, currValue, counter = 0;
    Stack *stack = stackAlloc(sizeof(char));
    MathObject *converted = (MathObject *) allocateMemory(MAX_LINE * sizeof(MathObject));
    char headData, currType;
    headData = 0;

    for (i = 0; i < len; i++)
    {
        currType = infix[i].type;
        currValue = infix[i].value;

        if (currType == operand)
        {
            counter = appendOperand(counter, converted, currType, currValue);

        }
        else if (currType != lPar && currType != rPar)
        {
            counter = handleOperator(counter, stack, converted, headData, currType);
        }
        else
        {
            if (currType == lPar)
            {
                push(stack, &currType);
            }
            else
            {
                counter = popTillLeftPar(stack, converted, currType, counter);
            }
        }
    }
    appendFromStack(counter, stack, converted);
    freeStack(&stack);
    return converted;
}