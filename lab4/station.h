#pragma once
#include <string>
#include <iostream>

class Station {
    static int MaxID;
    int id;
    std::string name = "None";
    int workshops = 0, workshops_in_operation = 0;
    char station_class = 'a';

public:
    int getId() const { return id; }
    int getActiveWorkshops() const { return workshops_in_operation; }
    int getWorkshops() const { return workshops; }
    std::string getName() const { return name; }
    char getStationClass() const { return station_class; }

    void setActiveWorkshops(int x) { workshops_in_operation = x; }

    void LoadStation(std::istream& in);
    void SaveStation(std::ostream& out) const;

    static bool check_station_by_name(const Station& station, std::string name);
    static bool check_station_by_workshop_percentage(const Station& station, double percentage);

    friend std::istream& operator>>(std::istream& in, Station& station);
    friend std::ostream& operator<<(std::ostream& out, const Station& station);
};