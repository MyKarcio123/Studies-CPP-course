#pragma once
#include <unordered_map>
#include "glm/glm.hpp"
#include "Blocks/BlocksData.h"
#include <memory>
#include "Object.h"

class Chunk : Object
{
private:
	std::unordered_map<glm::vec3, sharedBlock> blocks;
public:
	void addBlock(glm::vec3, sharedBlock block);
	void addBlock(glm::vec3, int id);
	void removeBlock(glm::vec3);
	sharedBlock getBlock(glm::vec3);
	bool isBlock(glm::vec3);
	void draw(const glm::mat4& viewMatrix) override;
};

