#include "World.h"
#include <memory>

bool World::isBlockAt(glm::ivec2 chunkPos, glm::ivec3 blockPos)
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
void World::loadChunk(glm::ivec2 chunkPos) {
    auto it = chunkMap.find(chunkPos);
    if (it == chunkMap.end()) {
        generateChunk(chunkPos);
    }
    loadedChunk.push_back(chunkPos);
}
void World::unloadChunk(glm::ivec2 chunkPos) {
    loadedChunk.erase(std::remove(loadedChunk.begin(), loadedChunk.end(), chunkPos), loadedChunk.end());
}
void World::generateChunk(glm::ivec2 chunkPos) {
    std::shared_ptr<Chunk> currentChunk = std::make_shared<Chunk>(chunkPos, noise, std::shared_ptr<ChunkObserver>(this));
    chunkMap.insert(std::make_pair(chunkPos, currentChunk));
    loadedChunk.push_back(chunkPos);
}
void World::draw(const glm::mat4& viewMatrix) {
    for (glm::ivec2 chunkPos : loadedChunk) {
        chunkMap.find(chunkPos)->second->draw(viewMatrix);
    }
}

