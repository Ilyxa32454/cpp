#include "network.h"
#include "pipe.h"
#include "station.h"
#include "utils.h"
#include <iostream>
#include <set>
#include <vector>
#include <fstream>
#include <queue>
#include <algorithm>

void Network::AddPipe() {
    Pipe pipe;
    std::cout << "\n=== Добавление новой трубы ===" << std::endl;
    std::cin >> pipe;
    pipes.emplace(pipe.getId(), pipe);
    std::cout << "Труба добавлена с ID: " << pipe.getId() << std::endl;
}

void Network::AddStation() {
    Station station;
    std::cout << "\n=== Добавление новой КС ===" << std::endl;
    std::cin >> station;
    stations.emplace(station.getId(), station);
    std::cout << "Станция добавлена с ID: " << station.getId() << std::endl;
}

void Network::PrintObjs() {
    std::cout << "\n=== Список всех объектов ===" << std::endl;
    std::cout << "\nТрубы (" << pipes.size() << " шт.):" << std::endl;
    if (pipes.empty()) {
        std::cout << "Труб нет" << std::endl;
    }
    else {
        for (const auto& pipe_pair : pipes) {
            std::cout << "ID: " << pipe_pair.first << std::endl;
            std::cout << pipe_pair.second;
        }
    }

    std::cout << "\nСтанции (" << stations.size() << " шт.):" << std::endl;
    if (stations.empty()) {
        std::cout << "Станций нет" << std::endl;
    }
    else {
        for (const auto& station_pair : stations) {
            std::cout << "ID: " << station_pair.first << std::endl;
            std::cout << station_pair.second;
        }
    }

    std::cout << "\nСоединения (" << connections.size() << " шт.):" << std::endl;
    if (connections.empty()) {
        std::cout << "Соединений нет" << std::endl;
    }
    else {
        for (const auto& conn : connections) {
            int pipe_id = -1;
            for (const auto& pipe_pair : pipes) {
                if (pipe_pair.second.getStart() == conn.first &&
                    pipe_pair.second.getEnd() == conn.second) {
                    pipe_id = pipe_pair.first;
                    break;
                }
            }
            std::cout << "Станция " << conn.first << " -> Станция " << conn.second;
            if (pipe_id != -1) {
                std::cout << " (Труба ID: " << pipe_id << ")" << std::endl;
            }
            else {
                std::cout << " (Труба не найдена)" << std::endl;
            }
        }
    }
}

std::set<int> Network::FindPipes() {
    if (pipes.empty()) {
        std::cout << "Нет труб для поиска" << std::endl;
        return std::set<int>();
    }

    std::cout << "\n=== Поиск труб ===" << std::endl;
    std::cout << "Критерий поиска:\n0 - по названию\n1 - по статусу ремонта\nВыберите: ";

    if (safe_input<int>(0, 1) == 0) {
        std::cout << "Введите название (или начало названия): ";
        std::string name;
        INPUT_LINE(std::cin, name);
        return find_by_filter(pipes, Pipe::check_pipe_by_name, name);
    }
    else {
        std::cout << "Статус ремонта:\n0 - не в ремонте\n1 - в ремонте\nВыберите: ";
        bool in_repair = safe_input<int>(0, 1);
        return find_by_filter(pipes, Pipe::check_pipe_by_in_repair, in_repair);
    }
}

std::set<int> Network::FindStations() {
    if (stations.empty()) {
        std::cout << "Нет станций для поиска" << std::endl;
        return std::set<int>();
    }

    std::cout << "\n=== Поиск станций ===" << std::endl;
    std::cout << "Критерий поиска:\n0 - по названию\n1 - по проценту незадействованных цехов\nВыберите: ";

    if (safe_input<int>(0, 1) == 0) {
        std::cout << "Введите название (или начало названия): ";
        std::string name;
        INPUT_LINE(std::cin, name);
        return find_by_filter(stations, Station::check_station_by_name, name);
    }
    else {
        std::cout << "Минимальный процент незадействованных цехов (0-100): ";
        double percentage = safe_input<double>(0., 100.) / 100.;
        return find_by_filter(stations, Station::check_station_by_workshop_percentage, percentage);
    }
}

void Network::PipesButchEditing() {
    if (pipes.empty()) {
        std::cout << "Нет труб для редактирования" << std::endl;
        return;
    }

    std::cout << "\n=== Редактирование труб ===" << std::endl;
    std::set<int> found_ids = FindPipes();

    if (found_ids.empty()) {
        std::cout << "Трубы не найдены" << std::endl;
        return;
    }

    std::set<int> selected_ids = select_objects_for_editing(pipes, found_ids);

    if (selected_ids.empty()) {
        std::cout << "Не выбрано ни одной трубы для редактирования" << std::endl;
        return;
    }

    std::cout << "\nДействие:\n1 - Изменить статус ремонта\nВыберите: ";

    if (safe_input<int>(1, 1) == 1) {
        std::cout << "Новый статус ремонта (0 - не в ремонте, 1 - в ремонте): ";
        bool in_repair = safe_input<int>(0, 1);

        for (int id : selected_ids) {
            pipes.at(id).setInRepair(in_repair);
        }
        std::cout << "Статус ремонта обновлен для " << selected_ids.size() << " труб" << std::endl;
    }
}

void Network::StationsButchEditing() {
    if (stations.empty()) {
        std::cout << "Нет станций для редактирования" << std::endl;
        return;
    }

    std::cout << "\n=== Редактирование станций ===" << std::endl;
    std::set<int> found_ids = FindStations();

    if (found_ids.empty()) {
        std::cout << "Станции не найдены" << std::endl;
        return;
    }

    std::set<int> selected_ids = select_objects_for_editing(stations, found_ids);

    if (selected_ids.empty()) {
        std::cout << "Не выбрано ни одной станции для редактирования" << std::endl;
        return;
    }

    std::cout << "\nДействие:\n1 - Изменить количество работающих цехов\nВыберите: ";

    if (safe_input<int>(1, 1) == 1) {
        std::cout << "Изменение цехов:\n1 - Увеличить на 1\n2 - Уменьшить на 1\nВыберите: ";
        int delta = (safe_input<int>(1, 2) == 1) ? 1 : -1;

        for (int id : selected_ids) {
            auto& station = stations.at(id);
            int new_active = station.getActiveWorkshops() + delta;

            if (new_active < 0) new_active = 0;
            if (new_active > station.getWorkshops()) new_active = station.getWorkshops();

            station.setActiveWorkshops(new_active);
        }
        std::cout << "Количество цехов изменено для " << selected_ids.size() << " станций" << std::endl;
    }
}

void Network::DeletePipes() {
    if (pipes.empty()) {
        std::cout << "Нет труб для удаления" << std::endl;
        return;
    }

    std::cout << "\n=== Удаление труб ===" << std::endl;
    std::set<int> found_ids = FindPipes();

    if (found_ids.empty()) {
        std::cout << "Трубы не найдены" << std::endl;
        return;
    }

    std::set<int> selected_ids = select_objects_for_editing(pipes, found_ids);

    if (selected_ids.empty()) {
        std::cout << "Не выбрано ни одной трубы для удаления" << std::endl;
        return;
    }

    std::cout << "Вы уверены, что хотите удалить " << selected_ids.size() << " труб? (1 - да, 0 - нет): ";
    if (safe_input<int>(0, 1) == 1) {
        for (int id : selected_ids) {
            removeConnectionsWithPipe(id);
        }

        delete_objects(pipes, selected_ids);
        std::cout << "Трубы удалены. Связанные соединения разорваны." << std::endl;
    }
    else {
        std::cout << "Удаление отменено" << std::endl;
    }
}

void Network::DeleteStations() {
    if (stations.empty()) {
        std::cout << "Нет станций для удаления" << std::endl;
        return;
    }

    std::cout << "\n=== Удаление станций ===" << std::endl;
    std::set<int> found_ids = FindStations();

    if (found_ids.empty()) {
        std::cout << "Станции не найдены" << std::endl;
        return;
    }

    std::set<int> selected_ids = select_objects_for_editing(stations, found_ids);

    if (selected_ids.empty()) {
        std::cout << "Не выбрано ни одной станции для удаления" << std::endl;
        return;
    }

    std::cout << "Вы уверены, что хотите удалить " << selected_ids.size()
        << " станций? (1 - да, 0 - нет): " << std::endl;
    std::cout << "Внимание: Будут также удалены все связанные трубы и соединения!" << std::endl;

    if (safe_input<int>(0, 1) == 1) {
        for (int station_id : selected_ids) {
            removeConnectionsWithStation(station_id);
            updatePipeConnectionsAfterStationDeletion(station_id);
        }

        delete_objects(stations, selected_ids);
        std::cout << "Станции удалены. Связанные трубы и соединения также удалены." << std::endl;
    }
    else {
        std::cout << "Удаление отменено" << std::endl;
    }
}

void Network::removeConnectionsWithPipe(int pipeId) {
    if (!pipes.count(pipeId)) return;

    const Pipe& pipe = pipes.at(pipeId);
    int start = pipe.getStart();
    int end = pipe.getEnd();

    if (start != 0 && end != 0) {
        connections.erase(std::make_pair(start, end));
    }
}

void Network::removeConnectionsWithStation(int stationId) {
    std::set<std::pair<int, int>> connections_to_remove;

    for (const auto& conn : connections) {
        if (conn.first == stationId || conn.second == stationId) {
            connections_to_remove.insert(conn);
        }
    }

    for (const auto& conn : connections_to_remove) {
        connections.erase(conn);
    }
}

void Network::updatePipeConnectionsAfterStationDeletion(int deletedStationId) {
    for (auto& pipe_pair : pipes) {
        Pipe& pipe = pipe_pair.second;
        if (pipe.getStart() == deletedStationId || pipe.getEnd() == deletedStationId) {
            pipe.SetStartEnd(0, 0);
        }
    }
}

void Network::SaveToFile(const std::string& filename) {
    std::ofstream ofs(filename);
    if (!(pipes.empty() && stations.empty())) {
        for (const auto& pipe_pair : pipes)
            pipe_pair.second.SavePipe(ofs);
        for (const auto& station_pair : stations)
            station_pair.second.SaveStation(ofs);
        std::cout << "Данные сохранены в файл: " << filename << std::endl;
    }
    else
        std::cout << "Нет объектов для сохранения" << std::endl;
}

void Network::LoadFromFile(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cout << "Файл не найден: " << filename << std::endl;
        return;
    }

    pipes.clear();
    stations.clear();
    connections.clear();

    std::string line;
    while (ifs >> line) {
        if (line == "P") {
            Pipe pipe;
            pipe.LoadPipe(ifs);
            pipes.emplace(pipe.getId(), pipe);

            if (pipe.getStart() != 0 && pipe.getEnd() != 0) {
                connections.insert(std::make_pair(pipe.getStart(), pipe.getEnd()));
            }
        }
        else if (line == "S") {
            Station station;
            station.LoadStation(ifs);
            stations.emplace(station.getId(), station);
        }
    }
    std::cout << "Данные загружены из файла: " << filename << std::endl;
    std::cout << "Загружено труб: " << pipes.size() << ", станций: " << stations.size()
        << ", соединений: " << connections.size() << std::endl;
}

void Network::ConnectStations() {
    if (stations.size() < 2) {
        std::cout << "Необходимо минимум 2 станции для соединения" << std::endl;
        return;
    }

    std::cout << "\n=== Соединение станций ===" << std::endl;

    std::cout << "\nСписок станций:" << std::endl;
    for (const auto& station_pair : stations) {
        std::cout << "ID: " << station_pair.first << " - "
            << station_pair.second.getName() << std::endl;
    }

    std::cout << "\nВыберите ID станции-источника: ";
    int start = check_station_id(stations);

    std::cout << "Выберите ID станции-приемника: ";
    int end = check_station_id(stations, start);

    if (connections.count(std::make_pair(start, end)) > 0) {
        std::cout << "Соединение уже существует!" << std::endl;
        return;
    }

    std::cout << "\nДоступные диаметры: 500, 700, 1000, 1400 мм" << std::endl;
    int pipe_id = FindPipeByDiameter();

    pipes.at(pipe_id).SetStartEnd(start, end);
    connections.insert(std::make_pair(start, end));

    std::cout << "\nСоединение создано!" << std::endl;
    std::cout << "Станция " << start << " -> Станция " << end
        << " (Труба ID: " << pipe_id << ")" << std::endl;
}

void Network::DisconnectStations() {
    if (connections.empty()) {
        std::cout << "Нет соединений для разрыва" << std::endl;
        return;
    }

    std::cout << "\n=== Разрыв соединений ===" << std::endl;
    std::cout << "Список соединений:" << std::endl;

    int index = 1;
    std::vector<std::pair<int, int>> conn_list;

    for (const auto& conn : connections) {
        std::cout << index << ". Станция " << conn.first << " -> Станция " << conn.second << std::endl;
        conn_list.push_back(conn);
        index++;
    }

    std::cout << "\nВыберите номер соединения для разрыва (0 для отмены): ";
    int choice = safe_input<int>(0, static_cast<int>(conn_list.size()));

    if (choice == 0) {
        std::cout << "Отменено" << std::endl;
        return;
    }

    const auto& conn_to_remove = conn_list[choice - 1];

    for (auto& pipe_pair : pipes) {
        Pipe& pipe = pipe_pair.second;
        if (pipe.getStart() == conn_to_remove.first &&
            pipe.getEnd() == conn_to_remove.second) {
            pipe.SetStartEnd(0, 0);
            break;
        }
    }

    connections.erase(conn_to_remove);
    std::cout << "Соединение разорвано" << std::endl;
}

int Network::FindPipeByDiameter() {
    const std::vector<int> allowed_diameters = { 500, 700, 1000, 1400 };

    while (true) {
        std::cout << "Введите диаметр трубы: ";
        int diameter = safe_input<int>(500, 1400);

        bool is_allowed = false;
        for (int d : allowed_diameters) {
            if (d == diameter) {
                is_allowed = true;
                break;
            }
        }

        if (!is_allowed) {
            std::cout << "Диаметр должен быть 500, 700, 1000 или 1400 мм!" << std::endl;
            continue;
        }

        for (auto& pipe_pair : pipes) {
            if (pipe_pair.second.check_pipe_by_diameter(diameter) &&
                pipe_pair.second.getStart() == 0) {
                std::cout << "Найдена свободная труба ID: " << pipe_pair.first << std::endl;
                return pipe_pair.first;
            }
        }

        std::cout << "Свободной трубы с диаметром " << diameter << " мм не найдено." << std::endl;
        std::cout << "Создать новую трубу? (1 - да, 0 - ввести другой диаметр): ";

        if (safe_input<int>(0, 1) == 1) {
            Pipe pipe;
            std::cin >> pipe;
            pipes.emplace(pipe.getId(), pipe);
            std::cout << "Создана новая труба ID: " << pipe.getId() << std::endl;
            return pipe.getId();
        }
    }
}

void Network::TopologicSort() {
    if (stations.empty()) {
        std::cout << "Нет станций для сортировки" << std::endl;
        return;
    }

    if (connections.empty()) {
        std::cout << "Нет соединений между станциями" << std::endl;
        return;
    }

    std::cout << "\n=== Топологическая сортировка ===" << std::endl;

    std::unordered_map<int, std::vector<int>> graph;
    std::unordered_map<int, int> in_degree;

    for (const auto& station_pair : stations) {
        int id = station_pair.first;
        graph[id] = std::vector<int>();
        in_degree[id] = 0;
    }

    for (const auto& conn : connections) {
        int from = conn.first;
        int to = conn.second;

        if (stations.count(from) && stations.count(to)) {
            graph[from].push_back(to);
            in_degree[to]++;
        }
    }

    std::vector<int> sorted;
    std::queue<int> zero_in_degree;

    for (const auto& degree_pair : in_degree) {
        if (degree_pair.second == 0) {
            zero_in_degree.push(degree_pair.first);
        }
    }

    while (!zero_in_degree.empty()) {
        int current = zero_in_degree.front();
        zero_in_degree.pop();
        sorted.push_back(current);

        for (int neighbor : graph[current]) {
            in_degree[neighbor]--;
            if (in_degree[neighbor] == 0) {
                zero_in_degree.push(neighbor);
            }
        }
    }

    if (sorted.size() != stations.size()) {
        std::cout << "В графе обнаружен цикл! Топологическая сортировка невозможна." << std::endl;

        std::set<int> all_stations;
        for (const auto& station_pair : stations) {
            all_stations.insert(station_pair.first);
        }

        std::set<int> sorted_set(sorted.begin(), sorted.end());
        std::vector<int> cycle_stations;

        std::set_difference(all_stations.begin(), all_stations.end(),
            sorted_set.begin(), sorted_set.end(),
            std::back_inserter(cycle_stations));

        if (!cycle_stations.empty()) {
            std::cout << "Станции, образующие цикл: ";
            for (size_t i = 0; i < cycle_stations.size(); ++i) {
                std::cout << cycle_stations[i];
                if (i != cycle_stations.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << std::endl;
        }
        return;
    }

    std::cout << "Топологический порядок станций: " << std::endl;
    for (size_t i = 0; i < sorted.size(); ++i) {
        int station_id = sorted[i];
        auto it = stations.find(station_id);
        if (it != stations.end()) {
            std::cout << (i + 1) << ". Станция " << station_id << " - " << it->second.getName() << std::endl;
        }
        else {
            std::cout << (i + 1) << ". Станция " << station_id << " (не найдена)" << std::endl;
        }
    }
}