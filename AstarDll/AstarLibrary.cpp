#include "AStarLibrary.h"
#include "AstarContainer.h"
#include <vector>
extern "C" __declspec(dllexport) int CreateAstarGrid(int sizeX, int sizeY, int sizeZ, float nodeSize, float gridPosX, float gridPosY, float gridPosZ) {
    AstarGrid* newGrid = new AstarGrid(sizeX, sizeY, sizeZ, nodeSize, gridPosX, gridPosY, gridPosZ);
    newGrid->CreateGrid();
    return AddAstarGrid(newGrid);
}

extern "C" __declspec(dllexport) void DeleteAstarGrid(int id) {
    DeleteAstarGridById(id);
}

extern "C" __declspec(dllexport) int FindPath(int gridId, Vector3 start, Vector3 end, int* pathLength) {
    AstarGrid* grid = gridMap[gridId];
    AstarPathfinding pathFinder(grid);
    auto path = pathFinder.FindPath(start, end);

    int pathId = nextPathId++;
    paths[pathId] = path;

    *pathLength = static_cast<int>(path.size());
    return pathId;
}

extern "C" __declspec(dllexport) AstarNode * *GetPath(int pathId, int* pathLength) {
    // ��� ID�� �ش��ϴ� �����͸� ã��
    auto it = paths.find(pathId);
    if (it == paths.end()) {
        // ��ΰ� ������ ���̸� 0���� �����ϰ� nullptr ��ȯ
        *pathLength = 0;
        return nullptr;
    }

    const std::vector<AstarNode*>& path = it->second;
    *pathLength = static_cast<int>(path.size());

    AstarNode** pathArray = new AstarNode * [*pathLength];
    std::copy(path.begin(), path.end(), pathArray);

    return pathArray;
}

extern "C" __declspec(dllexport) void setNodeWalkable(int gridId,int gridX, int gridY, int gridZ, bool isWalkable) {
    AstarGrid* grid = gridMap[gridId];
    grid->SetNodeWalkable(gridX, gridY, gridZ, isWalkable);
}

extern "C" __declspec(dllexport) void RemovePath(int pathId) {
    auto it = paths.find(pathId);
    if (it != paths.end()) {
        // ��ο� ���Ե� ��� ��带 ����
        for (AstarNode* node : it->second) {
            delete node;
        }
        paths.erase(it); // ���� �����̳ʿ��� ����
    }
}