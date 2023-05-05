#include "Block.h"
Block::Block(int id_, bool isSolid_, bool isOpaque_, float health_, glm::vec2 face_, glm::vec2 top_, glm::vec2 side_) : id(id_), isSolid(isSolid_), isOpaque(isOpaque_), health(health_), face(face_), top(top_), side(side_),isDestroyed(false){

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

glm::vec2 Block::getFace() const {
	return face;
}

glm::vec2 Block::getTop() const {
	return top;
}

glm::vec2 Block::getSide() const {
	return side;
}
