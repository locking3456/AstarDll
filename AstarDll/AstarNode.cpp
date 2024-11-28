#include "AstarNode.h"

// Constructor
AstarNode::AstarNode(bool isWalkable, int x, int y, int z)
    : IsWalkable(isWalkable),
    GridX(x), GridY(y), GridZ(z), GCost(0), HCost(0), Parent(nullptr) {}

// Destructor
AstarNode::~AstarNode() {
    // Cleanup logic if needed
}

// FCost = GCost + HCost
int AstarNode::FCost() const {
    return GCost + HCost;
}

// Comparison operator for sorting by FCost
bool AstarNode::operator<(const AstarNode& other) const {
    return FCost() < other.FCost();
}
