#pragma once
#ifndef ASTAR_CONTAINER_H
#define ASTAR_CONTAINER_H

#include <unordered_map>
#include <mutex>
#include <vector>
#include "AstarGrid.h"

// 전역 컨테이너 선언
extern std::unordered_map<int, AstarGrid*> gridMap;
extern std::unordered_map<int, std::vector<AstarNode*>> paths;
extern std::mutex gridMutex;
extern int nextGridId;
extern int nextPathId;

// 함수 선언
int AddAstarGrid(AstarGrid* grid);
AstarGrid* GetAstarGridById(int id);
void DeleteAstarGridById(int id);

#endif // ASTAR_CONTAINER_H
