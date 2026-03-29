#include "cs.h"
#include <limits>
#include <iomanip>

using namespace std;

extern int nextId;

CS::CS() {
    id = nextId++;
    Chex = 0;
    Workchex = 0;
    Class = 0;
}

void CS::readFromConsole() {
    cout << "Введите название КС: ";
    getline(cin >> ws, Name);

    cout << "Введите количество цехов (1-20): ";
    while (!(cin >> Chex) || Chex <= 0 || Chex > 20) {
        cout << "Некорректные данные, введите заново (1-20): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Введите количество рабочих цехов: ";
    while (!(cin >> Workchex) || Workchex < 0 || Workchex > Chex) {
        cout << "Количество рабочих цехов не может быть больше " << Chex << "! Введите заново: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Введите класс КС (1-9): ";
    while (!(cin >> Class) || Class <= 0 || Class > 9) {
        cout << "Некорректные данные, введите заново (1-9): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void CS::display() const {
    double percent = getInactivePercentage();
    cout << "КС ID:" << id 
         << " | Название: " << Name
         << " | Цехов всего: " << Chex
         << " | Рабочих цехов: " << Workchex
         << " | Незадействовано: " << fixed << setprecision(1) << percent << "%"
         << " | Класс: " << Class << endl;
}

void CS::startWorkshop() {
    if (Workchex < Chex) {
        Workchex++;
        cout << "Цех запущен. Работает цехов: " << Workchex << "/" << Chex << endl;
    } else {
        cout << "Все цехи уже работают!" << endl;
    }
}

void CS::stopWorkshop() {
    if (Workchex > 0) {
        Workchex--;
        cout << "Цех остановлен. Работает цехов: " << Workchex << "/" << Chex << endl;
    } else {
        cout << "Нет работающих цехов!" << endl;
    }
}

void CS::saveToFile(ofstream& file) const {
    file << "STATION" << endl;
    file << id << endl;
    file << Name << endl;
    file << Chex << endl;
    file << Workchex << endl;
    file << Class << endl;
}

void CS::loadFromFile(ifstream& file) {
    file >> id;
    file.ignore();
    getline(file, Name);
    file >> Chex;
    file >> Workchex;
    file >> Class;
    file.ignore(numeric_limits<streamsize>::max(), '\n');
}

double CS::getInactivePercentage() const {
    return (Chex > 0) ? (100.0 * (Chex - Workchex) / Chex) : 0;
}