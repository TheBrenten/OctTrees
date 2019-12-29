#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
class Model;

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))
void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
private:
	
public:
    glm::mat4 proj;
    glm::mat4 view;
	void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void draw(const Model& mdl) const;
	void clear();
};
