#pragma once
#include "Block.h"
#include "Dirt.h"
#include "Air.h"
#include "SOIL2.h"
#define sharedBlock std::shared_ptr<Block>
/*extern GLuint atlasID;
int main()
{
    atlasID = SOIL_load_OGL_texture(
        "texture.jpg", // �cie�ka do pliku tekstury
        SOIL_LOAD_AUTO, // Automatyczne rozpoznanie formatu tekstury
        SOIL_CREATE_NEW_ID, // Utw�rz nowe ID dla tekstury
        SOIL_FLAG_INVERT_Y // Odwr�� o� Y (opcjonalne)
    );

    if (atlasID == 0)
    {
        std::cerr << "Nie uda�o si� wczyta� tekstury!" << std::endl;
        return -1;
    }
}*/