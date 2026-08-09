#include "expressions.h"

static Expression currentExpression = NORMAL;

void setExpression(Expression exp)
{


    currentExpression = exp;
}

Expression getExpression()
{
    return currentExpression;
}