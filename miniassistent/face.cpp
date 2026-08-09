#include "face.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

Adafruit_SH1106G display(128, 64, &Wire, -1);

void initDisplay()
{
    Wire.begin(21, 22);

    display.begin(0x3C, true);
    display.clearDisplay();
    display.display();
}

void drawSleepAnimation()
{
    static int frame = 0;
    static unsigned long lastUpdate = 0;

    if (millis() - lastUpdate >= 500)
    {
        frame = (frame + 1) % 11;
        lastUpdate = millis();
    }

    display.setTextSize(2);
    display.setTextColor(SH110X_WHITE);

    switch (frame)
    {
    case 0:
        display.setCursor(95, 34);
        display.print("z");
        break;

    case 1:
        display.setCursor(97, 31);
        display.print("z");
        break;

    case 2:
        display.setCursor(99, 28);
        display.print("z");
        break;

    case 3:
        display.setCursor(101, 25);
        display.print("Z");
        break;

    case 4:
        display.setCursor(103, 22);
        display.print("Z");
        break;

    case 5:
        display.setCursor(105, 19);
        display.print("Z");
        break;

    case 6:
        display.setCursor(107, 16);
        display.print("Z");
        break;

    case 7:
        display.setCursor(109, 13);
        display.print("Z");
        break;

    case 8:
        display.setCursor(111, 10);
        display.print("Z");
        break;

    case 9:
        display.setCursor(113, 7);
        display.print("z");
        break;

    case 10:
        display.setCursor(115, 4);
        display.print("z");
        break;

    case 11:
        display.setCursor(117, 1);
        display.print("z");
        break;
    }
}

void drawFace(const FaceState &face)
{
    display.clearDisplay();

    if (face.sleepEyes)
    {
        // Closed eyes
        display.drawLine(38, 33, 54, 33, SH110X_WHITE);
        display.drawLine(74, 33, 90, 33, SH110X_WHITE);

        drawSleepAnimation();
    }
    else if (face.blink)
    {
        // Blink
        display.drawLine(38, 33, 54, 33, SH110X_WHITE);
        display.drawLine(74, 33, 90, 33, SH110X_WHITE);
    }
    else
    {
        // Left eye
        display.fillRoundRect(
            36 + face.eyeX,
            22 + face.eyeY,
            face.eyeWidth,
            face.eyeHeight,
            face.eyeRadius,
            SH110X_WHITE);

        // Right eye
        display.fillRoundRect(
            74 + face.eyeX,
            22 + face.eyeY,
            face.eyeWidth,
            face.eyeHeight,
            face.eyeRadius,
            SH110X_WHITE);
    }

    display.display();
}