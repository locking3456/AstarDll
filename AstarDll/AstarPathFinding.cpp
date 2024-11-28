#include "AstarPathfinding.h"
#include <queue>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cmath>

// Constructor
AstarPathfinding::AstarPathfinding(AstarGrid* grid, int maxPathFindCount)
    : grid(grid), maxPathFindCount(maxPathFindCount) {}

AstarPathfinding::AstarPathfinding(AstarGrid* grid)
    : grid(grid) {
    maxPathFindCount = grid->gridSizeX * grid->gridSizeY * grid->gridSizeY;
}

// Heuristic function: Manhattan distance
int AstarPathfinding::GetDistance(AstarNode* nodeA, AstarNode* nodeB) {
    int distX = std::abs(nodeA->GridX - nodeB->GridX);
    int distY = std::abs(nodeA->GridY - nodeB->GridY);
    int distZ = std::abs(nodeA->GridZ - nodeB->GridZ);
    return distX + distY + distZ;
}

// Heuristic function with directional weight
int AstarPathfinding::GetDistanceWithWeight(AstarNode* nodeA, AstarNode* nodeB, AstarNode* endNode, AstarNode* parentNode) {
    int baseDistance = GetDistance(nodeA, nodeB);
    int directionWeight = 0;

    if (parentNode != nullptr) {
        Vector3 prevDir = {
            (float)(nodeA->GridX - parentNode->GridX),
            (float)(nodeA->GridY - parentNode->GridY),
            (float)(nodeA->GridZ - parentNode->GridZ)
        };

        Vector3 newDir = {
            (float)(nodeB->GridX - nodeA->GridX),
            (float)(nodeB->GridY - nodeA->GridY),
            (float)(nodeB->GridZ - nodeA->GridZ)
        };

        if (prevDir != newDir) {
            directionWeight += 1;
        }
    }

    return baseDistance + directionWeight;
}

// Retrace path from end to start
std::vector<AstarNode*> AstarPathfinding::RetracePath(AstarNode* startNode, AstarNode* endNode) {
    std::vector<AstarNode*> path;
    AstarNode* currentNode = endNode;

    while (currentNode != startNode) {
        path.push_back(currentNode);
        grid->SetNodeWalkable(currentNode->GridX, currentNode->GridY, currentNode->GridZ, false);
        currentNode = currentNode->Parent;
    }

    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<AstarNode*> AstarPathfinding::FindPath(const Vector3& startPos, const Vector3& endPos) {
    AstarNode* startNode = grid->NodeFromWorldPoint(startPos);
    AstarNode* endNode = grid->NodeFromWorldPoint(endPos);

    if (startNode == nullptr || endNode == nullptr || !startNode->IsWalkable || !endNode->IsWalkable) {
        return {};
    }

    std::vector<AstarNode*> openSet;
    std::unordered_set<AstarNode*> closedSet;

    openSet.push_back(startNode);

    while (!openSet.empty()) {
        AstarNode* currentNode = openSet[0];
        for (size_t i = 1; i < openSet.size(); ++i) {
            if (openSet[i]->FCost() < currentNode->FCost() ||
                (openSet[i]->FCost() == currentNode->FCost() && openSet[i]->HCost < currentNode->HCost)) {
                currentNode = openSet[i];
            }
        }

        openSet.erase(std::remove(openSet.begin(), openSet.end(), currentNode), openSet.end());
        closedSet.insert(currentNode);

        if (currentNode == endNode) {
            return RetracePath(startNode, endNode);
        }

        for (AstarNode* neighbor : grid->GetNeighbours(currentNode)) {
            if (!neighbor->IsWalkable || closedSet.find(neighbor) != closedSet.end()) {
                continue;
            }

            int newCostToNeighbor = currentNode->GCost + GetDistanceWithWeight(currentNode, neighbor, endNode, currentNode->Parent);
            if (newCostToNeighbor < neighbor->GCost ||
                std::find(openSet.begin(), openSet.end(), neighbor) == openSet.end()) {
                neighbor->GCost = newCostToNeighbor;
                neighbor->HCost = GetDistance(neighbor, endNode);
                neighbor->Parent = currentNode;

                if (std::find(openSet.begin(), openSet.end(), neighbor) == openSet.end()) {
                    openSet.push_back(neighbor);
                }
            }
        }
    }

    return {};
}

// Find path with a dynamic end position
std::vector<AstarNode*> AstarPathfinding::FindPath(const Vector3& startPos, std::function<Vector3(const Vector3&)> endPositionFunc) {
    AstarNode* startNode = grid->NodeFromWorldPoint(startPos);

    if (startNode == nullptr || !startNode->IsWalkable) {
        return {};
    }

    std::vector<AstarNode*> openSet;
    std::unordered_set<AstarNode*> closedSet;

    openSet.push_back(startNode);

    while (!openSet.empty()) {
        AstarNode* currentNode = openSet[0];
        for (size_t i = 1; i < openSet.size(); ++i) {
            if (openSet[i]->FCost() < currentNode->FCost() ||
                (openSet[i]->FCost() == currentNode->FCost() && openSet[i]->HCost < currentNode->HCost)) {
                currentNode = openSet[i];
            }
        }

        openSet.erase(std::remove(openSet.begin(), openSet.end(), currentNode), openSet.end());
        closedSet.insert(currentNode);

        Vector3 dynamicEndPos = endPositionFunc(grid->GetWorldPosition(currentNode));
        AstarNode* endNode = grid->NodeFromWorldPoint(dynamicEndPos);

        if (currentNode == endNode) {
            return RetracePath(startNode, endNode);
        }

        for (AstarNode* neighbor : grid->GetNeighbours(currentNode)) {
            if (!neighbor->IsWalkable || closedSet.find(neighbor) != closedSet.end()) {
                continue;
            }

            int newCostToNeighbor = currentNode->GCost + GetDistanceWithWeight(currentNode, neighbor, endNode, currentNode->Parent);
            if (newCostToNeighbor < neighbor->GCost ||
                std::find(openSet.begin(), openSet.end(), neighbor) == openSet.end()) {
                neighbor->GCost = newCostToNeighbor;
                neighbor->HCost = GetDistance(neighbor, endNode);
                neighbor->Parent = currentNode;

                if (std::find(openSet.begin(), openSet.end(), neighbor) == openSet.end()) {
                    openSet.push_back(neighbor);
                }
            }
        }
    }

    return {};
}

// Mark path as unwalkable
void AstarPathfinding::MarkPathAsUnwalkable(const std::vector<AstarNode*>& path) {
    for (AstarNode* node : path) {
        node->IsWalkable = false;
    }
}

// Mark path as walkable
void AstarPathfinding::MarkPathAsWalkable(const std::vector<AstarNode*>& path) {
    for (AstarNode* node : path) {
        node->IsWalkable = true;
    }
}

AstarNode** AstarPathfinding::ConvertVectorToCArray(const std::vector<AstarNode*>& vec, int* length) {
    *length = static_cast<int>(vec.size());
    if (*length == 0) {
        return nullptr;
    }

    AstarNode** array = new AstarNode * [*length];
    for (int i = 0; i < *length; ++i) {
        array[i] = vec[i];
    }
    delete length;
    return array;
}