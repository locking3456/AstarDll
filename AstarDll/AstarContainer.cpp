#include "AstarContainer.h"
#include <map>
// 전역 변수 정의
std::unordered_map<int, AstarGrid*> gridMap;
std::unordered_map<int, std::vector<AstarNode*>> paths;
std::mutex gridMutex;
int nextGridId = 0;
int nextPathId = 0;
// 새로운 AstarGrid를 추가하고 ID 반환
int AddAstarGrid(AstarGrid* grid) {
    std::lock_guard<std::mutex> lock(gridMutex);
    int id = nextGridId++;
    gridMap[id] = grid;
    return id;
}

// ID로 AstarGrid를 검색
AstarGrid* GetAstarGridById(int id) {
    std::lock_guard<std::mutex> lock(gridMutex);
    auto it = gridMap.find(id);
    if (it != gridMap.end()) {
        return it->second;
    }
    return nullptr;
}

// ID로 AstarGrid를 삭제
void DeleteAstarGridById(int id) {
    std::lock_guard<std::mutex> lock(gridMutex);
    auto it = gridMap.find(id);
    if (it != gridMap.end()) {
        delete it->second;
        gridMap.erase(it);
    }
}
