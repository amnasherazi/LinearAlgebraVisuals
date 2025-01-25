#ifndef SQUARE_H
#define SQUARE_H

#include "Buffer.h"

class Square
{
public:
    Square();
    void render() const;
    ~Square();

private:
    Buffer *buffer;
};

#endif