#include "Object.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include "SOIL2.h"

Object::Object() :
    m_position(glm::vec3(0.0f)),
    m_scale(glm::vec3(1.0f)),
    m_rotation(glm::vec3(0.0)),
    m_vertexShader(GL_VERTEX_SHADER, "TextureVertex.vert"),
    m_fragmentShader(GL_FRAGMENT_SHADER, "TextureFragment.frag"),
    m_shaderProgram(glCreateProgram()),
    m_vao(0),
    m_vbo(0),
    m_ibo(0),
    m_indicesCount(0),
    m_textureSampler(0),
    viewMatrixLocation(-1)
{
    float side = 1.0f;
    float height = side * sqrt(3.0f) / 2.0f;
    atlasID = SOIL_load_OGL_texture(
        "Resources/Atlas/Terrain.png", // Œcie¿ka do pliku tekstury
        SOIL_LOAD_AUTO, // Automatyczne rozpoznanie formatu tekstury
        SOIL_CREATE_NEW_ID, // Utwórz nowe ID dla tekstury
        SOIL_FLAG_INVERT_Y // Odwróæ oœ Y (opcjonalne)
    );


    glm::vec3 vertices[] = {
        // Front face
        glm::vec3(0, 0, 0),
        glm::vec3(1, 0, 0),
        glm::vec3(1, 1, 0),
        glm::vec3(0, 1, 0),

        // Back face
        glm::vec3(1, 0, 1),
        glm::vec3(0, 0, 1),
        glm::vec3(0, 1, 1),
        glm::vec3(1, 1, 1),

        // Top face
        glm::vec3(0, 1, 0),
        glm::vec3(1, 1, 0),
        glm::vec3(1, 1, 1),
        glm::vec3(0, 1, 1),

        // Bottom face
        glm::vec3(0, 0, 0),
        glm::vec3(0, 0, 1),
        glm::vec3(1, 0, 1),
        glm::vec3(1, 0, 0),

        // Left face
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 1, 1),
        glm::vec3(0, 0, 1),

        // Right face
        glm::vec3(1, 0, 0),
        glm::vec3(1, 1, 0),
        glm::vec3(1, 1, 1),
        glm::vec3(1, 0, 1)
    };

    GLuint indices[] = {
        // Front face
        0, 1, 2,
        2, 3, 0,

        // Back face
        4, 5, 6,
        6, 7, 4,

        // Top face
        8, 9, 10,
        10, 11, 8,

        // Bottom face
        12, 13, 14,
        14, 15, 12,

        // Left face
        16, 17, 18,
        18, 19, 16,

        // Right face
        20, 21, 22,
        22, 23, 20
    };

    GLfloat uvs[] = {
        // Front face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Back face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Top face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Bottom face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Left face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Right face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };
    // Generowanie ID samplera
    glGenSamplers(1, &m_textureSampler);

    // Ustawianie wartoœci samplera
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(m_textureSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(m_textureSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glSamplerParameteri(m_textureSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(m_textureSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Przypisanie wartoœci samplera do uniform w programie shaderów
    glUseProgram(m_shaderProgram);
    glUniform1i(glGetUniformLocation(m_shaderProgram, "textureSampler"), 0);
    glUseProgram(0);

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

    glGenBuffers(1, &m_uvbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_uvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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
    m_indicesCount = sizeof(ibo);
}

void Object::renderFromVAO(const glm::mat4& viewMatrix) {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, m_position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, m_scale);

    modelMatrix = viewMatrix * modelMatrix;
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void Object::draw(const glm::mat4& viewMatrix) {
    glEnable(GL_CULL_FACE);

    glFrontFace(GL_CW);
    glUseProgram(m_shaderProgram);

    glBindVertexArray(m_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlasID);

    renderFromVAO(viewMatrix);

    glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glDisable(GL_CULL_FACE);
    glUseProgram(0);
}
