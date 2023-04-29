#include "Object.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

Object::Object() :
    m_position(glm::vec3(0.0f)),
    m_scale(glm::vec3(1.0f)),
    m_rotation(glm::vec3(0.0)),
    m_vertexShader(GL_VERTEX_SHADER, "DefaultVertex.vert"),
    m_fragmentShader(GL_FRAGMENT_SHADER, "DefaultFragment.frag"),
    m_shaderProgram(glCreateProgram()),
    m_vao(0),
    m_vbo(0),
    m_ibo(0),
    m_indicesCount(0)
{
    float side = 1.0f;
    float height = side * sqrt(3.0f) / 2.0f;
    glm::vec3 vertices[] = {
            glm::vec3(-side / 2.0f, -height / 3.0f, 0.0f),
            glm::vec3(side / 2.0f, -height / 3.0f, 0.0f),
            glm::vec3(0.0f, 2.0f * height / 3.0f, 0.0f),
            glm::vec3(side / 2.0f, 2.0f * height / 3.0f, 0.0f)
    };

    GLuint indices[] = {
        0, 1, 2, 2, 3, 1
    };

    m_indicesCount = sizeof(indices) / sizeof(GLuint);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_fragmentShader.compile();
    m_vertexShader.compile();
    glAttachShader(m_shaderProgram, m_fragmentShader.getID());
    glAttachShader(m_shaderProgram, m_vertexShader.getID());
    glLinkProgram(m_shaderProgram);

    viewMatrixLocation = glGetUniformLocation(m_shaderProgram, "viewMatrix");
}

Object::~Object() {}

void Object::setPosition(const glm::vec3& position) {
    m_position = position;
}

void Object::setScale(const glm::vec3& scale) {
    m_scale = scale;
}


void Object::setRotation(const glm::vec3& rotation) {
    m_rotation = rotation;
}

void Object::setVertexShader(const std::string& filename) {
    m_vertexShader = Shader(GL_VERTEX_SHADER, filename);    
    m_vertexShader.compile();
    glAttachShader(m_shaderProgram, m_vertexShader.getID());
    glLinkProgram(m_shaderProgram);
}

void Object::setFragmentShader(const std::string& filename) {
    m_fragmentShader = Shader(GL_FRAGMENT_SHADER, filename);
    m_fragmentShader.compile();
    glAttachShader(m_shaderProgram, m_fragmentShader.getID());
    glLinkProgram(m_shaderProgram);
}

void Object::setVAO(GLuint vao) {
    m_vao = vao;
}

void Object::setVBO(GLuint vbo) {
    m_vbo = vbo;
}

void Object::setIBO(GLuint ibo) {
	m_ibo = ibo;
	m_indicesCount = sizeof(m_ibo) / sizeof(GLuint);
}

void Object::draw(const glm::mat4& viewMatrix) {

    glUseProgram(m_shaderProgram);

    glBindVertexArray(m_vao);

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, m_position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, m_scale);

    modelMatrix = viewMatrix * modelMatrix;
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);


    glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}