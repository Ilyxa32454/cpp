#include "pipe.h"
#include "utils.h"

void Pipe::edit_pipe() {
    if (name.empty()) {
        
    }
    else {
        fixing = !fixing;
       
    }
}

void Pipe::save_pipe(ofstream& outputF) {
    if (!name.empty()) {
        outputF << "PIPE" << endl;
        outputF << id << endl << name << endl << length << endl << diameter << endl << fixing << endl;
    }
}

void Pipe::load_pipe(ifstream& loadF) {
    loadF >> id;
    getline(loadF >> ws, name);
    loadF >> length;
    loadF >> diameter;
    loadF >> fixing;
}
std::istream& operator>>(std::istream& in, Pipe& p) {
    std::cout << "Type pipe name: ";
    INPUT_LINE(in, p.name);

    std::cout << "Type pipe length: ";
    p.length = GetCorrectNumber<double>(0, 1500);

    std::cout << "Type pipe diameter: ";
    p.diameter = GetCorrectNumber<double>(0, 1200);

    return in;
}
std::ostream& operator<<(std::ostream& out, const Pipe& p) {
    if (!p.name.empty()) {
        out << endl << "PIPE PROPERTIES:" << endl
            << "Name: " << p.name << endl
            << "Length: " << p.length << endl
            << "Diameter: " << p.diameter << endl
            << "Fixing: " << p.fixing << endl;
    }
    else {
        cout << "Pipe properties are empty!" << endl;
    }
    return out;
}