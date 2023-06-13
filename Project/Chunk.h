#pragma once
#include <unordered_map>
#include <array>
#include <mutex>
#include <thread>
#include <condition_variable>
#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include "Blocks/BlocksData.h"
#include <memory>
#include "Object.h"
#include "ChunkObserver.h"
#include "Constants.h"
#include <FastNoiseLite.h>

enum class meshFlag {
	MAKEMESH,
	DONTMAKEMESH,
};

class Chunk : public Object
{
private:
	int counter = 0;
	glm::ivec2 coords;
	std::array<std::array<std::array<sharedBlock, Constants::chunkWidth>, Constants::chunkHeight>, Constants::chunkWidth> blocksMap;
	void updateMesh();
	std::shared_ptr<ChunkObserver> chunkObserver;
	glm::vec2 chunkPosition;
	void updateModel();
	void makeFace(const glm::ivec3 pos, const glm::ivec3 face);
	bool active = true;
	void makeSingleMesh();
public:
	Chunk(glm::ivec2 coords,const FastNoiseLite& noise, const std::shared_ptr<ChunkObserver>& observer, meshFlag flag = meshFlag::MAKEMESH);
	void makeMesh();
	void addBlock(glm::ivec3 pos, sharedBlock block);
	void addBlock(glm::ivec3 pos, int id);
	void removeBlock(glm::ivec3 pos);
	sharedBlock getBlock(glm::ivec3 pos);
	bool isBlock(glm::ivec3 pos);
	void processBlock(const glm::ivec3 pos);
	int getHeight(const glm::ivec2& blockCoords);
	bool isActive();
	void setActive(bool active);
};

