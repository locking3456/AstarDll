#ifndef ASTAR_PATHFINDING_H
#define ASTAR_PATHFINDING_H

#include "AstarGrid.h"
#include <vector>
#include <functional>

class AstarPathfinding {
private:
    AstarGrid* grid;
    int maxPathFindCount;

    // Helper method to retrace the path
    std::vector<AstarNode*> RetracePath(AstarNode* startNode, AstarNode* endNode);

    // Heuristic function for A* algorithm
    int GetDistance(AstarNode* nodeA, AstarNode* nodeB);

    // Heuristic function with directional weight
    int GetDistanceWithWeight(AstarNode* nodeA, AstarNode* nodeB, AstarNode* endNode, AstarNode* parentNode = nullptr);

public:
    AstarPathfinding(AstarGrid* grid, int maxPathFindCount);

    AstarPathfinding(AstarGrid* grid);
    // Find path between two points
    std::vector<AstarNode*> FindPath(const Vector3& startPos, const Vector3& endPos);

    // Find path with a dynamic end position
    std::vector<AstarNode*> FindPath(const Vector3& startPos, std::function<Vector3(const Vector3&)> endPositionFunc);

    // Mark path as unwalkable
    void MarkPathAsUnwalkable(const std::vector<AstarNode*>& path);

    // Mark path as walkable
    void MarkPathAsWalkable(const std::vector<AstarNode*>& path);

    static AstarNode** ConvertVectorToCArray(const std::vector<AstarNode*>& vec, int* length);
};

#endif