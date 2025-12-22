#include <iostream>  // Для ввода-вывода 
#include <fstream>   // Для работы с файлами 
#include <string>    // Для работы со строками 

using namespace std; // Использование стандартного пространства имен

// Структура для представления трубы
struct Pipe {
    string name;     // Название трубы
    double lenght;   // Длина трубы 
    int diametr;     // Диаметр трубы 
    bool inRepair;   // Состояние трубы 
};

// Структура для представления компрессорной станции
struct CompressorStation {
    string name;           // Название станции
    int numberWorkshop;    // Общее количество цехов
    int workingWorkshop;   // Количество работающих цехов
    int classWorkshop;     // Класс станции 
};

// Функция ввода данных о трубе
void input_pipe(Pipe& pipe) {
    cout << "Pipe name: ";  

    // Ввод названия 
    getline(cin >> ws, pipe.name);
    // Проверка, что название не пустое
    while (pipe.name.empty()) {
        cout << "Error: incorrect data entered: ";  
        getline(cin, pipe.name);
    }

    cout << "Length: ";  
    // Ввод длины с проверкой корректности
    while (!(cin >> pipe.lenght) || (pipe.lenght < 0) || (cin.peek() != '\n')) {
        cout << "Error: please enter correct data: "; 
        cin.clear();    // Очистка флагов ошибок
        cin.ignore(100, '\n');  // Игнорирование оставшихся символов в буфере
    }

    cout << "Diameter: ";  
    // Ввод диаметра с проверкой корректности
    while (!(cin >> pipe.diametr) || (pipe.diametr < 0) || (cin.peek() != '\n')) {
        cout << "Error: incorrect data entered: ";  // "Ошибка: некорректно введены данные: "
        cin.clear();
        cin.ignore(100, '\n');
    }

    cout << "Status (0 - under repair, 1 - working): ";  // "Состояние(0 - в ремонте, 1 - целый): "
    // Ввод состояния трубы
    while (!(cin >> pipe.inRepair) || (cin.peek() != '\n')) {
        cout << "Error: incorrect data entered: ";  // "Ошибка: некорректно введены данные: "
        cin.clear();
        cin.ignore(100, '\n');
    }
}

// Функция редактирования состояния трубы
void edit_Repair_Pipe(Pipe& pipe) {
    cout << "Status (0 - under repair, 1 - working): ";  // "Состояние(0 - в ремонте, 1 - целый): "             
    while (!(cin >> pipe.inRepair)) {
        cout << "Error: incorrect data entered: ";  //"Ошибка: некорректно введены данные: "
        cin.clear();
        cin.ignore();
    }
}

// Функция проверки создания трубы
bool Pipe_created(const Pipe& pipe) {
    return !pipe.name.empty();  // Возвращает true, если название не пустое
}

// Функция сохранения данных о трубе в файл
void save_Pipe(const Pipe& pipe, ofstream& file) {
    if (Pipe_created(pipe)) {
        file << "== Pipe ==\n";  // "== Труба ==\n"
        file << pipe.name << "\n";
        file << pipe.lenght << "\n";
        file << pipe.diametr << "\n";
        file << pipe.inRepair << "\n";
    }
}

// Функция загрузки данных о трубе из файла
void ffile_1(Pipe& pipe, ifstream& file) {
    string line;
    getline(file >> ws, pipe.name);  // Чтение названия с пропуском пробелов
    file >> pipe.lenght;    // Чтение длины
    file >> pipe.diametr;   // Чтение диаметра
    file >> pipe.inRepair;  // Чтение состояния
}

// Функция ввода данных о компрессорной станции
void input_CS(CompressorStation& CS) {
    cout << "CS name: ";  // "Название КС: "
    getline(cin >> ws, CS.name);
    while (CS.name.empty()) {
        cout << "Error: incorrect data entered: ";  // "Ошибка: некорректно введены данные: "
        getline(cin, CS.name);
    }

    cout << "Number of workshops: ";  // "Количество цехов: "
    // Ввод количества цехов с проверкой
    while (!(cin >> CS.numberWorkshop) || (CS.numberWorkshop <= 0) || (cin.peek() != '\n')) {
        cout << "Error: incorrect data entered: ";  // "Ошибка: некорректно введены данные: "
        cin.clear();
        cin.ignore(100, '\n');
    }

    cout << "Number of working workshops: ";  // "Количество работающих цехов: "
    // Ввод работающих цехов с проверкой (не больше общего количества)
    while (!(cin >> CS.workingWorkshop) || (CS.workingWorkshop <= 0) || (CS.workingWorkshop > CS.numberWorkshop) ||  (cin.peek() != '\n')) {
        cout << "Error: incorrect data entered: ";  // "Ошибка: некорректно введены данные: "
        cin.clear();
        cin.ignore(100, '\n');
    }

    cout << "Station class (1 - 5): ";  // "Класс станции (1 - 5): "
    // Ввод класса станции (должен быть от 1 до 5)
    while (!(cin >> CS.classWorkshop) || (CS.classWorkshop <= 1) || (CS.classWorkshop >= 5) || (cin.peek() != '\n')) {
        cout << "Error: incorrect data entered: ";  // Было: "Ошибка: некорректно введены данные: "
        cin.clear();
        cin.ignore(100, '\n');
    }
}

// Функция редактирования компрессорной станции
void edit_CS(CompressorStation& CS) {
    int n;
    cout << "Start workshop 1, stop workshop -1: ";  // "Запуск цеха 1, остановка цеха -1: "
    // Ввод команды (только 1 или -1)
    while (!(cin >> n) || (n != 1 && n != -1)) {
        cout << "Error: incorrect data entered: ";  // "Ошибка: некорректно введены данные: "
        cin.clear();
        cin.ignore(100, '\n');
    }

    CS.workingWorkshop += n;  // Изменение количества работающих цехов
    cout << "Number of workshops: " << CS.workingWorkshop << endl;  // Было: "Количество цехов: "
}

// Функция проверки создания компрессорной станции
bool StationCreated(const CompressorStation& CS) {
    return !CS.name.empty();  // Возвращает true, если название не пустое
}

// Функция просмотра данных о трубе
void view_Object_1(Pipe& pipe) {
    if (Pipe_created(pipe)) {
        cout << "\n === Pipe === \n";  // Было: "=== Труба ==="
        cout << "Name: " << pipe.name << ", Length: " << pipe.lenght << ", Diameter: " << pipe.diametr << ", Under repair?: " << pipe.inRepair << endl; 
    } else cout << "\n Pipe not created \n";  // Было: "Труба не создана"
}

// Функция просмотра данных о компрессорной станции
void view_Object_2(CompressorStation& CS) {
    if (StationCreated(CS)) {
        cout << "\n == Compressor Station == \n";  // Было: "== Компрессорная станция =="
        cout << "Name: " << CS.name << ", Number of workshops: " << CS.numberWorkshop << ", Number of working workshops: " << CS.workingWorkshop << ", Station class: " << CS.classWorkshop << endl;
    } else cout << "\n CS not created \n";  // Было: "КС не создана"
}

// Функция отображения меню
void display_Menu() {
    cout << "\n=== Pipeline Management Menu ===\n";  // Было: "=== Меню управления трубопроводом ==="
    cout << "1. Add pipe\n";           //  "Добавить трубу"
    cout << "2. Add CS\n";             //  "Добавить КС"
    cout << "3. View all objects\n";   //  "Просмотр всех объектов"
    cout << "4. Edit pipe\n";          //  "Редактировать трубу"
    cout << "5. Edit CS\n";            //  "Редактировать КС"
    cout << "6. Save data\n";          //  "Сохранить данные"
    cout << "7. Load data\n";          //  "Загрузить данные"
    cout << "0. Exit\n";               //  "Выход"
    cout << "Select action: ";         //  "Выберите действие: "
}

// Функция сохранения данных о компрессорной станции в файл
void save_CS(const CompressorStation& CS, ofstream& file) {
    if (StationCreated(CS)) {
        file << "== CS ==\n";  // "== КС ==\n"
        file << CS.name << "\n";
        file << CS.numberWorkshop << "\n";
        file << CS.workingWorkshop << "\n";
        file << CS.classWorkshop << "\n";
    }
}

// Функция загрузки данных o компрессорной станции из файла
void ffile_2(CompressorStation& CS, ifstream& file) {
    string line;
    getline(file >> ws, CS.name);  // Чтение названия
    file >> CS.numberWorkshop;     // Чтение количества цехов
    file >> CS.workingWorkshop;    // Чтение работающих цехов
    file >> CS.classWorkshop;      // Чтение класса станции
}

// Главная функция программы
int main() {
    Pipe pipe;                    // Создание объекта трубы
    CompressorStation CS;         // Создание объекта компрессорной станции
    int choice;                   // Переменная для выбора пользователя

    do {   
        display_Menu();           // Отображение меню
        cin >> choice;            // Ввод выбора пользователя

        switch(choice) {
            case(1):
                input_pipe(pipe);
                cout << "Pipe added \n";  //  "Труба добавлена"
                break;
            case(2):
                input_CS(CS);
                cout << "CS added \n";    //  "КС добавлена"
                break;
            case(3):
                view_Object_1(pipe);      // Просмотр трубы
                view_Object_2(CS);        // Просмотр станции
                break;
            case(4):
                if (Pipe_created(pipe)) 
                    edit_Repair_Pipe(pipe);  // Редактирование трубы
                else 
                    cout << "== Create pipe ===\n";  // "== Создайте трубу ==="
                break;
            case(5):
                if (StationCreated(CS)) 
                    edit_CS(CS);          // Редактирование станции
                else 
                    cout << " == Create CS ==\n";  // "== Создайте КС =="
                break;
            case(6): {
                ofstream file("test.txt");  // Открытие файла для записи
                if (file.is_open()) {
                    if (Pipe_created(pipe)) {
                        save_Pipe(pipe, file);  // Сохранение трубы
                    } else {
                        cout << "Pipe was not created \n";  // "Труба не была создана"
                    }

                    if (StationCreated(CS)) {
                        save_CS(CS, file);      // Сохранение станции
                        file.close();
                    } else {
                        cout << "CS was not created \n";  // "КС не была создана"
                        file.close();
                    }
                }  
                cout << "\nData saved to file\n";  // "Данные сохранены в файл"
                break;
            }
            case (7): {
                ifstream file("test.txt");  // Открытие файла для чтения
                if (file.is_open()) {
                    string line;
                    while (getline(file, line)) {  // Чтение файла построчно
                        if (line == "== Pipe ==") {  // "== Труба =="
                            ffile_1(pipe, file);   // Загрузка данных о трубе
                        }
                        else if (line == "== CS ==") {  //  "== КС =="
                            ffile_2(CS, file);     // Загрузка данных о станции
                        }
                    }
                    file.close();
                    cout << "\nData loaded from file\n";  // "Данные загружены из файла"
                } else {
                    cout << "Error: failed to open file for reading\n";  // "Ошибка: не удалось открыть файл для чтения"
                }
                break;
            }
            case(0):
                cout << "Exiting program";  //  "выход из программы"
                break;  
            default:
                cout << "\nInvalid input, enter number from 1 to 7\n";  // "Неверный ввод, введите число от 1 до 7"
            
        }

    } while (choice != 0);  // Цикл продолжается пока не выбран выход

    return 0;  // Завершение программы
}