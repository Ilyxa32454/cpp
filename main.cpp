#include <iostream>
#include <unordered_map>
#include <chrono>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "pipe.h"
#include "cs.h"
#include "functions.h"

using namespace std;
using namespace chrono;

int nextId = 1;

string getCurrentTimeString() {
    auto now = system_clock::now();
    time_t now_time = system_clock::to_time_t(now);
    tm* local_time = localtime(&now_time);
    
    char buffer[100];
    strftime(buffer, sizeof(buffer), "%d_%m_%Y_%H_%M_%S", local_time);
    
    return string(buffer);
}

int main() {
    unordered_map<int, Pipe> pipes;
    unordered_map<int, CS> stations;
    int choice;

    do {
        string timeStr = getCurrentTimeString();
        string logFilename = "log_" + timeStr + ".txt";
        
        ofstream logfile(logFilename);
        streambuf* old_cerr_buf = cerr.rdbuf();
        
        if (logfile.is_open()) {
            cerr.rdbuf(logfile.rdbuf());
        }

        cout << "\n========== СИСТЕМА УПРАВЛЕНИЯ ТРУБОПРОВОДОМ ==========" << endl;
        cout << "1. Добавить трубу" << endl;
        cout << "2. Добавить КС" << endl;
        cout << "3. Просмотр всех объектов" << endl;
        cout << "4. Редактировать трубу" << endl;
        cout << "5. Редактировать КС" << endl;
        cout << "6. Удалить объект" << endl;
        cout << "7. Поиск и фильтрация" << endl;
        cout << "8. Пакетное редактирование труб" << endl;
        cout << "9. Сохранить в файл" << endl;
        cout << "10. Загрузить из файла" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите действие: ";
        
        if (!(cin >> choice)) {
            cerr << "Неверный ввод" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cerr.rdbuf(old_cerr_buf);
            if (logfile.is_open()) logfile.close();
            continue;
        }

        switch (choice) {
            case 1:
                addPipe(pipes);
                break;
            case 2:
                addCS(stations);
                break;
            case 3:
                viewAllObjects(pipes, stations);
                break;
            case 4:
                editPipe(pipes);
                break;
            case 5:
                editCS(stations);
                break;
            case 6:
                deleteObject(pipes, stations);
                break;
            case 7: {
                int searchChoice;
                do {
                    cout << "\n========== ПОИСК И ФИЛЬТРАЦИЯ ==========" << endl;
                    cout << "1. Поиск труб по названию" << endl;
                    cout << "2. Поиск труб по статусу" << endl;
                    cout << "3. Поиск КС по названию" << endl;
                    cout << "4. Поиск КС по проценту незадействованных цехов" << endl;
                    cout << "0. Назад" << endl;
                    cout << "Выберите действие: ";
                    
                    if (cin >> searchChoice) {
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        switch (searchChoice) {
                            case 1:
                                searchPipesByName(pipes);
                                break;
                            case 2:
                                searchPipesByStatus(pipes);
                                break;
                            case 3:
                                searchCSByName(stations);
                                break;
                            case 4:
                                searchCSByInactivePercentage(stations);
                                break;
                            case 0:
                                break;
                            default:
                                cerr << "Неверный выбор в меню поиска" << endl;
                        }
                    } else {
                        cerr << "Неверный ввод в меню поиска" << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                } while (searchChoice != 0);
                break;
            }
            case 8:
                batchEditPipes(pipes);
                break;
            case 9:
                saveToFile(pipes, stations);
                break;
            case 10:
                loadFromFile(pipes, stations);
                break;
            case 0:
                cerr << "Выход из программы" << endl;
                break;
            default:
                cerr << "Неверный выбор в главном меню" << endl;
        }
        
        cerr.rdbuf(old_cerr_buf);
        if (logfile.is_open()) logfile.close();
        
    } while (choice != 0);

    return 0;
}