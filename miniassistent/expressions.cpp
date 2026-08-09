#include "expressions.h"

static Expression currentExpression = HAPPY;

void setExpression(Expression exp)
{


    currentExpression = exp;
}

Expression getExpression()
{
    return currentExpression;
}