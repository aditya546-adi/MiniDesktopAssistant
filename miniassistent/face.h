#ifndef FACE_H
#define FACE_H

struct FaceState
{
    int eyeX = 0;
    int eyeY = 0;

    int eyeWidth = 18;
    int eyeHeight = 22;
    int eyeRadius = 8; 
    int eyeGap = 38;

    bool blink = false;

    bool sleepEyes = false;
};

void initDisplay();
void drawFace(const FaceState &face);

#endif