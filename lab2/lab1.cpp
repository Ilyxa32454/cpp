#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <climits>

#include "pipe.h"     // важно: совпадает по регистру с именем файла
#include "station.h"
#include "utils.h"    // тоже с заглавной U, как в ls
#include "manager.h"

using namespace std;

int main()
{
    redirect_output_wrapper cerr_out(cerr);

    // Генерируем строку с текущим временем для имени лог-файла
    std::time_t t = std::time(nullptr);
    std::tm* tm = std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(tm, "%d_%m_%Y_%H_%M_%S");
    std::string time = oss.str();

    ofstream logfile("log_" + time);
    if (logfile)
        cerr_out.redirect(logfile);

    Manager manager;
    string filename;

    while (true) {
        manager.display_main_menu();
        int choice = GetCorrectNumber(0, 13);

        switch (choice) {
            case 0:
                return 0;

            case 1:
                manager.add_pipe();
                break;

            case 2:
                manager.add_station();
                break;

            case 3:
                manager.display_all_objects();
                break;

            case 4: {
                cout << "Enter pipe ID to edit: ";
                int id = GetCorrectNumber(1, INT_MAX);
                manager.edit_pipe(id);
                break;
            }

            case 5: {
                cout << "Enter station ID to edit: ";
                int id = GetCorrectNumber(1, INT_MAX);
                manager.edit_station(id);
                break;
            }

            case 6: {
                cout << "Enter pipe ID to delete: ";
                int id = GetCorrectNumber(1, INT_MAX);
                manager.delete_pipe(id);
                break;
            }

            case 7: {
                cout << "Enter station ID to delete: ";
                int id = GetCorrectNumber(1, INT_MAX);
                manager.delete_station(id);
                break;
            }

            case 8:
                manager.display_all_pipes();
                break;

            case 9:
                manager.display_all_stations();
                break;

            case 10:
                cout << "Enter filename to save: ";
                INPUT_LINE(cin, filename);
                manager.save_to_file(filename);
                break;

            case 11:
                cout << "Enter filename to load: ";
                INPUT_LINE(cin, filename);
                manager.load_from_file(filename);
                break;

            case 12:
                manager.handle_search();
                break;

            case 13:
                manager.handle_pipes_batch_menu();
                break;
        }
    }
}