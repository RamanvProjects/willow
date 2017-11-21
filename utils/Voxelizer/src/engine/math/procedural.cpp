#include "procedural.h"

float Procedural::cosInterp(float a, float b, float x) {
    float ft = x * 3.1415927f;
    float f = (1 - glm::cos(ft)) * .5f;

    return a * (1-f) + b * f;
}

float Procedural::noise1(int x, int y) {
    int n = x + y * 57;
    n = (n<<13) ^n;
    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff)/1073741824.0f);
}

float Procedural::smoothNoise1(float x, float y) {
    float corners = (noise1(x-1, y-1)+noise1(x+1, y-1)+noise1(x-1, y+1)+noise1(x+1,y+1))/16.f;
    float sides = (noise1(x-1,y) + noise1(x+1,y) + noise1(x, y-1) + noise1(x, y+1))/8.f;
    float center = noise1(x,y)/4;
    return corners + sides + center;
}

float Procedural::interpNoise1(float x, float y) {
    int intX = (int)x;
    float fracX = x - intX;

    int intY = (int)y;
    float fracY = y - intY;

    float v1 = smoothNoise1(intX, intY);
    float v2 = smoothNoise1(intX+1, intY);
    float v3 = smoothNoise1(intX, intY+1);
    float v4 = smoothNoise1(intX+1, intY+1);

    float i1 = cosInterp(v1, v2, fracX);
    float i2 = cosInterp(v3, v4, fracX);

    return cosInterp(i1, i2, fracY);
}

float Procedural::valueNoise2d(float x, float y, float p, int ocaves) {
    float total = 0;
    float freq = .1f;

    for(int i = 0; i < ocaves; i++) {
        float frequency = glm::pow(2, i)*freq;
        float amplitude = glm::pow(p, i);

        total += interpNoise1(x * frequency, y*frequency) * amplitude;
    }
    return total;
}
