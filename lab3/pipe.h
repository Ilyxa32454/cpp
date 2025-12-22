#pragma once
#include <string>
#include <iostream>

class Pipe {
    static int MaxID;
    int id;
    std::string name = "None";
    double length = 0.0;
    int diameter = 0;
    bool in_repair = false;
    int start = 0, end = 0;

public:
    int getId() const { return id; }
    int getStart() const { return start; }
    int getEnd() const { return end; }
    double getLength() const { return length; }
    int getDiameter() const { return diameter; }
    bool getInRepair() const { return in_repair; }
    std::string getName() const { return name; }

    void SetStartEnd(int st, int en);
    void setInRepair(bool r) { in_repair = r; }
    void setStart(int s) { start = s; }
    void setEnd(int e) { end = e; }

    void LoadPipe(std::istream& in);
    void SavePipe(std::ostream& out) const;

    bool check_pipe_by_diameter(int d) const;

    static bool check_pipe_by_name(const Pipe& pipe, std::string name);
    static bool check_pipe_by_in_repair(const Pipe& pipe, bool in_rep);

    friend std::istream& operator>>(std::istream& in, Pipe& pipe);
    friend std::ostream& operator<<(std::ostream& out, const Pipe& pipe);
};