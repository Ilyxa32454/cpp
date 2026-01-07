#include "CompressionStation.h"
#include "Utils.h"
#include<iostream>
#include<fstream>
using namespace std;

int CS::current_ID = 0;



int CS::get_ID() const {
	return ID;
}

string CS::get_Name() const{
	return name;
}

int CS::get_Total() const {
	return total;
}

int CS::get_In_work() const{
	return in_work;
}

int CS::get_Class_of_CS() const {
	return class_of_CS;
}

float CS::get_Percentage_of_not_working_CS() const {
	return (static_cast<float>(in_work) / total) * 100.0f;
}

CS::CS() : ID(++current_ID), name(""), total(0), in_work(0), class_of_CS(0) {}

istream& operator >> (istream& in, CS& cs) {
	cout << "Enter CS name: ";
	Input_Line(cin >> ws, cs.name);
	cs.total = Input_Num("Enter the number of compression stations: ", 1, 1000);
	cs.in_work = Input_Num("Enter the number of working compression stations: ", 1, cs.total);
	cs.class_of_CS = Input_Num("Enter a class of compression station(between 1 and 9): ", 1, 9);
	return in;
}

void CS::Load(istream& in) {
	in >> ID;
	in >> ws;
	getline(in, name);
	in >> total;
	in >> in_work;
	in >> class_of_CS;
	if (ID > current_ID) current_ID = ID;
}

void CS::Save(ostream& out) const{
	out << "CS" << endl;
	out <<  ID<<endl;
	out << name << endl;
	out << total << endl;
	out << in_work << endl;
	out << class_of_CS << endl;
}

ostream& operator << (ostream& out, const CS& cs) {
	PRINT_PARAM(cout,"ID", cs.ID);
	PRINT_PARAM(cout,"Name", cs.name);
	PRINT_PARAM(cout, "Total", cs.total);
	PRINT_PARAM(cout,"In work", cs.in_work);
	PRINT_PARAM(cout, "Class of CS", cs.class_of_CS);
	return out;
}

void CS::edit() {
	int var;
	do {
		cout << endl;
		cout << "Number of working workshops currently: " << in_work << endl;
		cout << "1.Add one active workshop\n2.Subtract one active workshop\n3.Exit from editing" << endl;
		var = Input_Num("Select an option: ", 1, 3);
		switch (var) {
		case 1:
			if (in_work < total) in_work++;
			else cout << "Can't add anymore. Maximum number of working workshops" << endl;
			break;
		case 2:
			if (in_work > 0) in_work--;
			else cout << "Can't subtract anymore. Minimum number of working workshops" << endl;
			break;
		}
		cout << endl;
	} while (var != 3);
}

bool CS::Exists() const {
	return !name.empty();
}