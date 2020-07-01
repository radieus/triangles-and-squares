#include "pixelwithcolor.h"

PixelWithColor::PixelWithColor(int x, int y, int R, int G, int B)
{
    this->x = x;
    this->y = y;
    this->R = R;
    this->G = G;
    this->B = B;
    this->intensity = 1;
}

PixelWithColor::PixelWithColor(int x, int y, int R, int G, int B, double I) {
    this->x = x;
    this->y = y;
    this->R = R;
    this->G = G;
    this->B = B;
    this->intensity = I;
}
