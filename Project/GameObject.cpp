#include "GameObject.h"

void GameObject::changePosition(glm::vec3 position)
{
	this->m_position += position;
}
