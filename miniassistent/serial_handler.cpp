#include <Arduino.h>

#include "serial_handler.h"
#include "expressions.h"

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
        setExpression(MESSAGE);

    else if (command == "SLEEP")
        setExpression(SLEEP);
}