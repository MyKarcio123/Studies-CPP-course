#pragma once
#include "Block.h"
#include "SOIL2.h"
#define sharedBlock std::shared_ptr<Block>
class Air : public Block
{
public:
    Air() :Block(0, false, true, 0.0f, glm::vec2(0, 0)) {}
};
class Dirt : public Block
{
public:
    Dirt() :Block(1, true, false, 100, glm::vec2(2, 0)) {}
};
class Grass : public Block
{
public:
    Grass() :Block(2, true, false, 100, glm::vec2(3, 0), glm::vec2(0,0),glm::vec2(3,0),glm::vec2(2,0)) {}
};
/*extern GLuint atlasID;
int main()
{
    atlasID = SOIL_load_OGL_texture(
        "texture.jpg", // Œcie¿ka do pliku tekstury
        SOIL_LOAD_AUTO, // Automatyczne rozpoznanie formatu tekstury
        SOIL_CREATE_NEW_ID, // Utwórz nowe ID dla tekstury
        SOIL_FLAG_INVERT_Y // Odwróæ oœ Y (opcjonalne)
    );

    if (atlasID == 0)
    {
        std::cerr << "Nie uda³o siê wczytaæ tekstury!" << std::endl;
        return -1;
    }
}*/