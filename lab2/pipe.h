#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;



class Pipe {
private:
    int id = 0;
    double length;
    double diameter;
public:
    Pipe() : id(1), name(""), length(0.0), diameter(0.0), fixing(false) {}
    Pipe(int newId) : id(newId) {}
    string name;
    bool fixing = false;
    void edit_pipe();
    void save_pipe(ofstream& outputF);
    void load_pipe(ifstream& loadF);

    string get_pipe_name() { return name; }
    bool get_is_fixing() { return fixing; }
    int get_pipe_id() { return id; }

    void set_is_fixing(bool new_fixing) { fixing = new_fixing; }

    friend std::istream& operator>>(std::istream& in, Pipe& p);
    friend std::ostream& operator<<(std::ostream& out, const Pipe& p);


};
