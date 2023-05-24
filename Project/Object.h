#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "OpenGL/Shader.h"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include "GameObject.h"
class Object : public GameObject {
public:
	Object();
	virtual ~Object();

	void setPosition(const glm::vec3& position);
	void setScale(const glm::vec3& scale);
	void setRotation(const glm::vec3& rotation);

	void setVertexShader(const std::string& filename);
	void setFragmentShader(const std::string& filename);

	void setVAO(GLuint vao);
	void setVBO(GLuint vbo);
	void setIBO(GLuint ibo);

	virtual void draw(const glm::mat4& viewMatrix);
	void start() override {};
	void update() override {};
protected:
	void renderFromVAO(const glm::mat4& viewMatrix);
	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::vec3 m_rotation;

	GLuint m_shaderProgram;

	Shader m_vertexShader;
	Shader m_fragmentShader;

	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_uvbo;
	GLuint m_ibo;
	GLuint m_indicesCount;
	GLuint atlasID;
	GLuint m_textureSampler;
	GLuint viewMatrixLocation;

};