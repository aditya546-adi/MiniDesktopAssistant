#include "face.h"
#include "animations.h"
#include "serial_handler.h"

void setup()
{
    initDisplay();
    initSerialHandler();

    randomSeed(analogRead(0));
}

/* void loop()
{
    updateSerialHandler();
    updateAnimation();
} */

void loop()
{
    static unsigned long last = 0;

    if (millis() - last > 1000)
    {
        Serial.println("Loop Running");
        last = millis();
    }

    updateAnimation();
    updateSerialHandler();
}