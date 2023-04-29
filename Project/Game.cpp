#include "Game.h"
Game::Game(GLFWwindow* window_) : window(window_) {
    cameraObject = std::make_shared<Camera>();
}
void Game::start() {
    gameObjects.push_back(cameraObject);
    std::shared_ptr<GameObject> simpleObject = std::make_shared<Object>();
    gameObjects.push_back(simpleObject);
    renderObjects.push_back(std::static_pointer_cast<Object>(simpleObject));

    for (auto& gameObject : gameObjects) {
        gameObject->start();
    }
}
void Game::update() {
    for (auto& gameObject : gameObjects) {
        gameObject->update();
    }
}
void Game::render() {
    for (auto& renderObjects : renderObjects) {
        renderObjects->draw(cameraObject->GetViewMatrix());
    }
}