
// ------------------------------ includes -----------------------------

#include <math.h>
#include <stdio.h>
#include <memory.h>
#include "stack.h"
#include <assert.h>
#include "inFix.h"


// ------------------------------ functions -----------------------------

/**
 * This function is given two operands and an operator and evaluates the expression
 * @param operand1 an operand
 * @param operand2 an operand
 * @param operator and operator
 * @return the result of teh expression
 */
int getResult(int operand1, int operand2, char operator)
{

    switch (operator)
    {
        case plus:
            return operand2 + operand1;
        case minus:
            return operand2 - operand1;
        case mul:
            return operand1 * operand2;
        case division:
            if (operand1 == 0)
            {
                fprintf(stderr, "Division by 0!");
                exit(EXIT_FAILURE);
            }
            return operand2 / operand1;
        case power:
            return (int) pow(operand2, operand1);

        default:
            exit(EXIT_FAILURE);
    }
}

/**
* This function is given mathematical expression presented by postfix, evaluates it and returns the value
* @param postfix mathematical expression presented by postfix
* @param len the length of the expression
* @return The value pf the expression
*/
int calculatePostfix(MathObject *postfix, int len)
{
    if (len == 0)
    {
        fprintf(stderr, "Can't evaluate expression\n");
        exit(EXIT_FAILURE);
    }
    int i;
    char currType = 0;
    int result, operand1, operand2, currValue;
    operand1 = operand2 = 0;
    Stack *stack = stackAlloc(sizeof(int));

    for (i = 0; i < len; i++)
    {
        result = 0;
        currType = postfix[i].type;
        currValue = postfix[i].value;
        if (currType == operand)
        {
            push(stack, &currValue);
        }
        else
        {
            assert(!isEmptyStack(stack));
            pop(stack, &operand1);
            assert(!isEmptyStack(stack));
            pop(stack, &operand2);
            result = getResult(operand1, operand2, currType);
            push(stack, &result);
        }
    }
    pop(stack, &result);
    freeStack(&stack);
    return result;
}
