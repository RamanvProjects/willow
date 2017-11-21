#ifndef PROCEDURAL_H
#define PROCEDURAL_H

#include "util/CommonIncludes.h"

class Procedural
{
public:
    static float noise1(int x, int y);

    static float cosInterp(float a, float b, float x);

    static float smoothNoise1(float x, float y);

    static float interpNoise1(float x, float y);

    static float valueNoise2d(float x, float y, float persistence, int octaves);
};

#endif // PROCEDURAL_H
