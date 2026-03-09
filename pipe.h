#ifndef PIPE_H
#define PIPE_H

#include <iostream>
#include <fstream>
#include <string>

class Pipe {
private:
    int id;
    std::string Name;
    double Lenght;
    double Diametr;
    bool Work;

public:
    Pipe();

    int getId() const { return id; }
    std::string getName() const { return Name; }
    double getLenght() const { return Lenght; }
    double getDiametr() const { return Diametr; }
    bool isWorking() const { return Work; }

    void setName(const std::string& name) { Name = name; }
    void setLenght(double len) { Lenght = len; }
    void setDiametr(double diam) { Diametr = diam; }
    void setWork(bool work) { Work = work; }

    void readFromConsole();
    void display() const;
    void toggleWork();
    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);
};

#endif