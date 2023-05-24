#include "Game.h"
#include "World.h"
Game::Game(GLFWwindow* window_) : window(window_) {
    cameraObject = std::make_shared<Camera>();
}
void Game::start() {
    srand(std::time(0));
    gameObjects.push_back(cameraObject);
    std::shared_ptr<World> world = std::make_shared<World>();
    gameObjects.push_back(world);
    renderObjects.push_back(world);
    world->generateSpawn();
    cameraObject->setPos(glm::ivec3{ glm::vec3(Constants::chunkWidth / 2, world->getHeight(glm::ivec2{ 0,0 },glm::vec2{Constants::chunkWidth / 2,Constants::chunkWidth / 2}) + 3, Constants::chunkWidth / 2) });
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