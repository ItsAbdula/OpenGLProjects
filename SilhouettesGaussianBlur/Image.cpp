#include "Image.h"

Image::Image(int _width, int _height, int _cahnnel, unsigned char *_data)
{
    width = _width;
    height = _height;
    channel = _cahnnel;
    data = _data;
}

int Image::getWidth()
{
    return width;
}

int Image::getHeight()
{
    return height;
}

unsigned char *Image::getData()
{
    return data;
}