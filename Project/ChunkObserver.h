#pragma once
#include "glm/glm.hpp"
class ChunkObserver
{
public:
	virtual bool isBlockAt(const glm::ivec2& chunkPos,const glm::ivec3& blockPos) = 0;
};

