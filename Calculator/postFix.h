#ifndef EX3_POSTFIX_H
#define EX3_POSTFIX_H

// ------------------------------ includes -----------------------------

#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <errno.h>
#include "stack.h"
#include "inFix.h"
// ------------------------------ functions -----------------------------

/**
 * This function is given mathematical expression presented by postfix, evaluates it and returns the value
 * @param postfix mathematical expression presented by postfix
 * @param len the length of the expression
 * @return The value pf the expression
 */
int calculatePostfix(MathObject *postfix, int len);

#endif
