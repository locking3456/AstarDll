#ifndef ASTAR_GRID_H
#define ASTAR_GRID_H

#include "AstarNode.h"
#include <vector>

class AstarGrid {
private:
    float gridStartPosX, gridStartPosY, gridStartPosZ;
    float nodeSize;
    std::vector<std::vector<std::vector<AstarNode*>>> grid;
    

public:
    int gridSizeX, gridSizeY, gridSizeZ;
    AstarGrid(int sizeX, int sizeY, int sizeZ, float nodeSize, float gridPosX, float gridPosY, float gridPosZ);
    ~AstarGrid();
    void CreateGrid();
    AstarNode* NodeFromWorldPoint(const Vector3& position);
    Vector3 GetWorldPosition(const AstarNode* node) const;
    std::vector<AstarNode*> GetNeighbours(AstarNode* node);
    void SetNodeWalkable(int x, int y, int z, bool walkable);
    AstarNode* GetNode(int x, int y, int z) const;
};

#endif // ASTAR_GRID_H
