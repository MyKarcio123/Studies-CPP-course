#include "Object.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include "SOIL2.h"

Object::Object() :
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

    vertices.push_back(glm::vec3(0, 0, 0));

    vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f)); // 0
    vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));  // 1
    vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));   // 2
    vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));  // 3
    vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));  // 4
    vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));   // 5
    vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));    // 6
    vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));   // 7

    // Indeksy wierzcho³ków dla tworzenia trójk¹tów


    // Wspó³rzêdne tekstur UV
    uvs.push_back(glm::vec2(0.0f, 0.0f)); // 0
    uvs.push_back(glm::vec2(1.0f, 0.0f)); // 1
    uvs.push_back(glm::vec2(1.0f, 1.0f)); // 2
    uvs.push_back(glm::vec2(0.0f, 1.0f)); // 3
    uvs.push_back(glm::vec2(1.0f, 0.0f)); // 4
    uvs.push_back(glm::vec2(0.0f, 0.0f)); // 5
    uvs.push_back(glm::vec2(0.0f, 1.0f)); // 6
    uvs.push_back(glm::vec2(1.0f, 1.0f)); // 7
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

    m_indicesCount = indicies.size();

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_uvbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_uvbo);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(GLuint), indicies.data(), GL_STATIC_DRAW);

    m_indicesCount = indicies.size();

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
