#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <unordered_map>
#include <vector>
#include "pipe.h"
#include "cs.h"

void addPipe(std::unordered_map<int, Pipe>& pipes);
void addCS(std::unordered_map<int, CS>& stations);
void viewAllObjects(const std::unordered_map<int, Pipe>& pipes, const std::unordered_map<int, CS>& stations);
void editPipe(std::unordered_map<int, Pipe>& pipes);
void editCS(std::unordered_map<int, CS>& stations);
void deleteObject(std::unordered_map<int, Pipe>& pipes, std::unordered_map<int, CS>& stations);
void searchPipesByName(const std::unordered_map<int, Pipe>& pipes);
void searchPipesByStatus(const std::unordered_map<int, Pipe>& pipes);
void searchCSByName(const std::unordered_map<int, CS>& stations);
void searchCSByInactivePercentage(const std::unordered_map<int, CS>& stations);
void batchEditPipes(std::unordered_map<int, Pipe>& pipes);
void saveToFile(const std::unordered_map<int, Pipe>& pipes, const std::unordered_map<int, CS>& stations);
void loadFromFile(std::unordered_map<int, Pipe>& pipes, std::unordered_map<int, CS>& stations);

#endif