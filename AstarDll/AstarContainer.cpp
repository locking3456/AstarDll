#include "AstarContainer.h"
#include <map>
// ���� ���� ����
std::unordered_map<int, AstarGrid*> gridMap;
std::unordered_map<int, std::vector<AstarNode*>> paths;
std::mutex gridMutex;
int nextGridId = 0;
int nextPathId = 0;
// ���ο� AstarGrid�� �߰��ϰ� ID ��ȯ
int AddAstarGrid(AstarGrid* grid) {
    std::lock_guard<std::mutex> lock(gridMutex);
    int id = nextGridId++;
    gridMap[id] = grid;
    return id;
}

// ID�� AstarGrid�� �˻�
AstarGrid* GetAstarGridById(int id) {
    std::lock_guard<std::mutex> lock(gridMutex);
    auto it = gridMap.find(id);
    if (it != gridMap.end()) {
        return it->second;
    }
    return nullptr;
}

// ID�� AstarGrid�� ����
void DeleteAstarGridById(int id) {
    std::lock_guard<std::mutex> lock(gridMutex);
    auto it = gridMap.find(id);
    if (it != gridMap.end()) {
        delete it->second;
        gridMap.erase(it);
    }
}
