#ifndef POINT_H
#define POINT_H
#include "Buffer.h"

class Point
{
public:
    Point(float x, float y);
    void render(float size) const;
    ~Point();

private:
    Buffer *buffer;
};

#endif