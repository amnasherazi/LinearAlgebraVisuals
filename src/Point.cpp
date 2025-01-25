#include "../include/Point.h"
// Point class
Point::Point(float x, float y)
{
    float vertices[] = {x, y, 0.0f};
    buffer = new Buffer(vertices, sizeof(vertices));
}

void Point::render(float size) const
{
    glPointSize(size);
    buffer->bind();
    glDrawArrays(GL_POINTS, 0, 1);
}

Point::~Point()
{
    delete buffer;
}
