#include "World.h"
#include <memory>
#include <unordered_set>

bool World::isBlockAt(const glm::ivec2& chunkPos, const glm::ivec3& blockPos)
{
    auto it = chunkMap.find(chunkPos);
    if (it == chunkMap.end()) {
        return false;
    }
    if (it != chunkMap.end() && it->second && it->second->isActive()) {
        return it->second->isBlock(blockPos);
    }
    return false;
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
        generateChunk(chunkPos, meshFlag::DONTMAKEMESH);
    }
    else {
        chunkMap.find(chunkPos)->second->setActive(true);
    }
    loadedChunk.push_back(chunkPos);
}
void World::unloadChunk(const glm::ivec2& chunkPos) {
    chunkMap.find(chunkPos)->second->setActive(false);
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
            chunkMap.find(glm::ivec2{ i,j })->second->fillChunkWithBlocks();
        }
    }
    for (auto it = chunkMap.begin(); it != chunkMap.end(); ++it) {
        makeAsyncMesh(it->second);
    }
}
void World::chunkMenager() {
    while (true) {
        if (isChunkToMakeMesh) {
            while (!chunkToMakeMesh.empty()) {
                std::unique_lock<std::mutex> lock(makeMesh);
                std::shared_ptr<Chunk> currentChunk = chunkToMakeMesh.front();
                chunkToMakeMesh.pop();
                lock.unlock();
                if (!currentChunk->isFiled()) {
                    currentChunk->fillChunkWithBlocks();
                }
                chunkFilledWithData.push(currentChunk);
            }
            while (!chunkFilledWithData.empty()) {
                std::shared_ptr<Chunk> currentChunk = chunkFilledWithData.front();
                chunkFilledWithData.pop();
                currentChunk->makeMesh();
                std::unique_lock<std::mutex> secondLock(updateModel);
                chunkToUpdateModel.push(currentChunk);
                isChunkToUpdateModel = true;
                secondLock.unlock();
            }
            std::unique_lock<std::mutex> lock(makeMesh);
            isChunkToMakeMesh = false;
            lock.unlock();
        }
    }
}
void World::start() {
    std::thread chunkLoader(&World::chunkMenager, this);
    chunkLoader.detach();
}
void World::update() {
    if (isChunkToUpdateModel) {
        while (!chunkToUpdateModel.empty()) {
            std::unique_lock<std::mutex> lock(updateModel);
            std::shared_ptr<Chunk> currentChunk = chunkToUpdateModel.front();
            chunkToUpdateModel.pop();
            lock.unlock();
            currentChunk->updateModel();
        }
        std::unique_lock<std::mutex> lock(updateModel);
        isChunkToUpdateModel = false;
        lock.unlock();
    }
}
void World::makeAsyncMesh(std::shared_ptr<Chunk> chunk)
{
    std::unique_lock<std::mutex> lock(makeMesh);
    chunkToMakeMesh.push(chunk);
    isChunkToMakeMesh = true;
    lock.unlock();
}
void World::playerWentOutOfChunk(const glm::vec3& playerPos) {

    glm::ivec2 currentPlayerCord{ static_cast<int>(std::floor(playerPos.x / Constants::chunkWidth)),static_cast<int>(std::floor(playerPos.z / Constants::chunkWidth)) };
    if (currentPlayerCord == currentChunk)
        return;
    glm::ivec2 dirVector = currentPlayerCord - currentChunk;
    currentChunk = currentPlayerCord;
    std::vector<glm::ivec2> meshToRepair;
    //Delete chunks out of render distance
    auto it = loadedChunk.begin();
    while (it != loadedChunk.end()) {
        glm::ivec2 difference{ abs(currentChunk.x - (*it).x), abs(currentChunk.y - (*it).y) };
        if (difference.x <= Constants::renderDistance && difference.y <= Constants::renderDistance) {
            ++it;
        }
        else {
            meshToRepair.push_back(glm::ivec2{ (*it) + dirVector });
            chunkMap.find(*it)->second->setActive(false);
            it = loadedChunk.erase(it);
        }
    }
    //Make new chunks
    glm::ivec2 newChunk;
    if (dirVector.x != 0) {
        newChunk = { currentChunk.x + dirVector.x * Constants::renderDistance,currentChunk.y };
        for (int i = -Constants::renderDistance; i <= Constants::renderDistance; ++i) {
            loadChunk(glm::ivec2{newChunk.x,newChunk.y+i});
            meshToRepair.push_back({ newChunk.x,newChunk.y + i });
            loadChunk(glm::ivec2{newChunk.x - 1,newChunk.y+i});
            meshToRepair.push_back({ newChunk.x - 1,newChunk.y + i });
        }
    }
    if (dirVector.y != 0) {
        newChunk = { currentChunk.x + dirVector.x,currentChunk.y + dirVector.y * Constants::renderDistance };
        for (int i = -Constants::renderDistance; i <= Constants::renderDistance; ++i) {
            loadChunk(glm::ivec2{ newChunk.x + i,newChunk.y });
            meshToRepair.push_back({ newChunk.x + i,newChunk.y });
            loadChunk(glm::ivec2{ newChunk.x + i,newChunk.y - 1 });
            meshToRepair.push_back({ newChunk.x + i,newChunk.y - 1 });
        }
    }
    //Repair mesh
    for (glm::ivec2 mesh : meshToRepair) {
        auto it = chunkMap.find(mesh);
        if (it != chunkMap.end()) {
            makeAsyncMesh(it->second);
        }
        else {
            continue;
        }
    }
}
