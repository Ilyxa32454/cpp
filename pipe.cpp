#include "pipe.h"
#include <limits>

using namespace std;

extern int nextId;

Pipe::Pipe() {
    id = nextId++;
    Lenght = 0;
    Diametr = 0;
    Work = false;
}

void Pipe::readFromConsole() {
    cout << "Введите название трубы: ";
    getline(cin >> ws, Name);

    cout << "Введите длину трубы: ";
    while (!(cin >> Lenght) || Lenght < 0) {
        cout << "Длина трубы не может быть отрицательна! Введите корректные данные: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Введите диаметр трубы: ";
    while (!(cin >> Diametr) || Diametr < 0) {
        cout << "Диаметр трубы не может быть отрицательным! Введите корректные данные: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Введите статус трубы (1-работает/0-в ремонте): ";
    while (!(cin >> Work) || (Work != 0 && Work != 1)) {
        cout << "Введите корректные данные (0 или 1): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void Pipe::display() const {
    cout << "Труба ID:" << id 
         << " | Название: " << Name
         << " | Длина: " << Lenght 
         << " | Диаметр: " << Diametr 
         << " | Статус: " << (Work ? "Работает" : "В ремонте") << endl;
}

void Pipe::toggleWork() {
    Work = !Work;
    cout << "Статус трубы изменен на: " << (Work ? "Работает" : "В ремонте") << endl;
}

void Pipe::saveToFile(ofstream& file) const {
    file << "PIPE" << endl;
    file << id << endl;
    file << Name << endl;
    file << Lenght << endl;
    file << Diametr << endl;
    file << Work << endl;
}

void Pipe::loadFromFile(ifstream& file) {
    file >> id;
    file.ignore();
    getline(file, Name);
    file >> Lenght;
    file >> Diametr;
    file >> Work;
    file.ignore(numeric_limits<streamsize>::max(), '\n');
}