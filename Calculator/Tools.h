
#ifndef EX3_TESTTOOLS_H
#define EX3_TESTTOOLS_H

#include <stdlib.h>

// ------------------------------ functions -----------------------------

/**
 *allocated memory by using malloc and returns the pointer or exits the program if the pointer is NULL
 * @param capacity the capacity of the memory
 * @return pointer that has been received from malloc
 */
void *allocateMemory(size_t capacity);

/**
* This function is given a string (array of chars), and  attempts to convert it to int.
* @param str - Array of chars of digits
* @return The converted int in success otherwise exits the program
*/
int convertStrToInt(char *str);

/**
 * checks if the char in a given index is a digit
 * @param index index of the char
 * @param text the text that contains the char
 * @return non zero int if the char is a digit and zero otherwise
 */
int isItDigit(int index, const char *text);


#endif
