#include "AstarGrid.h"
#include <algorithm>

AstarGrid::AstarGrid(int sizeX, int sizeY, int sizeZ, float nodeSize, float gridPosX, float gridPosY, float gridPosZ)
    : gridSizeX(sizeX), gridSizeY(sizeY), gridSizeZ(sizeZ), nodeSize(nodeSize),gridStartPosX(gridPosX), gridStartPosY(gridPosY), gridStartPosZ(gridPosZ)  {
    grid.resize(sizeX, std::vector<std::vector<AstarNode*>>(sizeY, std::vector<AstarNode*>(sizeZ, nullptr)));
}

AstarGrid::~AstarGrid() {
    for (auto& layer : grid) {
        for (auto& row : layer) {
            for (auto& node : row) {
                delete node;
            }
        }
    }
}

void AstarGrid::CreateGrid() {
    for (int x = 0; x < gridSizeX; ++x) {
        for (int y = 0; y < gridSizeY; ++y) {
            for (int z = 0; z < gridSizeZ; ++z) {
                grid[x][y][z] = new AstarNode(true, x, y, z);
            }
        }
    }
}

AstarNode* AstarGrid::NodeFromWorldPoint(const Vector3& position) {
    // 시작 위치를 기준으로 상대 위치 계산
    float relativeX = position.x - gridStartPosX;
    float relativeY = position.y - gridStartPosY;
    float relativeZ = position.z - gridStartPosZ;

    // 노드 크기를 사용하여 인덱스 계산
    int x = static_cast<int>(std::clamp(relativeX / nodeSize, 0.0f, static_cast<float>(gridSizeX - 1)));
    int y = static_cast<int>(std::clamp(relativeY / nodeSize, 0.0f, static_cast<float>(gridSizeY - 1)));
    int z = static_cast<int>(std::clamp(relativeZ / nodeSize, 0.0f, static_cast<float>(gridSizeZ - 1)));

    // 노드 반환
    return grid[x][y][z];
}

Vector3 AstarGrid::GetWorldPosition(const AstarNode* node) const {
    float worldX = gridStartPosX + (0.5f * nodeSize) + (nodeSize * node->GridX);
    float worldY = gridStartPosY + (0.5f * nodeSize) + (nodeSize * node->GridY);
    float worldZ = gridStartPosZ + (0.5f * nodeSize) + (nodeSize * node->GridZ);
    return Vector3(worldX, worldY, worldZ);
}


std::vector<AstarNode*> AstarGrid::GetNeighbours(AstarNode* node) {
    std::vector<AstarNode*> neighbours;
    int x = node->GridX, y = node->GridY, z = node->GridZ;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dz = -1; dz <= 1; ++dz) {
                if (dx == 0 && dy == 0 && dz == 0) continue;

                int checkX = x + dx, checkY = y + dy, checkZ = z + dz;
                if (checkX >= 0 && checkX < gridSizeX &&
                    checkY >= 0 && checkY < gridSizeY &&
                    checkZ >= 0 && checkZ < gridSizeZ) {
                    neighbours.push_back(grid[checkX][checkY][checkZ]);
                }
            }
        }
    }
    return neighbours;
}

AstarNode* AstarGrid::GetNode(int x, int y, int z) const {
    if (x >= 0 && x < gridSizeX && y >= 0 && y < gridSizeY && z >= 0 && z < gridSizeZ) {
        return grid[x][y][z];
    }
    return nullptr; // 범위를 벗어난 경우 nullptr 반환
}

void AstarGrid::SetNodeWalkable(int x, int y, int z, bool walkable) {
    if (grid[x][y][z]) {
        grid[x][y][z]->IsWalkable = walkable;
    }
}


