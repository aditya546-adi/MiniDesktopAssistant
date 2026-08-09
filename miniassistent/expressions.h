#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

enum Expression
{
    NORMAL,
    HAPPY,
    SURPRISED,
    SAD,
    THINKING,
    ALERT,
    MESSAGE,
    SLEEP
};

void setExpression(Expression exp);
Expression getExpression();

#endif