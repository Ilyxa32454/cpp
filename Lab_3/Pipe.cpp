#include "Pipe.h"
#include "Utils.h"
#include<iostream>
#include<fstream>
using namespace std;

int pipe::current_ID = 0;



int pipe::get_ID() const{
	return ID;
}

string pipe::get_Name() const {
	return name;
}

int pipe::get_Length() const {
	return length;
}

int pipe::get_Diametr() const {
	return diametr;
}

int pipe::get_Status() const {
	return under_construction;
}

void pipe::set_Status() {
	this->under_construction = Input_Num("Change tube status(0-working, 1-under construction): ",0,1);
}

void pipe::set_Status(const int status) {
	this->under_construction = status;
}

pipe::pipe() : ID(++current_ID), name(""), length (0), diametr(0), under_construction(0) {}

ostream& operator << (ostream& out, const pipe& p) {
	PRINT_PARAM(cout,"ID", p.ID);
	PRINT_PARAM(cout,"Name", p.name);
	PRINT_PARAM(cout,"Length", p.length);
	PRINT_PARAM(cout, "Diametr",p.diametr);
	PRINT_PARAM(cout, "Status(0-working, 1-under construction)",p.under_construction);
	return out;	
}

void pipe::Load(istream& in) {
	in >> ID;
	in >> ws;
	getline(in, name);
	in >> length;
	in >> diametr;
	in >> under_construction;
	if (ID > current_ID) current_ID = ID;
}

void pipe::Save(ostream& out) const{
	out << "Pipe" << endl;
	out << ID << endl;
	out << name << endl;
	out << length << endl;
	out << diametr << endl;
	out << under_construction << endl;
}

istream& operator >> (istream& in, pipe& p) {
	cout << "Enter name: ";
	Input_Line(cin >> ws, p.name);
	p.length = Input_Num("Enter pipe length: ", 1, 10000);
	p.diametr = Input_Num("Enter pipe diametr: ", 1, 10000);
	p.under_construction = 0;
	return in;
}

bool pipe::Exists() const {
	return !name.empty();
}