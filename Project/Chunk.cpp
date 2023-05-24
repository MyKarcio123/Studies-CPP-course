#include "Chunk.h"
#include <vector>
Chunk::Chunk(glm::ivec2 chunkCoords, const FastNoiseLite& noise, const std::shared_ptr<ChunkObserver>& observer,meshFlag flag) : Object(), coords{ chunkCoords }, chunkObserver{observer} {
    this->setPosition(glm::vec3{ chunkCoords.x * Constants::chunkWidth,0,chunkCoords.y * Constants::chunkWidth });
    for (int x = 0; x < Constants::chunkWidth; ++x) {
        for (int z = 0; z < Constants::chunkWidth; ++z) {
            float value = noise.GetNoise((float)x + chunkCoords.x*Constants::chunkWidth,(float)z + chunkCoords.y * Constants::chunkWidth);
            int height = (value + 1) / 2 * Constants::chunkHeight;
            for (int y = 0; y < height; ++y) {
                if (y + 1 == height) {
                    blocksMap[x][y][z] = std::make_shared<Grass>();
                    continue;
                }
                blocksMap[x][y][z]= std::make_shared<Dirt>();
            }
            for (int y = height; y < Constants::chunkHeight; ++y) {
                blocksMap[x][y][z] = std::make_shared<Air>();
            }
        }
    }
    if(flag==meshFlag::MAKEMESH)    makeMesh();
}
void Chunk::addBlock(glm::ivec3 pos, sharedBlock block)
{
    blocksMap[pos.x][pos.y][pos.z] = block;
    makeMesh();
}

void Chunk::removeBlock(glm::ivec3 pos)
{
    blocksMap[pos.x][pos.y][pos.z] = std::make_shared<Air>();
    makeMesh();
}

sharedBlock Chunk::getBlock(glm::ivec3 pos)
{
    return blocksMap[pos.x][pos.y][pos.z];
}

bool Chunk::isBlock(glm::ivec3 pos)
{
    return blocksMap[pos.x][pos.y][pos.z]->getId() != 0;
}
void Chunk::makeFace(const glm::ivec3 pos,const glm::ivec3 face) {
    int size = vertices.size();
    sharedBlock currentBlock = getBlock(pos);
    glm::vec2 offset = currentBlock->getFront();
    if (face == glm::ivec3{ 0, 0, 1 }) {
        currentBlock->setSideVisibility(side::RIGHT, true);
        vertices.push_back(pos + glm::ivec3{ 1, 0, 1 });
        vertices.push_back(pos + glm::ivec3{ 1, -1, 1 });
        vertices.push_back(pos + glm::ivec3{ 0, -1, 1 });
        vertices.push_back(pos + glm::ivec3{ 0, 0, 1 });
    }
    else if (face == glm::ivec3{ 0, 0, -1 }) {
        currentBlock->setSideVisibility(side::LEFT, true);
        vertices.push_back(pos);
        vertices.push_back(pos + glm::ivec3{ 0, -1, 0 });
        vertices.push_back(pos + glm::ivec3{ 1, -1, 0 });
        vertices.push_back(pos + glm::ivec3{ 1, 0, 0 });
    }
    else if (face == glm::ivec3{ 0, 1, 0 }) { //topface
        offset = currentBlock->getTop();
        currentBlock->setSideVisibility(side::TOP, true);
        vertices.push_back(pos + glm::ivec3{ 1, 0, 1 });
        vertices.push_back(pos + glm::ivec3{ 0, 0, 1 });
        vertices.push_back(pos);
        vertices.push_back(pos + glm::ivec3{ 1, 0, 0 });
    }
    else if (face == glm::ivec3{ 0, -1, 0 }) {
        currentBlock->setSideVisibility(side::BOTTOM, true);
        vertices.push_back(pos + glm::ivec3{ 0, -1, 1 });
        vertices.push_back(pos + glm::ivec3{ 1, -1, 1 });
        vertices.push_back(pos + glm::ivec3{ 1, -1, 0 });
        vertices.push_back(pos + glm::ivec3{ 0, -1, 0 });
    }
    else if (face == glm::ivec3{ 1, 0, 0 }) { //front face
        offset = currentBlock->getFront();
        currentBlock->setSideVisibility(side::FRONT, true);
        vertices.push_back(pos + glm::ivec3{ 1, 0, 0 });
        vertices.push_back(pos + glm::ivec3{ 1, -1, 0 });
        vertices.push_back(pos + glm::ivec3{ 1, -1, 1 });
        vertices.push_back(pos + glm::ivec3{ 1, 0, 1 });
    }
    else{
        currentBlock->setSideVisibility(side::BACK, true);
        vertices.push_back(pos + glm::ivec3{ 0, 0, 1 });
        vertices.push_back(pos + glm::ivec3{ 0, -1, 1 });
        vertices.push_back(pos + glm::ivec3{ 0, -1, 0 });
        vertices.push_back(pos);
    }
    float unit = 1.0f / Constants::atlasSize;
    offset = offset * (float)Constants::atlasBlockAmount * unit;
    uvs.push_back(glm::vec2{ offset.x + (((float)Constants::atlasBlockAmount - 1) * unit),1 - offset.y });
    uvs.push_back(glm::vec2{ offset.x + (((float)Constants::atlasBlockAmount - 1) * unit),1 - offset.y - (((float)Constants::atlasBlockAmount - 1) * unit) });
    uvs.push_back(glm::vec2{ offset.x,1 - offset.y - (((float)Constants::atlasBlockAmount - 1) * unit) });
    uvs.push_back(glm::vec2{ offset.x,1 - offset.y});
    indicies.push_back(size);
    indicies.push_back(size + 1);
    indicies.push_back(size + 2);
    indicies.push_back(size);
    indicies.push_back(size + 2);
    indicies.push_back(size + 3);
}

void Chunk::processBlock(const glm::ivec3 pos) {
    int moves[] = {-1,1};
    glm::ivec3 dirs[] = { {1,0,0},{0,1,0},{0,0,1} };
    for (glm::ivec3 dir : dirs) {
        for (int move : moves) {
            glm::ivec3 currentDir = dir * move;
            glm::ivec3 newPos = pos + currentDir;
            glm::ivec2 offset = glm::ivec2(
                (newPos.x < 0) ? -1 : (newPos.x >= Constants::chunkWidth) ? 1 : 0,
                (newPos.z < 0) ? -1 : (newPos.z >= Constants::chunkWidth) ? 1 : 0
            ) + coords;
            if (newPos.y < 0 ) continue;
            if (offset == coords) {
                if (isBlock(newPos)) continue;
            }
            else{
                glm::ivec3 blockPosInOtherChunk = glm::ivec3(
                    (offset.x < coords.x) ? Constants::chunkWidth-1 : (offset.x > coords.x) ? 0 : newPos.x,
                    newPos.y,
                    (offset.y < coords.y) ? Constants::chunkWidth-1 : (offset.y > coords.y) ? 0 : newPos.z
                );
                if (chunkObserver->isBlockAt(offset, blockPosInOtherChunk)) {
                    continue;
                }
            }
            makeFace(pos, currentDir);
            ++counter;
        }
    }
}
void Chunk::makeMesh() 
{
    indicies.clear();
    vertices.clear();
    uvs.clear();
    for (int i = 0; i < Constants::chunkWidth; ++i) {
        for (int j = 0; j < Constants::chunkWidth; ++j) {
            for (int k = 0; k < Constants::chunkHeight-1; ++k) {
                glm::ivec3 vec{ i,k,j };
                if (!isBlock(vec)) {
                    continue;
                }
                processBlock(vec);
            }
        }
    }
    updateModel();
}
void Chunk::updateModel()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_uvbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_uvbo);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(GLuint), indicies.data(), GL_STATIC_DRAW);

    m_indicesCount = indicies.size();

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
int Chunk::getHeight(const glm::ivec2& blockCoords) {
    for (int y = Constants::chunkHeight - 1; y >= 0; --y) {
        if (isBlock(glm::ivec3{ blockCoords.x,y,blockCoords.y })) {
            return y;
        }
    }
    return 0;
}

