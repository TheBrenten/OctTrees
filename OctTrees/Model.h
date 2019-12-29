#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

class Model {
public:
    glm::mat4 matPos;
    glm::mat4 matRot;
    glm::mat4 matScale;

    IndexBuffer* ib;
    VertexArray* va;
    Shader* shader;
    Texture* texture;

    Model(IndexBuffer& ib, VertexArray& va, Shader& shader, Texture* texture = nullptr);
    //~Model();

    void setPosition(glm::vec3 pos);
    void setRotation(glm::vec3 rot); // X/Y/Z in degrees
    void setScale(float val);
    void setScale(glm::vec3 scale);

};

