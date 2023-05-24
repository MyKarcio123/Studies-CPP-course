#pragma once
#include <memory>
#include <unordered_map>
#include <FastNoiseLite.h>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include "glm/glm.hpp"

#include "Object.h"
#include "Chunk.h"

#include "ChunkObserver.h"
#include "WorldObserver.h"
#include "PlayerWolrdObserver.h"

class World : public Object, public ChunkObserver, public WorldObserver, public PlayerWolrdObserver
{
private:
	std::unordered_map<glm::ivec2, std::shared_ptr<Chunk>> chunkMap;
	std::vector<glm::ivec2> loadedChunk;
	FastNoiseLite noise;
public:
	World();
	void generateSpawn();
	void loadChunk(const glm::ivec2& chunkPos) override;
	void unloadChunk(const glm::ivec2& chunkPos) override;
	void draw(const glm::mat4& viewMatrix) override;
	void playerWentOutOfChunk(const glm::vec3& playerPos) override;
	bool isBlockAt(const glm::ivec2& chunkPos,const glm::ivec3& blockPos) override;
	void generateChunk(const glm::ivec2& chunkPos,meshFlag flag = meshFlag::MAKEMESH);
	int getHeight(const glm::ivec2& chunkPos, const glm::vec2& blockCoords);
};

