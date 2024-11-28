#ifndef ASTAR_LIBRARY_H
#define ASTAR_LIBRARY_H

#include "AStarPathFinding.h"
#ifdef ASTARLIBRARY_EXPORTS
#define ASTARLIBRARY_API __declspec(dllexport)
#else
#define ASTARLIBRARY_API __declspec(dllimport)
#endif

extern "C" {
    __declspec(dllexport) int CreateAstarGrid(int sizeX, int sizeY, int sizeZ, float nodeSize, float gridPosX, float gridPosY, float gridPosZ);
    __declspec(dllexport) void DeleteAstarGrid(int id);
    __declspec(dllexport) int FindPath(int gridId, Vector3 start, Vector3 end, int* pathLength);
    __declspec(dllexport) void setNodeWalkable(int gridId, int gridX, int gridY, int gridZ, bool isWalkable);
}
#endif