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
        setExpression(NORMAL);

    else if (command == "HAPPY")
        setExpression(HAPPY);

    else if (command == "SURPRISED")
        setExpression(SURPRISED);

    else if (command == "SAD")
        setExpression(SAD);

    else if (command == "THINKING")
        setExpression(THINKING);

    else if (command == "ALERT")
        setExpression(ALERT);

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