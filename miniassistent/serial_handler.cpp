#include <Arduino.h>

#include "serial_handler.h"
#include "expressions.h"

static String notificationTitle = "";
static String notificationBody = "";

void initSerialHandler()
{
    Serial.begin(115200);
}

void updateSerialHandler()
{
    if (!Serial.available())
        return;

    String command = Serial.readStringUntil('\n');
    command.trim();

    Serial.print("Received: ");
    Serial.println(command);

    if (command == "NORMAL")
{
    setNotificationState(NOTIFICATION_IDLE);
    setExpression(NORMAL);
}

    else if (command == "HAPPY")
    {
    setNotificationState(NOTIFICATION_IDLE);
    setExpression(HAPPY);
}

    else if (command == "SURPRISED")
 {
    setNotificationState(NOTIFICATION_IDLE);
    setExpression(SURPRISED);
}

    else if (command == "SAD")
{
    setNotificationState(NOTIFICATION_IDLE);
    setExpression(SAD);
}

    else if (command == "THINKING")
{
    setNotificationState(NOTIFICATION_IDLE);
    setExpression(THINKING);
}

    else if (command == "ALERT")
{
    setNotificationState(NOTIFICATION_IDLE);
    setExpression(ALERT);
}

else if (command == "MUSIC")
    setExpression(MUSIC);

    else if (command == "MESSAGE")
    {
        setExpression(MESSAGE);
        setNotificationState(NOTIFICATION_ENTER);
    }

    else if (command == "SLEEP")
        setExpression(SLEEP);

    else if (command.startsWith("TEXT:"))
    {
        notificationTitle = command.substring(5);

        Serial.print("Title: ");
        Serial.println(notificationTitle);
    }

    else if (command.startsWith("BODY:"))
    {
        notificationBody = command.substring(5);

        Serial.print("Body: ");
        Serial.println(notificationBody);
    }

}

String getNotificationTitle()
{
    return notificationTitle;
}

String getNotificationBody()
{
    return notificationBody;
}