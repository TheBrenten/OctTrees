#pragma once
#include <string>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
class Mesh {
public:
    VertexBuffer* vb;
    IndexBuffer* ib;
    VertexArray va;
    float* vertices;
    unsigned int* indices;
    // obj loader
    Mesh(std::string filepath);
    ~Mesh();
};

