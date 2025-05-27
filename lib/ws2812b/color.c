#include "color.h"

void HSV_to_RGB(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b)
{
    float c = v * s;
    float x = c * (1 - fabsf(fmodf(h / 60.0f, 2) - 1));
    float m = v - c;

    float r_, g_, b_;

    if (h < 60)
    {
        r_ = c;
        g_ = x;
        b_ = 0;
    }
    else if (h < 120)
    {
        r_ = x;
        g_ = c;
        b_ = 0;
    }
    else if (h < 180)
    {
        r_ = 0;
        g_ = c;
        b_ = x;
    }
    else if (h < 240)
    {
        r_ = 0;
        g_ = x;
        b_ = c;
    }
    else if (h < 300)
    {
        r_ = x;
        g_ = 0;
        b_ = c;
    }
    else
    {
        r_ = c;
        g_ = 0;
        b_ = x;
    }

    *r = (uint8_t)((r_ + m) * 255);
    *g = (uint8_t)((g_ + m) * 255);
    *b = (uint8_t)((b_ + m) * 255);
}