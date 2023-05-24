#pragma once
class GameObject {
protected:
    glm::vec3 m_position;
public:
    GameObject(glm::vec3 position = glm::vec3(0.0f)) : m_position{ position } {}
    virtual void start() = 0;
    virtual void update() = 0;
    virtual ~GameObject() {};
};