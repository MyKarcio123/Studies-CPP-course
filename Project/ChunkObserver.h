#pragma once
#include "glm/glm.hpp"
class ChunkObserver
{
public:
	virtual bool isBlockAt(glm::ivec2 chunkPos, glm::ivec3 blockPos) = 0;
};

