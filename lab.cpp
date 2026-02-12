#include <iostream>
#include <string>
#include <fstream>
#include <limits>
using namespace std;

struct Pipe{
    string Name;
    double Lenght;
    double Diametr;
    bool Work;
};

struct CS{
    string Name;
    int Chex;
    int Workchex;
    int Class;
};

void AddPipe(Pipe& pipe){
    cout << "Введите название трубы: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, pipe.Name);

    while(true) {
        cout << "Введите длину трубы: ";
        cin >> pipe.Lenght;

        if (pipe.Lenght < 0){
            cout << "Длина трубы не может быть отрицательна! Введите корректные данные: " << endl;
        } else{
            break;
        }
    }

    while(true) {
        cout << "Введите диметр трубы: ";
        cin >> pipe.Diametr;

        if (pipe.Diametr < 0) {
            cout << "Диаметр трубы не может быть отрицательным! Введите коректные данные: " << endl;
        } else{
            break;
        }
    }

    while(true) {
        cout << "Введите статус трубы (1-работает/0-в ремонте): ";
        cin >> pipe.Work;

        if (pipe.Work != 0 && pipe.Work != 1) {
            cout << "Введите коректные данные! ";
        } else {
            break;
        }
    }
}

void printCS(CS& cs){
    cout << "Введите название КС: ";
    cin >> cs.Name;

    while(true){
        cout << "Введите колличество цехов(1-20): ";
        cin >> cs.Chex;

        if (cs.Chex <= 0 || cs.Chex > 20){
            cout << "Некоректные данны, введите заново";
        } else{
            break;
        }
    }

    while(true){
        cout << "Введите колличество рабочих цехов: ";
        cin >> cs.Workchex;

        if (cs.Workchex > cs.Chex){
        cout << "Колличество рабочих цехов не может быть больше всех цехов в целом ";

        } else if (cs.Workchex <= 0 || cs.Workchex > 9){
            cout << "Некоректные данны, введите заново";

        } else {
            break;
        }

    }

    while(true){
        cout << "Введите класс КС (1-9): ";
        cin >> cs.Class;

        if (cs.Class <= 0 || cs.Class > 9){
            cout << "Некоректные данны, введите заново";
        } else{
            break;
        }
    }
}

void ShowObjects(const Pipe& pipe, const CS& cs) {
    bool empty = true;

    cout << "\n===== ОБЪЕКТЫ =====\n";

    if (pipe.Name != "") {
        empty = false;
        cout << "\n--- Труба ---\n";
        cout << "Название: " << pipe.Name << endl;
        cout << "Длина: " << pipe.Lenght << endl;
        cout << "Диаметр: " << pipe.Diametr << endl;
        cout << "Статус: " << (pipe.Work ? "Работает" : "В ремонте") << endl;
    }

    if (cs.Name != "") {
        empty = false;
        cout << "\n--- КС ---\n";
        cout << "Название: " << cs.Name << endl;
        cout << "Цехов всего: " << cs.Chex << endl;
        cout << "Рабочих цехов: " << cs.Workchex << endl;
        cout << "Класс: " << cs.Class << endl;
    }

    if (empty) {
        cout << "Нет добавленных объектов.\n";
    }
}


void EditPipe(Pipe& pipe) {
    if (pipe.Name == "") {
        cout << "Труба не добавлена.\n";
        return;
    }

    int choice;
    cout << "Текущий статус: " << (pipe.Work ? "Работает" : "В ремонте") << endl;

    cout << "Выберите новый статус:\n";
    cout << "1) Работает\n";
    cout << "0) В ремонте\n";

    cin >> choice;

    if (choice == 0 || choice == 1) {
        pipe.Work = choice;
        cout << "Статус трубы обновлён.\n";
    } else {
        cout << "Некорректный ввод.\n";
    }
}

void EditCS(CS& cs) {
    if (cs.Name == "") {
        cout << "КС не добавлена.\n";
        return;
    }

    int cmd;
    cout << "\nТекущие данные КС:\n";
    cout << "Всего цехов: " << cs.Chex << endl;
    cout << "Рабочих цехов: " << cs.Workchex << endl;

    cout << "\n1) Запустить цех\n";
    cout << "2) Остановить цех\n";
    cin >> cmd;

    if (cmd == 1) {
        if (cs.Workchex < cs.Chex) {
            cs.Workchex++;
            cout << "Цех запущен.\n";
        } else {
            cout << "Все цехи уже работают.\n";
        }
    }
    else if (cmd == 2) {
        if (cs.Workchex > 0) {
            cs.Workchex--;
            cout << "Цех остановлен.\n";
        } else {
            cout << "Все цехи уже остановлены.\n";
        }
    }
    else {
        cout << "Некорректная команда.\n";
    }
}

void SaveToFile(const Pipe& pipe, const CS& cs) {
    ofstream file("data.txt");

    if (!file) {
        cout << "Ошибка открытия файла для записи";
        return;
    }

    file << pipe.Name << endl;
    file << pipe.Lenght << endl;
    file << pipe.Diametr << endl;
    file << pipe.Work << endl;

    file << cs.Name << endl;
    file << cs.Chex << endl;
    file << cs.Workchex << endl;
    file << cs.Class << endl;

    file.close();
    cout << "Данные сохранены в файл.";
}

void LoadFromFile(Pipe& pipe, CS& cs) {
    ifstream file("data.txt");

    if (!file) {
        cout << "Файл не найден.";
        return;
    }

    file >> pipe.Name;
    file >> pipe.Lenght;
    file >> pipe.Diametr;
    file >> pipe.Work;

    file >> cs.Name;
    file >> cs.Chex;
    file >> cs.Workchex;
    file >> cs.Class;

    file.close();
    cout << "Данные загружены из файла.";
}

int main() {
    Pipe pipe;
    CS cs;
    int cmd;

    while(true){
        cout << "\n===== МЕНЮ =====\n";
        cout << "1) Добавить трубу\n";
        cout << "2) Добавить КС\n";
        cout << "3) Показать объекты\n";
        cout << "4) Редактировать трубу\n";
        cout << "5) Редактировать КС\n";
        cout << "6) Сохранить в файл\n";
        cout << "7) Загрузить из файла\n";
        cout << "0) Выход\n";
        cout << "Выберите действие: ";
        cin >> cmd;//!!

        switch(cmd) {
            case 1: AddPipe(pipe); break;
            case 2: printCS(cs); break;
            case 3: ShowObjects(pipe, cs); break;
            case 4: EditPipe(pipe); break;
            case 5: EditCS(cs); break;
            case 6: SaveToFile(pipe, cs); break;
            case 7: LoadFromFile(pipe, cs); break;
            case 0: cout << "Выход...\n"; return 0;
            default: cout << "Некорректный пункт меню, попробуйте снова.\n";
        }
    }

    return 0;
}

