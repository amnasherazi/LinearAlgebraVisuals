#include "../include/Square.h"
Square::Square()
{
    float vertices[] = {
        -0.05f, -0.05f, 0.0f,
        0.05f, -0.05f, 0.0f,
        0.05f, 0.05f, 0.0f,
        -0.05f, 0.05f, 0.0f};
    unsigned int indices[] = {0, 1, 2, 0, 2, 3};
    buffer = new Buffer(vertices, sizeof(vertices), indices, sizeof(indices));
}

void Square::render() const
{
    buffer->bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

Square::~Square()
{
    delete buffer;
}
