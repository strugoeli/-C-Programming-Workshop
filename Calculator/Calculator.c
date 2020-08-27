/**
 * @file Calculator.c
 * @author  Eliyahu Strugo <eli.strugo@mail.huji.ac.il>
 * @version 1.0
 * @date 27 Nov 2018
 *
 * @brief The program reads it reads mathematical expressions presented by infix converts it to postfix
 * and calculates the value.
 *
 * @section DESCRIPTION
 * Input  : mathematical expressions presented by infix
 * Process: in each iteration parsing the expression that is given  by the user converts it to postfix
 * and evaluate it.
 * Output : prints the infix presentation, postfix presentation and the value
 */



// ------------------------------ includes ------------------------------

#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <errno.h>
#include "stack.h"
#include "inFix.h"
#include "postFix.h"
#include "Tools.h"

// -------------------------- const definitions -------------------------

#define MAX_LINE 100
#define END_OF_LINE '\n'
#define EMPTY_STR '\0'

// ------------------------------ functions -----------------------------

/**
 * This function is given a string and counts the number of digits from the beginning till the first non digit char
 * @param buffer a string that includes math expression
 * @param charsNum the number of chars in the main expression
 * @return the number of digits till there is a type that is not a digit
 */
size_t getDigitNum(const char *buffer, int charsNum)
{
    size_t digitCounter = 0;
    while (isItDigit(charsNum, buffer))
    {
        digitCounter++;
        charsNum++;
    }
    return digitCounter;
}

/**
 *Finds if the given char is operator
 * @param type the type of the given char
 * @return non-zero if true and false otherwise
 */
int isOperator(char type)
{
    return type == plus || type == minus || type == mul || type == division || type == power;
}

/**
* This function is given mathematical expression presented and prints it
* @param valueNum The number of values
* @param postfix mathematical expressions
*/

void printExpression(int valueNum, const MathObject *expression, char *order)
{
    int i;
    printf("%s: ", order);
    for (i = 0; i < valueNum; i++)
    {
        if (expression[i].type == operand)
        {
            printf("% d ", expression[i].value);
        }
        else
        {
            printf("%c", expression[i].type);
        }
    }
    printf("\n");
}

/**
 * This function is given mathematical expressions presented by postfix
 * calculates its value and prints it.
 * @param postFix mathematical expressions presented by postfix
 * @param len length of the mathematical expressions
 */
void printValue(MathObject *postFix, int len)
{
    int theValue = calculatePostfix(postFix, len);
    printf("The value is %d", theValue);
    printf("\n");
}

/**
 *This function is given mathematical expression and store the digits from a certain place till it meets non-digit char
 *and then it converts the digits into number and store it in the given array
 * @param valuesNum Number of value in the expression
 * @param capacity The current capacity of memory that allocated for the container of the string that
 * represents a number
 * @param inPut The array that will be contain the infix expression
 * @param buffer The container for the whole expression
 * @param charsNum
 * @param num container for the string that represents a number
 * @return
 */
int parseNumber(char *buffer, char *num, size_t digitCounter)
{
    num[digitCounter] = EMPTY_STR;
    strncpy(num, buffer, digitCounter);
    char *temp = num;


    return convertStrToInt(temp);
}


/**
 * This function generates and prints the output of the program
 * @param valuesNum Number of value in the expression
 * @param parenthesisNum number of parenthesis in the expression
 * @param infix mathematical expressions presented by infix
 */
void generateOutput(int valuesNum, int parenthesisNum, MathObject *infix)
{
    printExpression(valuesNum, infix, "Infix");
    MathObject *postfix = inToPost(infix, valuesNum);
    printExpression(valuesNum - parenthesisNum, postfix, "Postfix");
    printValue(postfix, valuesNum - parenthesisNum);
    free(postfix);
}

/**
 * This function runs the calculator program. it reads mathematical expressions presented by infix
 * from the user store them, converts them into postfix and finally calculates the expression.
 */
void runCalculator()
{
    int counterChars, counterValues, counterPars;

    MathObject inPut[MAX_LINE];
    char *buffer = malloc(MAX_LINE);
    char *num = NULL;
    char currType;
    int currNum = 0;

    while (fgets(buffer, MAX_LINE, stdin))
    {
        counterChars = 0, counterValues = 0, counterPars = 0;
        while (buffer[counterChars] != END_OF_LINE)
        {
            currType = buffer[counterChars];
            if (isItDigit(counterChars, buffer))
            {
                char *p = (buffer + counterChars);
                size_t digitCounter;
                digitCounter = getDigitNum(buffer, counterChars);
                counterChars += digitCounter;

                num = malloc(digitCounter + 1);
                char *temp = num;
                currNum = parseNumber(p, num, digitCounter);
                inPut[counterValues].value = currNum;
                inPut[counterValues].type = operand;
                free(temp);
            }
            else if (isOperator(currType) || currType == lPar || currType == rPar)
            {
                inPut[counterValues].type = buffer[counterChars];

                if (currType == lPar || currType == rPar)
                {
                    counterPars++;
                }
                counterChars++;
            }
            else
            {
                counterChars++;
                counterValues--;
            }
            counterValues++;
        }

        generateOutput(counterValues, counterPars, inPut);
    }
    free(buffer);
}

int main()
{
    runCalculator();
    return 0;
}
