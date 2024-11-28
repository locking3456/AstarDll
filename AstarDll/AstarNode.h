#ifndef ASTAR_NODE_H
#define ASTAR_NODE_H

#include <iostream>
#include <cmath>

struct Vector3 {
    float x, y, z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    bool operator==(const Vector3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Vector3& other) const {
        return !(*this == other);
    }

    float DistanceTo(const Vector3& other) const {
        return std::sqrt((x - other.x) * (x - other.x) +
            (y - other.y) * (y - other.y) +
            (z - other.z) * (z - other.z));
    }
};

class AstarNode {
public:
    bool IsWalkable;
    int GridX, GridY, GridZ;
    AstarNode* Parent;

    // Constructor
    AstarNode(bool isWalkable, int x, int y, int z);
    // Destructor
    ~AstarNode();

    // G and H cost properties
    int GCost;
    int HCost;

    int FCost() const;

    // Comparison for priority
    bool operator<(const AstarNode& other) const;
};

#endif