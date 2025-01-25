#include <vector>
#include "../include/Grid.h"

Grid::Grid(int numLines, float spacing)
{
    std::vector<float> vertices;
    for (int i = -numLines; i <= numLines; ++i)
    {
        float pos = i * spacing;
        vertices.insert(vertices.end(), {-1.0f, pos, 0.0f, 1.0f, pos, 0.0f});
        vertices.insert(vertices.end(), {pos, -1.0f, 0.0f, pos, 1.0f, 0.0f});
    }
    buffer = new Buffer(vertices.data(), vertices.size() * sizeof(float));
    lineCount = (numLines * 2 + 1) * 4;
}

void Grid::render() const
{
    buffer->bind();
    glDrawArrays(GL_LINES, 0, lineCount);
}

Grid::~Grid()
{
    delete buffer;
}
