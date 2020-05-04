#include "pixel.h"

Pixel::Pixel(int x, int y)
{
    this->x = x;
    this->y = y;
}

Pixel::Pixel(int x, int y, double brightness)
{
    this->x = x;
    this->y = y;
    this->brightness = brightness;
}
