#pragma once
#include "Object.h"
#include <memory>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include <unordered_map>
#include "glm/glm.hpp"
#include "Chunk.h"
#include "ChunkObserver.h"
#include <FastNoiseLite.h>
#include "WorldObserver.h"
class World : public Object, public ChunkObserver, public WorldObserver
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
	bool isBlockAt(const glm::ivec2& chunkPos,const glm::ivec3& blockPos) override;
	void generateChunk(const glm::ivec2& chunkPos,meshFlag flag = meshFlag::MAKEMESH);
	int getHeight(const glm::ivec2& chunkPos, const glm::vec2& blockCoords);
};

