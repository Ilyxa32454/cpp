#include "station.h"
#include "utils.h"

void Station::recalculate_unused_percentage() {
    unused_workshops_percentage = (workshop_amount - Inwork) / (double)workshop_amount * 100;
}

void Station::edit_station() {
    if (name.empty()) {
        cout << "Station options are empty" << endl;
        return;
    }
    cout << "Please enter new number of workshops in work from 0 to " << workshop_amount << ": ";
    int value = GetCorrectNumber(0, workshop_amount);
    Inwork = value;
    recalculate_unused_percentage();
    cout << "Successfully updated workshops in work to: " << value << endl;
}

void Station::save_station(ofstream& outputF){
    if (!name.empty()) {
        outputF << "CS" << endl;
        outputF << id << endl << name << endl << workshop_amount << endl
            << Inwork << endl << cl4ss << endl;
    }
}

void Station::load_station(ifstream& loadF) {
    loadF >> id;
    getline(loadF >> ws, name);
    loadF >> workshop_amount;
    loadF >> Inwork;
    getline(loadF >> ws, cl4ss);
    recalculate_unused_percentage();


}
std::istream& operator>>(std::istream& in, Station& s) {
    std::cout << "Type station name: ";
    INPUT_LINE(in, s.name);

    std::cout << "Type amount of workshops: ";
    s.workshop_amount = GetCorrectNumber<int>(1, 100000);

    std::cout << "Type amount of workshops in work: ";
    s.Inwork = GetCorrectNumber<int>(0, s.workshop_amount);

    std::cout << "Type class of station: ";
    INPUT_LINE(in, s.cl4ss);

    s.recalculate_unused_percentage();
    return in;
}
std::ostream& operator<<(std::ostream& out,const Station& s) {
    if (!s.name.empty()) {
        out << "STATION PROPERTIES:" << endl
            << "Name: " << s.name << endl
            << "Workshop amount: " << s.workshop_amount << endl
            << "Workshop inwork: " << s.Inwork << endl
            << "Class: " << s.cl4ss << endl;
    }
    else {
        cout << "CS properties are empty!" << endl;
    }
    return out;
}