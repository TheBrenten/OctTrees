#include "Model.h"

Model::Model(IndexBuffer& ib, VertexArray& va, Shader& shader, Texture* texture) {
    this->ib = &ib;
    this->va = &va;
    this->shader = &shader;
    this->texture = texture;
}

void Model::setPosition(glm::vec3 pos) {
    matPos = glm::translate(glm::mat4(1.0f), pos);
}

void Model::setRotation(glm::vec3 rot) {
    matRot = glm::mat4(
        glm::quat(
            glm::vec3(glm::radians(rot.x), glm::radians(rot.y), glm::radians(rot.z))
        )
    );
}

void Model::setScale(float val) {
    matScale = glm::scale(glm::mat4(1.0f), glm::vec3(val, val, val));
}

void Model::setScale(glm::vec3 scale) {
    matScale = glm::scale(glm::mat4(1.0f), glm::vec3(scale));
}
