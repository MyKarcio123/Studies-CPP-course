#include "Game.h"
#include "World.h"
Game::Game(GLFWwindow* window_) : window(window_) {
    cameraObject = std::make_shared<Camera>();
}
void Game::start() {
    srand(std::time(0));
    gameObjects.push_back(cameraObject);
    std::shared_ptr<GameObject> simpleObject = std::make_shared<Object>();
    std::shared_ptr<World> world = std::make_shared<World>();
    gameObjects.push_back(world);
    renderObjects.push_back(world);
    world->generateChunk(glm::ivec2{ 0,0 });
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