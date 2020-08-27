// ------------------------------ includes -----------------------------

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "Tools.h"

// ------------------------------ functions -----------------------------
/**
 *allocated memory by using malloc and returns the pointer or exits the program if the pointer is NULL
 * @param capacity the capacity of the memory
 * @return pointer that has been received from malloc
 */
void *allocateMemory(size_t capacity)
{
    void *pointer = malloc(capacity);
    if (pointer == NULL)
    {
        exit(EXIT_FAILURE);
    }
    return pointer;
}

/**
 * converts the given string into int
 * @param str The string to be converted to int
 * @return The value of the sting in int
 */
int convertStrToInt(char *str)
{
    char *end;
    errno = 0;
    int val;
    val = (int) strtol(str, &end, 10);
    if (val == 0 && (errno != 0 || *end != '\0'))
    {
        fprintf(stderr, "Error: input is not a valid Integer\n");
        exit(EXIT_FAILURE);
    }
    return val;
}

int isItDigit(int index, const char *text)
{
    return '0' <= text[index] && text[index] <= '9';
}

