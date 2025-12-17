#pragma once
#include <vector>
#include <unordered_map>
#include "pipe.h"
#include "station.h"

using namespace std;

class Manager {
private:
    unordered_map<int, Pipe> pipes;
    unordered_map<int, Station> stations;
    int nextStationId = 1;
    int nextPipeId = 1;

public:
    void add_pipe();
    void display_all_pipes();
    void edit_pipe(int id);
    void delete_pipe(int id);

    void add_station();
    void display_all_stations() ;
    void edit_station(int id);
    void delete_station(int id);

    void handle_search();
    void handle_pipes_batch_menu();
    void pipes_batch_menu(vector<int>& ids);
    void edit_pipes_batch(vector<int>& ids);
    void delete_pipes_batch(vector<int>& ids);


    void display_main_menu();
    void display_search_menu();
    void display_all_objects();

    void save_to_file(string& filename) ;
    void load_from_file(string& filename);
};

