#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "Constants.h"

#include <vector>
#include <GL/glew.h>
#include "GameObject.h"
#include "Input.h"
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

//Default Values
constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f;

class Camera : public GameObject
{
    //camera Properties
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    //euler Angle
    float yaw;
    float pitch;

    //camera Options
    float movementSpeed;
    float mouseSensitivity;
    float zoom;
public:
    // constructor with vectors
    Camera(glm::vec3 position_ = glm::vec3(Constants::chunkWidth/2, Constants::chunkHeight+2, Constants::chunkWidth / 2), glm::vec3 up_ = glm::vec3(0.0f, 1.0f, 0.0f), float yaw_ = YAW, float pitch_ = PITCH);
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    glm::mat4 GetViewMatrix() const;
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);
    void setPos(glm::vec3 pos_);
    void start() override {};
    void update() override;
private:
    void updateCameraVectors();
    
};