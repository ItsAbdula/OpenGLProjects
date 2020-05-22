#pragma once
enum ImageType
{
    REPEAT,
    CLAMP
};

class Image
{
private:
    int width;
    int height;
    int channel;

    unsigned char *data;

public:
    Image(int _width, int _height, int _cahnnel, unsigned char *_data);

    int getWidth();
    int getHeight();
    unsigned char *getData();
};
