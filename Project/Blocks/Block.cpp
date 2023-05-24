#include "Block.h"
Block::Block() :
	id(0),
	isSolid(false),
	isOpaque(true),
	isDestroyed(true),
	health(0.0f),
	frontSide(glm::vec2(0,0)),
	topSide(glm::vec2(0,0)),
	sideWall(glm::vec2(0,0))
{}
Block::Block(int id_, bool isSolid_, bool isOpaque_, float health_, glm::vec2 face_, glm::vec2 top_, glm::vec2 side_) : id(id_), isSolid(isSolid_), isOpaque(isOpaque_), health(health_), frontSide(face_), topSide(top_), sideWall(side_),isDestroyed(false){
	visibleSides = {
	{side::FRONT, true},
	{side::BACK, true},
	{side::LEFT, true},
	{side::RIGHT, true},
	{side::BOTTOM, true},
	{side::TOP, true}
	};
}
Block::Block(int id_, bool isSolid_, bool isOpaque_, float health_, glm::vec2 face_, glm::vec2 top_) : Block(id_, isSolid_, isOpaque_, health_, face_, top_, top_){

}
Block::Block(int id_, bool isSolid_, bool isOpaque_, float health_, glm::vec2 face_) : Block(id_, isSolid_, isOpaque_, health_, face_, face_, face_) {

}
int Block::getId() const {
	return id;
}

bool Block::getIsSolid() const {
	return isSolid;
}

bool Block::getIsOpaque() const {
	return isOpaque;
}

bool Block::getIsDestroyed() const {
	return isDestroyed;
}

float Block::getHealth() const {
	return health;
}

glm::vec2 Block::getFront() const {
	return frontSide;
}

glm::vec2 Block::getTop() const {
	return topSide;
}

glm::vec2 Block::getSide() const {
	return sideWall;
}

void Block::setSideVisibility(const side& sideType, bool visibility)
{
	visibleSides[sideType] = visibility;
}

bool Block::isSideVisible(const side& sideType)
{
	return visibleSides[sideType];
}
