#pragma once
#ifndef ASTAR_CONTAINER_H
#define ASTAR_CONTAINER_H

#include <unordered_map>
#include <mutex>
#include <vector>
#include "AstarGrid.h"

// ���� �����̳� ����
extern std::unordered_map<int, AstarGrid*> gridMap;
extern std::unordered_map<int, std::vector<AstarNode*>> paths;
extern std::mutex gridMutex;
extern int nextGridId;
extern int nextPathId;

// �Լ� ����
int AddAstarGrid(AstarGrid* grid);
AstarGrid* GetAstarGridById(int id);
void DeleteAstarGridById(int id);

#endif // ASTAR_CONTAINER_H
