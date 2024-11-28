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
    // 경로 ID에 해당하는 데이터를 찾음
    auto it = paths.find(pathId);
    if (it == paths.end()) {
        // 경로가 없으면 길이를 0으로 설정하고 nullptr 반환
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
        // 경로에 포함된 모든 노드를 해제
        for (AstarNode* node : it->second) {
            delete node;
        }
        paths.erase(it); // 전역 컨테이너에서 제거
    }
}