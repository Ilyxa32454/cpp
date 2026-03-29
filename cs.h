#ifndef CS_H
#define CS_H

#include <iostream>
#include <fstream>
#include <string>

class CS {
private:
    int id;
    std::string Name;
    int Chex;
    int Workchex;
    int Class;

public:
    CS();

    int getId() const { return id; }
    std::string getName() const { return Name; }
    int getChex() const { return Chex; }
    int getWorkchex() const { return Workchex; }
    int getCSClass() const { return Class; }
    double getInactivePercentage() const;

    void setName(const std::string& name) { Name = name; }
    void setChex(int chex) { Chex = chex; }
    void setWorkchex(int workchex) { 
        if (workchex >= 0 && workchex <= Chex) {
            Workchex = workchex;
        }
    }
    void setCSClass(int csClass) { Class = csClass; }

    void readFromConsole();
    void display() const;
    void startWorkshop();
    void stopWorkshop();
    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);
};

#endif