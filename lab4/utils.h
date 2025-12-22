#pragma once
#include <iostream>
#include <unordered_map>
#include "pipe.h"
#include "station.h"
#include <set>
#include <sstream>

#define INPUT_LINE(in, str) getline(in>>std::ws, str); \
                        std::cerr << str << std::endl

class redirect_output_wrapper
{
    std::ostream& stream;
    std::streambuf* const old_buf;

public:
    redirect_output_wrapper(std::ostream& src)
        : old_buf(src.rdbuf()), stream(src)
    {
    }

    ~redirect_output_wrapper() {
        stream.rdbuf(old_buf);
    }

    void redirect(std::ostream& dest) {
        stream.rdbuf(dest.rdbuf());
    }
};

template <typename Par, typename Obj>
using Filter = bool(*)(const Obj& obj, Par param);

template <typename Par, typename Obj>
std::set<int> find_by_filter(const std::unordered_map<int, Obj>& objs, Filter<Par, Obj> func, Par param) {
    std::set<int> result;
    for (const auto& obj_pair : objs) {
        if (func(obj_pair.second, param)) {
            std::cout << obj_pair.second;
            result.insert(obj_pair.first);
        }
    }
    return result;
}

// Простое объявление без специализаций
template <typename T>
T safe_input(T min, T max, std::string message = "None");

// Явная специализация только для char
template <>
char safe_input<char>(char min, char max, std::string message);

template <typename Obj>
std::set<int> select_objects_for_editing(
    std::unordered_map<int, Obj>& objects,
    std::set<int> filtered_ids)
{
    if (filtered_ids.empty()) {
        std::cout << "По заданному фильтру объекты не найдены." << std::endl;
        return {};
    }

    std::cout << "Ввод id по одному в строке. Введите 0:\n"
        << " - если 0 введён первым — выберутся ВСЕ отфильтрованные объекты.\n"
        << " - иначе вводите id по одному и завершите ввод 0.\n";

    std::set<int> final_ids;
    int id = safe_input<int>(0, 1000, "id (0 для выбора всех)");

    if (id == 0) {
        return filtered_ids;
    }

    if (filtered_ids.count(id) != 0) {
        final_ids.insert(id);
        std::cout << "Объект с id " << id << " добавлен в список для редактирования." << std::endl;
    }
    else {
        std::cout << "ID " << id << " не найден среди отфильтрованных объектов." << std::endl;
    }

    while (true) {
        std::cout << "Введите следующий id (0 для завершения): ";
        id = safe_input<int>(0, 1000, "id (0 для завершения)");

        if (id == 0) {
            break;
        }

        if (filtered_ids.count(id) != 0) {
            final_ids.insert(id);
            std::cout << "Объект с id " << id << " добавлен в список для редактирования." << std::endl;
        }
        else {
            std::cout << "ID " << id << " не найден среди отфильтрованных объектов." << std::endl;
        }
    }

    if (final_ids.empty()) {
        std::cout << "Не выбрано ни одного объекта для редактирования." << std::endl;
    }
    else {
        std::cout << "Выбрано объектов для редактирования: " << final_ids.size() << std::endl;
    }

    return final_ids;
}

template <typename Obj>
void delete_objects(std::unordered_map<int, Obj>& objects,
    const std::set<int>& ids)
{
    for (int id : ids) {
        objects.erase(id);
    }
    std::cout << "Удалено объектов: " << ids.size() << std::endl;
}

template <typename Obj>
void print_objs(const std::unordered_map<int, Obj>& objs, std::string obj_name) {
    if (!objs.empty()) {
        std::cout << "Список " << obj_name << " (" << objs.size() << " шт.):" << std::endl;
        for (const auto& obj_pair : objs) {
            std::cout << "ID: " << obj_pair.first << std::endl;
            std::cout << obj_pair.second;
        }
    }
    else {
        std::cout << "Ни одной " << obj_name << " не было добавлено" << std::endl;
    }
}

inline int check_station_id(const std::unordered_map<int, Station>& stations, int prev_id = 0) {
    while (true) {
        std::cout << "Введите id станции: ";
        int id = safe_input<int>(1, 1000);

        if (stations.find(id) == stations.end()) {
            std::cout << "Станции с id " << id << " не существует." << std::endl;
            continue;
        }

        if (id == prev_id) {
            std::cout << "Нельзя выбрать ту же станцию, что и предыдущую." << std::endl;
            continue;
        }

        return id;
    }
}