#include <Arduino.h>
#include "animations.h"
#include "face.h"
#include "expressions.h"

unsigned long notificationShowStart = 0;

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

// -------------------- Notification Animation --------------------

int notificationCurrentY = 22;
int notificationTargetY = 22;

// -------------------- Music Animation --------------------

int musicTargetX = 0;
unsigned long lastMusicMove = 0;
bool musicRight = true;

// -------------------- Touch Animation --------------------

bool touchActive = false;
unsigned long touchStart = 0;

int touchPhase = 0;

// =======================================================

void updateAnimation()
{
    unsigned long now = millis();

    Expression exp = getExpression();

    NotificationState notificationState =
        getNotificationState();

    // ===================================================
    // TOUCH DETECTION
    // ===================================================

    if (digitalRead(2) == HIGH && !touchActive)
    {
        touchActive = true;
        touchStart = now;
        touchPhase = 1;

        Serial.println("[TOUCH] Touch detected!");
    }

    if (touchActive)
    {
        unsigned long elapsed = now - touchStart;

        if (elapsed < 500)
        {
            touchPhase = 1;
        }
        else if (elapsed < 1000)
        {
            touchPhase = 2;
        }
        else if (elapsed < 1500)
        {
            touchPhase = 3;
        }
        else if (elapsed < 2000)
        {
            touchPhase = 4;
        }
        else
        {
            touchActive = false;
            touchPhase = 0;
        }
    }

    // ===================================================
    // NOTIFICATION POSITION
    // ===================================================

    if (notificationState == NOTIFICATION_ENTER ||
        notificationState == NOTIFICATION_SHOW)
    {
        notificationTargetY = 6;
    }
    else
    {
        notificationTargetY = 22;
    }

    FaceState face;

    // Default values
    face.eyeX = currentEyeX;
    face.eyeY = currentEyeY;
    face.blink = isBlinking;

    face.notificationMode =
        (notificationState != NOTIFICATION_IDLE);

    // ===================================================
    // BLINK
    // ===================================================

    if (exp != SLEEP)
    {
        if (!isBlinking &&
            now - lastBlink >= nextBlink)
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

    // ===================================================
    // NORMAL EYE MOVEMENT
    // ===================================================

    if (exp != SLEEP &&
        exp != MUSIC &&
        !face.notificationMode &&
        !touchActive)
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

    if (face.notificationMode)
    {
        face.eyeY = notificationCurrentY - 22;
    }
    else
    {
        face.eyeY = currentEyeY;
    }

    // ===================================================
    // NOTIFICATION SLIDE
    // ===================================================

    if (notificationCurrentY < notificationTargetY)
        notificationCurrentY++;

    if (notificationCurrentY > notificationTargetY)
        notificationCurrentY--;

    if (notificationState == NOTIFICATION_ENTER &&
        notificationCurrentY == notificationTargetY)
    {
        setNotificationState(NOTIFICATION_SHOW);
        notificationShowStart = millis();
    }

    // ===================================================
    // NOTIFICATION TIMEOUT
    // ===================================================

    if (notificationState == NOTIFICATION_SHOW)
    {
        if (now - notificationShowStart >= 4000)
        {
            setNotificationState(NOTIFICATION_IDLE);
            setExpression(NORMAL);
        }
    }

    // ===================================================
    // MUSIC ANIMATION
    // ===================================================

    if (exp == MUSIC && !touchActive)
    {
        if (now - lastMusicMove >= 500)
        {
            musicTargetX = musicRight ? 3 : -3;

            musicRight = !musicRight;

            lastMusicMove = now;
        }

        if (currentEyeX < musicTargetX)
            currentEyeX++;

        if (currentEyeX > musicTargetX)
            currentEyeX--;

        face.eyeX = currentEyeX;
    }

    // ===================================================
    // EXPRESSIONS
    // ===================================================

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
            break;

        case MUSIC:
            face.eyeWidth = 22;
            face.eyeHeight = 18;
            face.eyeRadius = 10;
            break;

        case SLEEP:
            face.sleepEyes = true;
            face.blink = false;
            break;

        default:
            break;
    }

    // ===================================================
    // TOUCH ANIMATION
    // ===================================================

// ===================================================
// TOUCH ANIMATION — SMOOTH HAPPY PET DOG 🐶
// ===================================================

if (touchActive && !face.notificationMode)
{
    unsigned long t = now - touchStart;

    // -----------------------------------------------
    // 0–400 ms
    // Gentle excited pop
    // -----------------------------------------------

    if (t < 400)
    {
        float p = (float)t / 400.0;

        face.eyeWidth = 18 + (int)(6 * p);
        face.eyeHeight = 22 + (int)(2 * p);
        face.eyeRadius = 8 + (int)(3 * p);

        face.eyeX = 0;

        face.leftEyeY = -(int)(3 * p);
        face.rightEyeY = -(int)(3 * p);
    }

    // -----------------------------------------------
    // 400–1000 ms
    // Smooth excited bounce
    // -----------------------------------------------

    else if (t < 1000)
    {
        float p = (float)(t - 400) / 600.0;

        float bounce = sin(p * PI * 2.0) * 3.0;

        face.eyeWidth = 24;
        face.eyeHeight = 20;
        face.eyeRadius = 10;

        face.eyeX = (int)(sin(p * PI * 2.0) * 2.0);

        face.leftEyeY = (int)bounce;
        face.rightEyeY = (int)bounce;
    }

    // -----------------------------------------------
    // 1000–1600 ms
    // Happy little side-to-side movement
    // -----------------------------------------------

    else if (t < 1600)
    {
        float p = (float)(t - 1000) / 600.0;

        face.eyeWidth = 23;
        face.eyeHeight = 19;
        face.eyeRadius = 10;

        face.eyeX = (int)(sin(p * PI * 2.0) * 3.0);

        face.leftEyeY = (int)(sin(p * PI * 2.0) * 1.5);
        face.rightEyeY = face.leftEyeY;
    }

    // -----------------------------------------------
    // 1600–2200 ms
    // Smoothly settle back
    // -----------------------------------------------

    else if (t < 2200)
    {
        float p = (float)(t - 1600) / 600.0;

        float settle = 1.0 - p;

        face.eyeWidth =
            18 + (int)(5 * settle);

        face.eyeHeight =
            22 - (int)(3 * settle);

        face.eyeRadius =
            8 + (int)(2 * settle);

        face.eyeX =
            (int)(3 * settle);

        face.leftEyeY =
            -(int)(2 * settle);

        face.rightEyeY =
            -(int)(2 * settle);
    }

    // -----------------------------------------------
    // Finished
    // -----------------------------------------------

    else
    {
        touchActive = false;

        face.eyeWidth = 18;
        face.eyeHeight = 22;
        face.eyeRadius = 8;

        face.eyeX = 0;
        face.leftEyeY = 0;
        face.rightEyeY = 0;
    }
}


    drawFace(face);
}