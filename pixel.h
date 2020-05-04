#ifndef PIXEL_H
#define PIXEL_H


class Pixel
{
public:
    Pixel(int x, int y);
    Pixel(int x, int y, double brightness);
    int x;
    int y;
    double brightness = 1;

    double getBrightness();
};

#endif // PIXEL_H
