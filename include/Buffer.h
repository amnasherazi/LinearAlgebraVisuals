#ifndef BUFFER_H
#define BUFFER_H

#include <GL/glew.h>

class Buffer {
public:
    Buffer(const float *vertices, unsigned int vertexSize, const unsigned int *indices = nullptr, unsigned int indexSize = 0);
    void bind() const;
    ~Buffer();

private:
    unsigned int VAO, VBO, EBO = 0;
};

#endif