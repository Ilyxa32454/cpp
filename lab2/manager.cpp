#include "manager.h"
#include <iostream>
#include <chrono>
#include "Utils.h"
#include <unordered_set>
using namespace std;


void Manager::display_search_menu() {
    cout << endl << "SEARCH MENU:" << endl
        << "1. Find pipes by name" << endl << "2. Find pipes by repair status" << endl << "3. Find stations by name" << endl << "4. Find stations by unused percentage" << endl << "0. Back to main menu" << endl << "Choose option: ";
}

void Manager::display_main_menu() {
    cout << endl << "MAIN MENU:" << endl<< "1. Add pipe" << endl<< "2. Add station" << endl<< "3. View all objects" << endl << "4. Edit pipe" << endl<< "5. Edit station" << endl << "6. Delete pipe" << endl << "7. Delete station" << endl << "8. View all pipes" << endl<< "9. View all stations" << endl << "10. Save to file" << endl << "11. Load from file" << endl << "12. Search objects" << endl <<"13. Batch edit pipes" << endl << "0. Exit" << endl
<< "Choose option: ";

}

void Manager::display_all_pipes() {
    if (pipes.empty()) {
        cout << "No pipes available." << endl;
        return;
    }

    cout << "ALL PIPES" << endl;

    for (auto& [id, pipe] : pipes) {
        cout << pipe;
    }
}

void Manager::display_all_stations() {
    if (stations.empty()) {
        cout << "No stations available." << endl;
        return;
    }

    cout << "ALL STATIONS" << endl;
    for (auto& [id, station] : stations) {
        cout << station;
    }
}

void Manager::display_all_objects() {
    display_all_pipes();
    display_all_stations();
}
void Manager::save_to_file(string& filename) {
    ofstream outputF(filename);
    if (outputF.is_open()) {
        outputF << "NEXT_IDS" << endl;
        outputF << nextPipeId << ' ' << nextStationId << endl;

        for (auto& [id, pipe] : pipes) {
            pipe.save_pipe(outputF);
        }

        for (auto& [id, station] : stations) {
            station.save_station(outputF);
        }

        cout << "Successfully saved to " << filename << endl;
    }
    else {
        cout << "Error opening file for writing!" << endl;
    }
}


void Manager::add_pipe() {
    Pipe newPipe(nextPipeId);
    cin >> newPipe;
    pipes.emplace(nextPipeId,newPipe);
    cout << "Pipe added with ID: " << nextPipeId << endl;
    nextPipeId++;
}

void Manager::edit_pipe(int id) {
    if (pipes.contains(id)){
        pipes[id].edit_pipe(); // at(id) указывает на ключ https://www.geeksforgeeks.org/cpp/unordered_map-at-cpp/
        cout << endl << "You successfully changed the fixing value of Pipe with ID " + to_string(id) + "to " + to_string(pipes[id].get_is_fixing()) << endl;
    }
    else {
        cout << "Pipe with ID " << id << " not found!" << endl;
    }
}

void Manager::delete_pipe(int id) {
        pipes.erase(id);
        if (pipes.erase(id)) {
            cout << "Pipe with ID " << id << " deleted." << endl;
        }
        else {
        cout << "Pipe with ID " << id << " not found!" << endl;
    }
}

void Manager::add_station() {
    Station newStation(nextStationId);
    cin >> newStation;
    stations.emplace(nextStationId,newStation);;
    cout << "Station added with ID: " << nextStationId << endl;
    nextStationId++;
}

void Manager::edit_station(int id) {
    if (stations.contains(id)) {
        stations[id].edit_station();
    }
    else {
        cout << "Station with ID " << id << " not found!" << endl;
    }
}

void Manager::delete_station(int id) {

        stations.erase(id);
        if (stations.erase(id)){
            cout << "Station with ID " << id << " deleted." << endl;
    }
    else {
        cout << "Station with ID " << id << " not found!" << endl;
    }
}


void Manager::load_from_file(string& filename) {
    ifstream loadF(filename);
    if (loadF.is_open()) {
        pipes.clear();
        stations.clear();
        
        string marker;
        while (loadF >> marker) {
            if (marker == "NEXT_IDS") {
                loadF >> nextPipeId >> nextStationId;
            }
            else if (marker == "PIPE") {
                Pipe pipe;
                pipe.load_pipe(loadF);
                pipes[pipe.get_pipe_id()] = pipe;
            }
            else if (marker == "CS") {
                Station station;
                station.load_station(loadF);
                stations[station.get_station_id()] = station;
            }
        }
        
        cout << "Successfully loaded from " << filename << endl;
        cout << nextPipeId << " " <<  nextStationId << endl;
    } else {
        cout << "Error opening file for reading!" << endl;
    }
}
bool CheckByNamePipe(const Pipe & pipe,string filter_name) {
    return pipe.name == filter_name;
}
bool CheckByStatus(const Pipe & pipe,bool filter_status) {
    return pipe.fixing == filter_status;
}

bool CheckByNameStation(const Station& station, string filter_name) {
    return station.name == filter_name;
}
bool CheckByPercentage(const Station& station, double filter_value) {
    return station.unused_workshops_percentage <= filter_value;
}

template<typename T>
using Filter_pipe = bool(*)(const Pipe &pipe,T param);

template<typename T>
vector<int>FindPipesByFilter(const unordered_map<int, Pipe>& pipes, Filter_pipe<T> f, T param) {
    vector<int>res;
    int i = 1;
    for (auto& [id, pipe] : pipes) {
        if (f(pipe,param))
            res.push_back(i);
        i++;
    }
    return res;
}
template<typename T>
using Filter_station = bool(*)(const Station& station, T param);

template<typename T>
vector<int>FindStationsByFilter(const unordered_map<int, Station> &stations, Filter_station<T> f, T param) {
    vector<int>res;
    int i = 1;
    for (auto& [id, station] : stations) {
        if (f(station,param))
            res.push_back(i);
        i++;
    }
    return res;
}


void Manager::handle_search() {
    string search_name;
    double value;
    bool repair_status;
    vector<int> found_ids;

    while (true) {
        display_search_menu();

        switch (GetCorrectNumber(0,4)) {
        case 0:
            return;
        case 1: {
            cout << "Enter pipe name to search: ";
            INPUT_LINE(cin, search_name);
            found_ids = FindPipesByFilter(pipes,CheckByNamePipe, search_name);
            cout << "Found " << found_ids.size() << " pipes:" << endl;
            for (int id : found_ids) {
                cout << "Pipe ID: " << id << endl;
            }
            break;
        }
        case 2: {
            cout << "Enter repair status (0 - working, 1 - in repair): ";
            repair_status = GetCorrectNumber(0, 1);
            found_ids = FindPipesByFilter(pipes,CheckByStatus,repair_status);
            cout << "Found " << found_ids.size() << " pipes:" << endl;
            for (int id : found_ids) cout << "Pipe ID: " << id << endl;
            break;
        }
        case 3: {
            cout << "Enter station name to search: ";
            INPUT_LINE(cin, search_name);
            found_ids = FindStationsByFilter(stations, CheckByNameStation,search_name);
            cout << "Found " << found_ids.size() << " stations:" << endl;
            for (int id : found_ids) {
                cout << "Station ID: " << id << endl;
            }
            break;
        }
        case 4: {
            cout << "Enter maximum unused percentage: ";
            value = GetCorrectNumber(0, 100);
            found_ids = FindStationsByFilter(stations, CheckByPercentage, value);
            cout << "Found " << found_ids.size() << " stations:" << endl;
            for (int id : found_ids) {
                cout << "Station ID: " << id << endl;
            }
            break;
        }
        }
    }
}

void Manager::edit_pipes_batch(vector<int>& ids) {
    if (ids.empty()) {
        cout << "No pipes selected for editing." << endl;
        return;
    }

    cout << "Choose editing mode:" << endl << "1. Toggle fixing status for all selected pipes" << endl << "2. Set specific fixing status (0 - working, 1 - under repair)" << endl << "Enter option: ";

    int choice = GetCorrectNumber(1, 2);

    bool new_status = false;
    if (choice == 2) {
        cout << "Enter status (0 - working, 1 - under repair): ";
        GetCorrectNumber(0, 1);
    }

    int edited_count = 0;
    for (int id : ids) {
        if (choice == 1)
            pipes.at(id).set_is_fixing(!pipes.at(id).get_is_fixing());
        else if (choice == 2)
            pipes.at(id).set_is_fixing(new_status);
        edited_count++;
    }
    cout << "Successfully edited " << edited_count << " pipes." << endl;
}

void Manager::delete_pipes_batch(vector<int>& ids) {
    if (ids.empty()) {
        cout << "No pipes selected for deletion." << endl;
        return;
    }

    cout << "Are you sure you want to delete " << ids.size() << " pipes? (1 - yes, 0 - no): ";
    int confirm = GetCorrectNumber(0, 1);
    if (confirm == 0) {
        cout << "Operation canceled." << endl;
        return;
    }

    int deleted_count = 0;
    for (int id : ids) {
        if (pipes.erase(id)) {
            deleted_count++;
        }
    }
    cout << "Deleted " << deleted_count << " pipes." << endl;
}

void Manager::pipes_batch_menu(vector<int>& ids) {
    cout << "Choose operation:" << endl << "1 - batch delete." << endl << "2 - batch edit." << endl;

    switch (GetCorrectNumber(1, 2)) {
    case 1:
        delete_pipes_batch(ids);
        break;
    case 2:
        edit_pipes_batch(ids);
        break;
    }
    handle_pipes_batch_menu();
}
void Manager::handle_pipes_batch_menu() {
    if (pipes.empty()) {
        cout << "No pipes available for batch editing.\n";
        return;
    }

    vector<int> selected_ids;
    cout << endl << "Batch Operation Mode" << endl << "1 - Select all pipes" << endl << "2 - Select by name" << endl << "3 - Select by status" << endl << "4 - Select manually by ID" << endl << "0 - Cancel" << endl << "Enter option: ";

        switch (GetCorrectNumber(0, 4)) {
        case 0:
            break;
        case 1:
            for (auto& [id, _] : pipes)
                selected_ids.push_back(id);
            break;
        case 2: {
            string name;
            cout << "Enter name: ";
            INPUT_LINE(cin, name);
            selected_ids = FindPipesByFilter(pipes, CheckByNamePipe, name);
            break;
        }
        case 3: {
            unordered_set<int> selected_ids_set;
            bool status;
            cout << "Enter status (0 - working, 1 - under repair): ";
            status = GetCorrectNumber(0, 1);
            selected_ids = FindPipesByFilter(pipes, CheckByStatus, status);
            for (int id : selected_ids) cout << id << ' '; cout << endl;
            cout << "Enter pipe IDs separated by enters (0 to finish): " << endl;
            while (true)
            {
                int id = GetCorrectNumber(0, nextPipeId - 1);
                if (id == 0)
                    break;
                selected_ids_set.insert(id);

                selected_ids.assign(selected_ids_set.begin(), selected_ids_set.end());
                break;
            }
        }
        case 4: {
            unordered_set<int> selected_ids_set;
            cout << "Enter pipe IDs separated by enters (0 to finish): ";

            while (true)
            {
                int id = GetCorrectNumber(0, nextPipeId - 1);
                if (id == 0)
                    break;
                selected_ids_set.insert(id);
            }
            selected_ids.assign(selected_ids_set.begin(), selected_ids_set.end());
            break;
        }
        }
        if (selected_ids.empty()) {
            cout << "No pipes selected for batch operation." << endl;
            return;
        }

        cout << endl << "Selected " << selected_ids.size() << " pipes: ";
        for (int id : selected_ids) cout << id << ' ';
        cout << endl;
        pipes_batch_menu(selected_ids);
    }

