#ifndef GRID_H
#define GRID_H

#include "Buffer.h"

class Grid {
public:
    Grid(int numLines, float spacing);
    void render() const;
    ~Grid();
private:
    Buffer *buffer;
    int lineCount;
};

#endif