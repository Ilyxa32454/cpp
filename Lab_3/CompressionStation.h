#pragma once
using namespace std;
#include<iostream>

class CS {
	static int current_ID;
	int ID;
	string name;
	int total;
	int in_work;
	int class_of_CS;
public:
	int get_ID() const;
	string get_Name() const;
	int get_In_work() const;
	int get_Total() const;
	int get_Class_of_CS() const;
	float get_Percentage_of_not_working_CS() const;
	CS();
	bool Exists() const;
	void edit();

	void Load(istream& in);
	void Save(ostream& out) const;
	friend ostream& operator << (ostream& out, const CS& cs);
	friend istream& operator >> (istream& in, CS& cs);
};