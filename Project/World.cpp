#include "World.h"
#include <memory>

bool World::isBlockAt(const glm::ivec2& chunkPos, const glm::ivec3& blockPos)
{
    auto it = chunkMap.find(chunkPos);
    if (it == chunkMap.end()) {
        return false;
    }
    return it->second->isBlock(blockPos);
}
World::World():noise{FastNoiseLite(rand())} {
    noise.SetNoiseType(noise.NoiseType_Perlin);
    noise.SetFractalType(noise.FractalType_FBm);
    noise.SetFractalGain(0.5f);
    noise.SetFractalLacunarity(1.6f);
    noise.SetFractalOctaves(8);
    noise.SetFractalPingPongStrength(2.0f);
    noise.SetFractalWeightedStrength(0.0f);
    noise.SetFrequency(0.02f);
}
void World::loadChunk(const glm::ivec2& chunkPos) {
    auto it = chunkMap.find(chunkPos);
    if (it == chunkMap.end()) {
        generateChunk(chunkPos);
    }
    loadedChunk.push_back(chunkPos);
}
void World::unloadChunk(const glm::ivec2& chunkPos) {
    loadedChunk.erase(std::remove(loadedChunk.begin(), loadedChunk.end(), chunkPos), loadedChunk.end());
}
void World::generateChunk(const glm::ivec2& chunkPos, meshFlag flag) {
    std::shared_ptr<Chunk> currentChunk = std::make_shared<Chunk>(chunkPos, noise, std::shared_ptr<ChunkObserver>(this),flag);
    chunkMap.insert(std::make_pair(chunkPos, currentChunk));
    loadedChunk.push_back(chunkPos);
}
void World::draw(const glm::mat4& viewMatrix) {
    for (glm::ivec2 chunkPos : loadedChunk) {
        chunkMap.find(chunkPos)->second->draw(viewMatrix);
    }
}
int World::getHeight(const glm::ivec2& chunkPos, const glm::vec2& blockCoords) {
    auto it = chunkMap.find(chunkPos);
    if (it == chunkMap.end()) {
        return false;
    }
    return it->second->getHeight(glm::ivec2{ (int)blockCoords.x,(int)blockCoords.y });
}
void World::generateSpawn() {
    for (int i = -Constants::renderDistance; i <= Constants::renderDistance; ++i) {
        for (int j = -Constants::renderDistance; j <= Constants::renderDistance; ++j) {
            generateChunk(glm::ivec2{ i,j }, meshFlag::DONTMAKEMESH);
        }
    }
    for (auto it = chunkMap.begin(); it != chunkMap.end(); ++it) {
        it->second->makeMesh();
    }
}
void World::playerWentOutOfChunk(const glm::vec3& playerPos) {
    glm::ivec2 currentPlayerCord{ (int)(playerPos.x / Constants::chunkWidth),(int)(playerPos.z / Constants::chunkWidth) };
    for (auto it = loadedChunk.begin(); it != loadedChunk.end();) {
        glm::ivec2 diffrence{ abs(currentPlayerCord.x - (*it).x),abs(currentPlayerCord.y - (*it).y) };
        if (diffrence.x <= Constants::renderDistance && diffrence.y <= Constants::renderDistance) {
            continue;
        }
        unloadChunk((*it));
        
        ++it;
    }
}
