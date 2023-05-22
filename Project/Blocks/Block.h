#pragma once
#include "glm/glm.hpp"
#include <unordered_map>
enum class side{
	FRONT,
	RIGHT,
	BACK,
	LEFT,
	BOTTOM,
	TOP
};
class Block {
private:
	int id;
	bool isSolid;
	bool isOpaque;
	bool isDestroyed;
	float health;
	glm::vec2 frontSide;
	glm::vec2 topSide;
	glm::vec2 sideWall;
	std::unordered_map<side, bool> visibleSides;
protected:
	Block(int id_, bool isSolid_, bool isOpaque_, float health_, glm::vec2 face_, glm::vec2 top_, glm::vec2 side_);
	Block(int id_, bool isSolid_, bool isOpaque_, float health_, glm::vec2 face_, glm::vec2 top_);
	Block(int id_, bool isSolid_, bool isOpaque_, float health_, glm::vec2 face_);
public:
	Block();
	int getId() const;
	bool getIsSolid() const;
	bool getIsOpaque() const;
	bool getIsDestroyed() const;
	float getHealth() const;
	glm::vec2 getFace() const;
	glm::vec2 getTop() const;
	glm::vec2 getSide() const;
	void setSideVisibility(const side& sideType, bool visibility);
	bool isSideVisible(const side& sideType);
};