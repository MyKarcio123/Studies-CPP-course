#pragma once
#include "glm/glm.hpp"
class PlayerWolrdObserver
{
public:
	virtual void playerWentOutOfChunk(const glm::vec3& playerPos) = 0;
};
