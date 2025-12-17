#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


class Station {
private:
    int id = 0;
    void recalculate_unused_percentage();
    int workshop_amount;
    string cl4ss;
    int Inwork;

public:
    string name;
    Station(int newId) : id(newId) {}
    Station() : id(0), name(""), workshop_amount(0), cl4ss(""), Inwork(0) {}
    double unused_workshops_percentage;
    void edit_station();
    void save_station(ofstream& outputF);
    void load_station(ifstream& loadF);

    int get_station_id() const { return id; }
    string get_station_name() { return name; }
    int get_station_id() { return id; }
    double get_unused_percentage() { return unused_workshops_percentage; }

    friend std::istream& operator>>(std::istream& in, Station& s);
    friend std::ostream& operator<<(std::ostream& out, const Station& s);

};