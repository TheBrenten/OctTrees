
#include <iostream>
#include "Renderer.h"
#include "Model.h"

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << "): " << function <<
			" " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
	shader.bind();
	va.bind();
	ib.bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));

}

void Renderer::draw(const Model& mdl) const {
    mdl.shader->bind();
    mdl.va->bind();
    mdl.ib->bind();
    glm::mat4 model = mdl.matPos * mdl.matRot * mdl.matScale;
    glm::mat4 mvp = proj * view * model;
    mdl.shader->bind();
    if (mdl.texture) {
        mdl.texture->bind();
        mdl.shader->setUniform1i("u_Texture", 0);
    }
    mdl.shader->setUniformMat4f("u_MVP", mvp);
    GLCall(glDrawElements(GL_TRIANGLES, mdl.ib->getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::clear() {
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
