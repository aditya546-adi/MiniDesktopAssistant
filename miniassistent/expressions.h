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

enum NotificationState
{
    NOTIFICATION_IDLE,
    NOTIFICATION_ENTER,
    NOTIFICATION_SHOW,
    NOTIFICATION_EXIT
};

void setNotificationState(NotificationState state);
NotificationState getNotificationState();

#endif