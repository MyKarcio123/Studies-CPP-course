#include "Camera.h"
#include "Time.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

Camera::Camera(glm::vec3 position_, glm::vec3 up_, float yaw_, float pitch_) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM), worldUp(up_), yaw(yaw_), pitch(pitch_), GameObject(position_){
	updateCameraVectors();
}
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw_, float pitch_) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM), worldUp(glm::vec3(upX, upY, upZ)), yaw(yaw_), pitch(pitch_), GameObject(glm::vec3{ posX ,posY,posZ }) {
	updateCameraVectors();
}
glm::mat4 Camera::GetViewMatrix() const{
	return glm::perspective(glm::radians(zoom), (float)800 / (float)600, 0.1f, 100.0f) * glm::lookAt(m_position, m_position + front, up);
}
void Camera::setPos(glm::vec3 pos_) { m_position = pos_; }
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        m_position += front * velocity;
    if (direction == BACKWARD)
        m_position -= front * velocity;
    if (direction == LEFT)
        m_position -= right * velocity;
    if (direction == RIGHT)
        m_position += right * velocity;
}
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}
void Camera::ProcessMouseScroll(float yoffset)
{
    zoom -= (float)yoffset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}
void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front_;
    front_.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front_.y = sin(glm::radians(pitch));
    front_.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front_);

    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
void Camera::update() {
    ProcessMouseMovement(Input::GetAxis(VERTICAL), Input::GetAxis(HORIZONTAL), 1);
    int keys[4] = { GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D };
    for (int i = 0; i < 4;++i) {
        if (Input::GetKey(keys[i])) {
            ProcessKeyboard(static_cast<Camera_Movement>(i), Time::getDeltaTime());
        }
    }
}