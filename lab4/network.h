#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include <set>
#include <utility>
#include <vector>
#include "pipe.h"
#include "station.h"

class Network {
    std::unordered_map<int, Pipe> pipes;
    std::unordered_map<int, Station> stations;
    std::set<std::pair<int, int>> connections;

public:
    void AddPipe();
    void AddStation();
    void PrintObjs();

    std::set<int> FindPipes();
    std::set<int> FindStations();

    void PipesButchEditing();
    void StationsButchEditing();

    void DeletePipes();
    void DeleteStations();

    void SaveToFile(const std::string& filename);
    void LoadFromFile(const std::string& filename);

    void ConnectStations();
    void DisconnectStations();
    int FindPipeByDiameter();

    void TopologicSort();

    // Новые методы
    void CalculateMaxFlow();
    void CalculateShortestPath();

private:
    void removeConnectionsWithPipe(int pipeId);
    void removeConnectionsWithStation(int stationId);
    void updatePipeConnectionsAfterStationDeletion(int deletedStationId);

    // Вспомогательные методы для расчетов
    bool bfsForMaxFlow(int source, int sink, std::unordered_map<int, int>& parent);
    std::vector<std::pair<int, double>> dijkstra(int start, int end);
};