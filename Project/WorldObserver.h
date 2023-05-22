#pragma once
#include "glm/glm.hpp"
class WorldObserver
{
public:
	virtual void loadChunk(glm::ivec2 chunkPos) = 0;
	virtual void unloadChunk(glm::ivec2 chunkPos) = 0;
};
