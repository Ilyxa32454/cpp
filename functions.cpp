#include "functions.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>

using namespace std;

void addPipe(unordered_map<int, Pipe>& pipes) {
    cout << "\n--- ДОБАВЛЕНИЕ ТРУБЫ ---" << endl;
    Pipe pipe;
    pipe.readFromConsole();
    pipes[pipe.getId()] = pipe;
    cout << "Труба успешно добавлена (ID: " << pipe.getId() << ")" << endl;
    cerr << "Труба успешно добавлена (ID: " << pipe.getId() << ")" << endl;
}

void addCS(unordered_map<int, CS>& stations) {
    cout << "\n--- ДОБАВЛЕНИЕ КС ---" << endl;
    CS station;
    station.readFromConsole();
    stations[station.getId()] = station;
    cout << "КС успешно добавлена (ID: " << station.getId() << ")" << endl;
    cerr << "КС успешно добавлена (ID: " << station.getId() << ")" << endl;
}

void viewAllObjects(const unordered_map<int, Pipe>& pipes, const unordered_map<int, CS>& stations) {
    cout << "\n========== ВСЕ ОБЪЕКТЫ ==========" << endl;

    cout << "\nТРУБЫ (" << pipes.size() << ")" << endl;
    if (pipes.empty()) {
        cout << "   Трубы отсутствуют" << endl;
    } else {
        for (const auto& pair : pipes) {
            cout << "   ";
            pair.second.display();
        }
    }

    cout << "\nКОМПРЕССОРНЫЕ СТАНЦИИ (" << stations.size() << ")" << endl;
    if (stations.empty()) {
        cout << "   КС отсутствуют" << endl;
    } else {
        for (const auto& pair : stations) {
            cout << "   ";
            pair.second.display();
        }
    }
    
    cerr << "Просмотр всех объектов" << endl;
}

void editPipe(unordered_map<int, Pipe>& pipes) {
    cout << "\n--- РЕДАКТИРОВАНИЕ ТРУБЫ ---" << endl;
    
    if (pipes.empty()) {
        cout << "Нет добавленных труб" << endl;
        cerr << "Ошибка: нет добавленных труб" << endl;
        return;
    }

    cout << "\nДоступные трубы:" << endl;
    for (const auto& pair : pipes) {
        cout << "   ";
        pair.second.display();
    }

    cout << "\nВведите ID трубы для редактирования: ";
    int id;
    if (cin >> id) {
        auto it = pipes.find(id);
        if (it != pipes.end()) {
            cout << "\nТекущий статус: " << (it->second.isWorking() ? "Работает" : "В ремонте") << endl;
            cout << "Выберите новый статус:" << endl;
            cout << "1) Работает" << endl;
            cout << "0) В ремонте" << endl;
            cout << "Ваш выбор: ";
            
            int status;
            cin >> status;
            
            if (status == 0 || status == 1) {
                it->second.setWork(status == 1);
                cout << "Статус трубы обновлен" << endl;
                cerr << "Статус трубы ID " << id << " изменен на " << (status == 1 ? "работает" : "в ремонте") << endl;
            } else {
                cout << "Некорректный ввод" << endl;
            }
        } else {
            cout << "Труба с ID " << id << " не найдена" << endl;
            cerr << "Ошибка: труба ID " << id << " не найдена" << endl;
        }
    } else {
        cout << "Неверный ввод ID" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void editCS(unordered_map<int, CS>& stations) {
    cout << "\n--- РЕДАКТИРОВАНИЕ КС ---" << endl;
    
    if (stations.empty()) {
        cout << "Нет добавленных КС" << endl;
        cerr << "Ошибка: нет добавленных КС" << endl;
        return;
    }

    cout << "\nДоступные КС:" << endl;
    for (const auto& pair : stations) {
        cout << "   ";
        pair.second.display();
    }

    cout << "\nВведите ID КС для редактирования: ";
    int id;
    if (cin >> id) {
        auto it = stations.find(id);
        if (it != stations.end()) {
            cout << "\nТекущие данные КС:" << endl;
            cout << "   Всего цехов: " << it->second.getChex() << endl;
            cout << "   Рабочих цехов: " << it->second.getWorkchex() << endl;
            
            cout << "\nВыберите действие:" << endl;
            cout << "1) Запустить цех" << endl;
            cout << "2) Остановить цех" << endl;
            cout << "Ваш выбор: ";
            
            int cmd;
            cin >> cmd;
            
            if (cmd == 1) {
                it->second.startWorkshop();
                cerr << "Запущен цех на КС ID " << id << endl;
            } else if (cmd == 2) {
                it->second.stopWorkshop();
                cerr << "Остановлен цех на КС ID " << id << endl;
            } else {
                cout << "Некорректная команда" << endl;
            }
        } else {
            cout << "КС с ID " << id << " не найдена" << endl;
            cerr << "Ошибка: КС ID " << id << " не найдена" << endl;
        }
    } else {
        cout << "Неверный ввод ID" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void deleteObject(unordered_map<int, Pipe>& pipes, unordered_map<int, CS>& stations) {
    cout << "\n--- УДАЛЕНИЕ ОБЪЕКТА ---" << endl;
    
    cout << "\nТекущие объекты:" << endl;
    for (const auto& pair : pipes) {
        cout << "   ";
        pair.second.display();
    }
    for (const auto& pair : stations) {
        cout << "   ";
        pair.second.display();
    }
    
    cout << "\nВведите ID объекта для удаления: ";
    int id;
    if (cin >> id) {
        auto pipeIt = pipes.find(id);
        if (pipeIt != pipes.end()) {
            pipes.erase(pipeIt);
            cout << "Труба ID:" << id << " удалена" << endl;
            cerr << "Удалена труба ID " << id << endl;
            return;
        }

        auto stationIt = stations.find(id);
        if (stationIt != stations.end()) {
            stations.erase(stationIt);
            cout << "КС ID:" << id << " удалена" << endl;
            cerr << "Удалена КС ID " << id << endl;
            return;
        }

        cout << "Объект с ID:" << id << " не найден" << endl;
        cerr << "Ошибка: объект ID " << id << " не найден" << endl;
    } else {
        cout << "Неверный ввод ID" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void searchPipesByName(const unordered_map<int, Pipe>& pipes) {
    cout << "\n--- ПОИСК ТРУБ ПО НАЗВАНИЮ ---" << endl;
    cout << "Введите название для поиска: ";
    string name;
    getline(cin >> ws, name);

    vector<const Pipe*> found;
    for (const auto& pair : pipes) {
        if (pair.second.getName().find(name) != string::npos) {
            found.push_back(&pair.second);
        }
    }

    cout << "\nНайдено труб: " << found.size() << endl;
    if (found.empty()) {
        cout << "Трубы не найдены" << endl;
    } else {
        for (const auto* pipe : found) {
            cout << "   ";
            pipe->display();
        }
    }
    cerr << "Поиск труб по названию: \"" << name << "\", найдено " << found.size() << endl;
}

void searchPipesByStatus(const unordered_map<int, Pipe>& pipes) {
    cout << "\n--- ПОИСК ТРУБ ПО СТАТУСУ ---" << endl;
    cout << "Искать работающие трубы? (1 - Да, 0 - Нет, в ремонте): ";
    int status;
    if (cin >> status) {
        bool working = (status == 1);
        vector<const Pipe*> found;
        
        for (const auto& pair : pipes) {
            if (pair.second.isWorking() == working) {
                found.push_back(&pair.second);
            }
        }

        cout << "\nНайдено труб: " << found.size() << endl;
        if (found.empty()) {
            cout << "Трубы не найдены" << endl;
        } else {
            for (const auto* pipe : found) {
                cout << "   ";
                pipe->display();
            }
        }
        cerr << "Поиск труб по статусу: " << (working ? "работающие" : "в ремонте") << ", найдено " << found.size() << endl;
    } else {
        cout << "Неверный ввод" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void searchCSByName(const unordered_map<int, CS>& stations) {
    cout << "\n--- ПОИСК КС ПО НАЗВАНИЮ ---" << endl;
    cout << "Введите название для поиска: ";
    string name;
    getline(cin >> ws, name);

    vector<const CS*> found;
    for (const auto& pair : stations) {
        if (pair.second.getName().find(name) != string::npos) {
            found.push_back(&pair.second);
        }
    }

    cout << "\nНайдено КС: " << found.size() << endl;
    if (found.empty()) {
        cout << "КС не найдены" << endl;
    } else {
        for (const auto* station : found) {
            cout << "   ";
            station->display();
        }
    }
    cerr << "Поиск КС по названию: \"" << name << "\", найдено " << found.size() << endl;
}

void searchCSByInactivePercentage(const unordered_map<int, CS>& stations) {
    cout << "\n--- ПОИСК КС ПО ПРОЦЕНТУ НЕЗАДЕЙСТВОВАННЫХ ЦЕХОВ ---" << endl;
    cout << "Введите минимальный процент незадействованных цехов: ";
    double minPercent;
    if (cin >> minPercent) {
        vector<const CS*> found;
        
        for (const auto& pair : stations) {
            if (pair.second.getInactivePercentage() >= minPercent) {
                found.push_back(&pair.second);
            }
        }

        cout << "\nНайдено КС: " << found.size() << endl;
        if (found.empty()) {
            cout << "КС не найдены" << endl;
        } else {
            for (const auto* station : found) {
                cout << "   ";
                station->display();
            }
        }
        cerr << "Поиск КС по проценту >= " << minPercent << "%, найдено " << found.size() << endl;
    } else {
        cout << "Неверный ввод" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void batchEditPipes(unordered_map<int, Pipe>& pipes) {
    if (pipes.empty()) {
        cout << "\nНет добавленных труб" << endl;
        cerr << "Ошибка: нет добавленных труб" << endl;
        return;
    }

    cout << "\n--- ПАКЕТНОЕ РЕДАКТИРОВАНИЕ ТРУБ ---" << endl;
    cout << "Выберите фильтр для поиска труб:" << endl;
    cout << "1. По названию" << endl;
    cout << "2. По статусу" << endl;
    cout << "3. Все трубы" << endl;
    cout << "Выберите фильтр: ";

    int filterChoice;
    cin >> filterChoice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<int> foundIds;

    switch (filterChoice) {
        case 1: {
            cout << "Введите название для поиска: ";
            string name;
            getline(cin, name);
            for (const auto& pair : pipes) {
                if (pair.second.getName().find(name) != string::npos) {
                    foundIds.push_back(pair.first);
                }
            }
            break;
        }
        case 2: {
            cout << "Искать работающие трубы? (1 - Да, 0 - Нет, в ремонте): ";
            int status;
            cin >> status;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            bool working = (status == 1);
            for (const auto& pair : pipes) {
                if (pair.second.isWorking() == working) {
                    foundIds.push_back(pair.first);
                }
            }
            break;
        }
        case 3:
            for (const auto& pair : pipes) {
                foundIds.push_back(pair.first);
            }
            break;
        default:
            cout << "Неверный выбор" << endl;
            return;
    }

    if (foundIds.empty()) {
        cout << "Трубы не найдены" << endl;
        cerr << "Поиск не дал результатов" << endl;
        return;
    }

    cout << "\nНайдено труб: " << foundIds.size() << endl;
    for (int id : foundIds) {
        cout << "   ";
        pipes[id].display();
    }

    cout << "\nВыберите действие:" << endl;
    cout << "1. Изменить статус у всех найденных" << endl;
    cout << "2. Выбрать конкретные трубы для редактирования" << endl;
    cout << "3. Удалить все найденные трубы" << endl;
    cout << "Выберите действие: ";

    int actionChoice;
    cin >> actionChoice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (actionChoice == 1) {
        cout << "Установить статус 'работает'? (1 - Да, 0 - Нет, в ремонте): ";
        int status;
        if (cin >> status) {
            bool working = (status == 1);
            for (int id : foundIds) {
                pipes[id].setWork(working);
            }
            cout << "Статус изменен у " << foundIds.size() << " труб" << endl;
            cerr << "Пакетное редактирование: изменен статус у " << foundIds.size() << " труб" << endl;
        }
    } 
    else if (actionChoice == 2) {
        cout << "Введите ID труб через пробел для редактирования: ";
        string line;
        getline(cin, line);
        istringstream ss(line);
        
        vector<int> selectedIds;
        int id;
        while (ss >> id) {
            selectedIds.push_back(id);
        }

        cout << "Установить статус 'работает'? (1 - Да, 0 - Нет, в ремонте): ";
        int status;
        if (cin >> status) {
            bool working = (status == 1);
            int changed = 0;
            for (int selId : selectedIds) {
                if (find(foundIds.begin(), foundIds.end(), selId) != foundIds.end()) {
                    pipes[selId].setWork(working);
                    changed++;
                }
            }
            cout << "Статус изменен у " << changed << " труб" << endl;
            cerr << "Пакетное редактирование: изменен статус у " << changed << " выбранных труб" << endl;
        }
    }
    else if (actionChoice == 3) {
        for (int id : foundIds) {
            pipes.erase(id);
        }
        cout << "Удалено труб: " << foundIds.size() << endl;
        cerr << "Пакетное редактирование: удалено " << foundIds.size() << " труб" << endl;
    }
    else {
        cout << "Неверный выбор" << endl;
    }
}

void saveToFile(const unordered_map<int, Pipe>& pipes, const unordered_map<int, CS>& stations) {
    cout << "\n--- СОХРАНЕНИЕ В ФАЙЛ ---" << endl;
    cout << "Введите имя файла для сохранения: ";
    string filename;
    getline(cin >> ws, filename);

    ofstream outFile(filename);
    if (!outFile) {
        cout << "Ошибка создания файла" << endl;
        cerr << "Ошибка сохранения в файл " << filename << endl;
        return;
    }

    outFile << pipes.size() << endl;
    for (const auto& pair : pipes) {
        pair.second.saveToFile(outFile);
    }
    
    outFile << stations.size() << endl;
    for (const auto& pair : stations) {
        pair.second.saveToFile(outFile);
    }
    
    outFile.close();
    cout << "Данные успешно сохранены в " << filename << endl;
    cerr << "Сохранено в файл " << filename << ": " << pipes.size() << " труб, " << stations.size() << " КС" << endl;
}

void loadFromFile(unordered_map<int, Pipe>& pipes, unordered_map<int, CS>& stations) {
    cout << "\n--- ЗАГРУЗКА ИЗ ФАЙЛА ---" << endl;
    cout << "Введите имя файла для загрузки: ";
    string filename;
    getline(cin >> ws, filename);

    ifstream inFile(filename);
    if (!inFile) {
        cout << "Ошибка открытия файла" << endl;
        cerr << "Ошибка загрузки из файла " << filename << endl;
        return;
    }

    pipes.clear();
    stations.clear();

    int pipeCount, stationCount;
    
    inFile >> pipeCount;
    inFile.ignore();
    for (int i = 0; i < pipeCount; i++) {
        string type;
        getline(inFile, type);
        Pipe pipe;
        pipe.loadFromFile(inFile);
        pipes[pipe.getId()] = pipe;
    }
    
    inFile >> stationCount;
    inFile.ignore();
    for (int i = 0; i < stationCount; i++) {
        string type;
        getline(inFile, type);
        CS station;
        station.loadFromFile(inFile);
        stations[station.getId()] = station;
    }
    
    inFile.close();
    cout << "Данные успешно загружены из " << filename << endl;
    cout << "Загружено труб: " << pipeCount << ", КС: " << stationCount << endl;
    cerr << "Загружено из файла " << filename << ": " << pipeCount << " труб, " << stationCount << " КС" << endl;
}