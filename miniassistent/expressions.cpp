#include "expressions.h"

static Expression currentExpression = NORMAL;

static NotificationState currentNotificationState = NOTIFICATION_IDLE;

void setExpression(Expression exp)
{


    currentExpression = exp;
}

Expression getExpression()
{
    return currentExpression;
}

void setNotificationState(NotificationState state)
{
    currentNotificationState = state;
}

NotificationState getNotificationState()
{
    return currentNotificationState;
}