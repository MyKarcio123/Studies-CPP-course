#pragma once
#include "glm/glm.hpp"

class Block {
private:
	int id;
	bool isSolid;
	bool isOpaque;
	bool isDestroyed;
	float health;
	glm::vec2 face;
	glm::vec2 top;
	glm::vec2 side;
protected:
	Block(int id_, bool isSolid_, bool isOpaque_, float health_, glm::vec2 face_, glm::vec2 top_, glm::vec2 side_);
	Block(int id_, bool isSolid_, bool isOpaque_, float health_, glm::vec2 face_, glm::vec2 top_);
	Block(int id_, bool isSolid_, bool isOpaque_, float health_, glm::vec2 face_);
public:
	int getId() const;
	bool getIsSolid() const;
	bool getIsOpaque() const;
	bool getIsDestroyed() const;
	float getHealth() const;
	glm::vec2 getFace() const;
	glm::vec2 getTop() const;
	glm::vec2 getSide() const;
};