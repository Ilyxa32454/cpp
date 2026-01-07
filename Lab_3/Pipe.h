#pragma once
using namespace std;
#include<iostream>

class pipe
{
	static int current_ID;
	int ID;
	string name;
	int length;
	int diametr;
	int under_construction;
public:
	int get_ID() const;
	string get_Name() const;
	int get_Length() const;
	int get_Diametr() const;
	int get_Status() const;
	void set_Diametr(int d) {diametr = d;}
	pipe();

	bool Exists() const;
	void set_Status();
	void set_Status(const int status);

	void Load(istream& in);
	void Save(ostream& out) const;
	friend ostream& operator << (ostream& out, const pipe& p);
	friend istream& operator >> (istream& in, pipe& p);
};