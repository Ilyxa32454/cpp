#include <iostream>
#include <fstream>
#include <limits>
#include "pipe.h"
#include "station.h"
#include "utils.h"
#include "network.h"
#include <chrono>
#include <unordered_map>
#include <set>
#include <iomanip>
#include <sstream>
#include <windows.h>

using namespace std;
using namespace chrono;

// Установка русской кодировки для Windows консоли
void setRussianEncoding() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    system("chcp 1251 > nul");
}

string get_current_time_string() {
    auto now = system_clock::now();
    time_t now_time = system_clock::to_time_t(now);
    tm tm_buf;
    localtime_s(&tm_buf, &now_time);

    ostringstream oss;
    oss << put_time(&tm_buf, "%d_%m_%Y %H_%M_%S");
    return oss.str();
}

void print_menu() {
    cout << "\nГазотранспортная сеть" << endl;
    cout << "====================" << endl;
    cout << "1. Добавить трубу" << endl;
    cout << "2. Добавить компрессорную станцию (КС)" << endl;
    cout << "3. Просмотр всех объектов" << endl;
    cout << "4. Поиск труб" << endl;
    cout << "5. Поиск станций" << endl;
    cout << "6. Редактировать трубы" << endl;
    cout << "7. Редактировать станции" << endl;
    cout << "8. Удалить трубы" << endl;
    cout << "9. Удалить станции" << endl;
    cout << "10.Соединить станции" << endl;
    cout << "11.Разорвать соединение" << endl;
    cout << "12.Топологическая сортировка" << endl;
    cout << "13.Сохранить в файл" << endl;
    cout << "14.Загрузить из файла" << endl;
    cout << "15.Расчет максимального потока" << endl;  // НОВЫЙ ПУНКТ
    cout << "16.Расчет кратчайшего пути" << endl;      // НОВЫЙ ПУНКТ
    cout << "0. Выход" << endl;
    cout << "\nВаш выбор: ";
}

int main() {
    setRussianEncoding();

    redirect_output_wrapper cerr_log(cerr);
    string time = get_current_time_string();
    ofstream logfile("logs/log_" + time + ".txt");
    if (logfile)
        cerr_log.redirect(logfile);

    Network network;

    while (true) {
        print_menu();
        switch (safe_input(0, 16)) {  // Изменено с 14 на 16
        case 1: {
            network.AddPipe();
            break;
        }
        case 2: {
            network.AddStation();
            break;
        }
        case 3: {
            network.PrintObjs();
            break;
        }
        case 4: {
            network.FindPipes();
            break;
        }
        case 5: {
            network.FindStations();
            break;
        }
        case 6: {
            network.PipesButchEditing();
            break;
        }
        case 7: {
            network.StationsButchEditing();
            break;
        }
        case 8: {
            network.DeletePipes();
            break;
        }
        case 9: {
            network.DeleteStations();
            break;
        }
        case 10: {
            network.ConnectStations();
            break;
        }
        case 11: {
            network.DisconnectStations();
            break;
        }
        case 12: {
            network.TopologicSort();
            break;
        }
        case 13: {
            cout << "Введите имя файла для сохранения: ";
            string filename;
            INPUT_LINE(cin, filename);
            network.SaveToFile(filename);
            break;
        }
        case 14: {
            cout << "Введите имя файла для загрузки: ";
            string filename;
            INPUT_LINE(cin, filename);
            network.LoadFromFile(filename);
            break;
        }
        case 15: {  // НОВЫЙ КЕЙС
            network.CalculateMaxFlow();
            break;
        }
        case 16: {  // НОВЫЙ КЕЙС
            network.CalculateShortestPath();
            break;
        }
        case 0: {
            return 0;
        }
        }
    }

    return 0;
}