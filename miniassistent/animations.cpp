#include <Arduino.h>
#include "animations.h"
#include "face.h"
#include "expressions.h"

// -------------------- Blink --------------------

unsigned long lastBlink = 0;
unsigned long blinkStart = 0;

bool isBlinking = false;

unsigned long nextBlink = 2500;

// -------------------- Eye Movement --------------------

int currentEyeX = 0;
int targetEyeX = 0;

int currentEyeY = 0;
int targetEyeY = 0;

unsigned long lastEyeMove = 0;
unsigned long nextEyeMove = 3000;

// =======================================================

void updateAnimation()
{
    
    unsigned long now = millis();

    Expression exp = getExpression();

    FaceState face;

    // Default values
    face.eyeX = currentEyeX;
    face.eyeY = currentEyeY;
    face.blink = isBlinking;

    face.notificationMode = (exp == MESSAGE); //temp

    // -------------------- Blink --------------------

    if (exp != SLEEP)
    {
        if (!isBlinking && now - lastBlink >= nextBlink)
        {
            isBlinking = true;
            blinkStart = now;
        }

        if (isBlinking)
        {
            if (now - blinkStart >= 120)
            {
                isBlinking = false;
                lastBlink = now;
                nextBlink = random(1800, 5500);
            }
        }
    }
    else
    {
        isBlinking = false;
    }

    face.blink = isBlinking;

    if (face.notificationMode)
    {
        face.blink = false;
    }

    // -------------------- Eye Movement --------------------

    if (exp != SLEEP && !face.notificationMode)
    {
        if (!isBlinking)
        {
            if (now - lastEyeMove >= nextEyeMove)
            {
                switch (random(0, 3))
                {
                    case 0:
                        targetEyeX = -4;
                        break;

                    case 1:
                        targetEyeX = 0;
                        break;

                    case 2:
                        targetEyeX = 4;
                        break;
                }

                switch (random(0, 3))
                {
                    case 0:
                        targetEyeY = -1;
                        break;

                    case 1:
                        targetEyeY = 0;
                        break;

                    case 2:
                        targetEyeY = 1;
                        break;
                }

                lastEyeMove = now;
                nextEyeMove = random(1000, 4000);
            }

            if (currentEyeX < targetEyeX)
                currentEyeX++;

            if (currentEyeX > targetEyeX)
                currentEyeX--;

            if (currentEyeY < targetEyeY)
                currentEyeY++;

            if (currentEyeY > targetEyeY)
                currentEyeY--;
        }
    }

    face.eyeX = currentEyeX;
    face.eyeY = currentEyeY;
        // -------------------- Expressions --------------------

    switch (exp)
    {
        case NORMAL:
            break;

        case HAPPY:
            face.eyeWidth = 22;
            face.eyeHeight = 18;
            face.eyeRadius = 10;
            break;

        case SURPRISED:
            face.eyeWidth = 12;
            face.eyeHeight = 32;
            face.eyeRadius = 6;
            face.eyeGap = 44;
            break;

        case SAD:
            face.eyeWidth = 16;
            face.eyeHeight = 24;
            face.eyeRadius = 6;
            face.eyeGap = 38;
            break;

        case THINKING:
            face.eyeWidth = 18;
            face.eyeHeight = 22;
            face.eyeRadius = 4;
            face.eyeGap = 36;
            break;

        case ALERT:
            face.eyeWidth = 14;
            face.eyeHeight = 30;
            face.eyeRadius = 5;
            face.eyeGap = 42;
            break;

        case MESSAGE:
            face.eyeWidth = 20;
            face.eyeHeight = 20;
            face.eyeRadius = 12;
            face.eyeGap = 38;
            face.eyeY = -2;
            break;

        case SLEEP:
            face.sleepEyes = true;
            face.blink = false;
            break;

        default:
            break;
    }

    drawFace(face);
}