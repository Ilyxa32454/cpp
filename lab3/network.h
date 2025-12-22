#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include <set>
#include <utility>
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

private:
    void removeConnectionsWithPipe(int pipeId);
    void removeConnectionsWithStation(int stationId);
    void updatePipeConnectionsAfterStationDeletion(int deletedStationId);
};