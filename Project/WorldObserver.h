#pragma once
#include "glm/glm.hpp"
class WorldObserver
{
public:
	virtual void loadChunk(const glm::ivec2& chunkPos) = 0;
	virtual void unloadChunk(const glm::ivec2& chunkPos) = 0;
};
