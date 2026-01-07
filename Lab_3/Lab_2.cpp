#include<iostream>
#include<fstream>
#include<unordered_map>
#include<set>
#include <format>
#include <chrono>

#include "Pipe.h"
#include "CompressionStation.h"	
#include "Network.h"
#include "Utils.h"
using namespace std;
using namespace chrono;

bool Name_check_pipe(const pipe& pi, const string &name) {
	return pi.get_Name() == name;
}

bool Pipe_status_check(const pipe& pi, const int& under_construcion) {
	return pi.get_Status() == under_construcion;
}

bool Name_check_CS(const CS& cs, const string &name) {
	return cs.get_Name() == name;
}
bool Percent_check_CS(const CS& cs, const float& perc) {
	return cs.get_Percentage_of_not_working_CS() == perc;
}

set<int> Pipe_Filter(const unordered_map<int, pipe> &pipes) {
	int action = Input_Num("1.Filter pipes by name\n2.Filter pipes by status\n0.Exit\nChoose an action:", 0, 2);
	switch (action) {
	case 1:{
		string name;
		cout << "Enter name: ";
		Input_Line(cin >> ws, name);
		return Find_by_filter<int, pipe, string>(pipes, Name_check_pipe, name);
		break;
	}
	case 2: {
		int under_construction = Input_Num("Enter status(0-working, 1-under construction): ", 0, 1);
		return Find_by_filter<int, pipe, int>(pipes, Pipe_status_check, under_construction);
		break;
	}
	case 0:
		return {};
	}
}

set<int> CS_Filter(const unordered_map<int, CS>& CSs) {
	int action = Input_Num("1.Filter CSs by name\n2.Filter CSs by percentage of not working workshops\n0.Exit\nChoose an action:", 0, 2);
	switch (action) {
	case 1: {
		string name;
		cout << "Enter name: ";
		Input_Line(cin >> ws, name);
		return Find_by_filter<int, CS, string>(CSs, Name_check_CS, name);
		break;
	}
	case 2: {
		float percentage = Input_Num("Enter percent of not working workshops: ", 0.f, 100.f);
		return Find_by_filter<int, CS, float>(CSs, Percent_check_CS, percentage);
		break;
	}
	case 0:
		return {};
	}
}

void Batch_edit_pipes(unordered_map<int, pipe>& pipes, Network& net) {
	auto ids = Pipe_Filter(pipes);
	Print_filtered(pipes, ids);

	int action = Input_Num("1.Choose all\n2.Choose manually\n0.Exit\nChoose command: ", 0, 2);
	switch (action) {
	case 1:
	{ break; }
	case 2: {
		set<int> ids_selected;
		cout << "Choose ids(place space between and end with typing 0): ";
		while (true) {
			int id = Input_Num("", 0, numeric_limits<int>::max());
			if (id == 0) break;
			if (ids.contains(id)) ids_selected.insert(id);
			else cout << "No pipe with this id";
		}
		ids = ids_selected;
		break;
	}
	case 3: return;
	}

	if (ids.empty()) { 
		cout << "No pipes selected\n";
		return; }

	int action1 = Input_Num("1.Delete all\n2.Change status\n0.Exit\nChoose command: ",0,2);
	switch (action1) {
	case 1: {
		for (int id : ids) { 
			pipes.erase(id);
			net.delete_Connection(id);

		}
		cout << "All selected pipes deleted.\n";
		break;
	}
	case 2: {
		int n_under_construction = Input_Num("Change tube status(0-working, 1-under construction): ", 0, 1);
		for (int id : ids) {
			pipes.at(id).set_Status(n_under_construction);
		}
		cout << "Changed status of all selected pipes\n";
		break;
	}
	case 0: return;
	}
}

void deleting_CSs(unordered_map<int, CS>& CSs, unordered_map<int, pipe>& pipes, Network& net) {
	auto ids = CS_Filter(CSs);
	Print_filtered(CSs, ids);

	int action = Input_Num("1.Choose all\n2.Choose manually\n0.Exit\nChoose command: ", 0, 2);
	switch (action) {
	case 1:
	{ break; }
	case 2: {
		set<int> ids_selected;
		cout << "Choose ids(place space between and end with typing 0): ";
		while (true) {
			int id = Input_Num("", 0, numeric_limits<int>::max());
			if (id == 0) break;
			if (ids.contains(id)) ids_selected.insert(id);
			else cout << "No CS with this id";
		}
		ids = ids_selected;
		break;
	}
	case 3: return;
	}

	if (ids.empty()) {
		cout << "No CSs selected\n";
		return;
	}

	int action1 = Input_Num("1.Delete all(including adjacent pipes) \n2.Delete CSs and unplug pipes\n0.Exit\nChoose command: ", 0, 2);
	switch (action1) {
	case 1: {
		for (int id : ids) {
			vector<int> pipes_deletion = net.find_All_pipes(id);
			for (int pid : pipes_deletion) {
				net.delete_Connection(pid);
				pipes.erase(pid);
			}
			CSs.erase(id);
		}
		cout << "All selected CSs and their adjacent pipes deleted.\n";
		break;
	}
	case 2: {
		for (int id : ids) {
			vector<int> pipes_deletion = net.find_All_pipes(id);;
			for (int pid : pipes_deletion) {
				net.delete_Connection(pid);
			}
			CSs.erase(id);
		}
		cout << "All selected CSs deleted and their adjacent pipes unplugged.\n";
		break;
	}
	case 0: break;
}
}


bool Save_TF(unordered_map<int, pipe>& pipes, unordered_map<int, CS>& CSs, Network &net) {
	string filename;
	cout << "Enter filename: ";
	Input_Line(cin >> ws, filename);
	ofstream MyFile(filename);
	if (!MyFile.is_open()) {
		cout << "Can't open this file. Please try again\n";
		return false;
	}
		
		
	for (const auto &[_,pi] : pipes)
	{	
		if (pi.Exists()) pi.Save(MyFile);
	}
	for (const auto& [_, cs] : CSs)
	{
		if (cs.Exists()) cs.Save(MyFile) ;
	}
	net.Save(MyFile);
	return true;
}

bool Load_from_file(unordered_map<int, pipe>& pipes, unordered_map<int, CS>& CSs, Network& net) {
	string filename;
	cout << "Enter filename: ";
	Input_Line(cin >> ws, filename);

	ifstream MyFile(filename);
	if (!MyFile.is_open()) {
		cout << "Can't open this file. Please try again";
		return false;
	}

	string line;
	while (getline(MyFile, line)) {
		if (line == "Pipe") {
			pipe pi;
			pi.Load(MyFile);
			if(pi.Exists()) pipes.emplace(pi.get_ID(), pi);
		}
		else if (line == "CS") {
			CS cs;
			cs.Load(MyFile);
			if (cs.Exists()) CSs.emplace(cs.get_ID(), cs);
		}
		else if (line == "Connection") {
			net.Load(MyFile);
		}
	}
	return true;
}


int main() {
	redirect_output_wrapper cerr_out(cerr);
	string time = std::format("{:%d_%m_%Y %H_%M_%OS}", system_clock::now());
	ofstream logfile("log_" + time + ".txt");
	if (logfile)
		cerr_out.redirect(logfile);

	unordered_map<int, pipe> pipes;
	unordered_map<int, CS> CSs;
	Network net(pipes, CSs);
	int command;
	string menu = "\n1.Add a pipe \n2.Add a CS \n3.Objects overview \n4.Edit a pipe \n5.Edit a CS \n6.Find pipes by filter \n7.Find CSs by filter \n8.Batch editing pipes \n9.Save into a file \n10.Download from a file \n11.Network menu \n0.Exit\n";
	string net_menu = "\n1.Add a connection \n2.Delete a connection \n3.Show all connections \n4.Topological sort \n5.Djikstra sort \n6.Max_Thread \n7.Delete a CS \n0.Back\n";
	while (true) {
		cout << menu;
		command = Input_Num("Choose a command ", 0, 11);
		switch (command) {
		case 1: {
			pipe New_pipe;
			cin >> New_pipe;
			pipes.emplace(New_pipe.get_ID(), New_pipe);
			break;
		}
		case 2: {
			CS New_CS;
			cin >> New_CS;
			CSs.emplace(New_CS.get_ID(), New_CS);
			break;
		}
		case 3: {
			if (pipes.empty() && CSs.empty())
			{
				cout << "No objects found. Create something first" << endl;
			}
			else {
				cout << "Pipes:" << endl;
				for (const auto& [_, pi] : pipes) cout << pi;
				cout << "CSs:" << endl;
				for (const auto& [_, cs] : CSs) cout << cs;
			}
			break;
		}
		case 4: {
			auto [status, PE] = Find_in_umap("Choose ID of pipe to edit: ", pipes);
			if (status) PE.set_Status();
			else cout << "Nonexistent pipe chosen. No changes made.";
			break;
		}
		case 5: {
			auto [status, CSE] = Find_in_umap("Choose ID of CS to edit: ", CSs);
			if (status) CSE.edit();
			else cout << "Nonexistent CS chosen. No changes made.";
			break;
		}
		case 6: {
			auto ids = Pipe_Filter(pipes);
			Print_filtered(pipes, ids);
			break;
		}
		case 7: {
			auto ids = CS_Filter(CSs);
			Print_filtered(CSs, ids);
			break;
		}
		case 8: {
			Batch_edit_pipes(pipes, net);
			break;
		}
		case 9: {
			if (Save_TF(pipes, CSs, net))
				cout << "Saved succesfully";
			break;
		}
		case 10: {
			if (Load_from_file(pipes, CSs, net))
				cout << "Loaded succesfully";
			break;
		}
		case 11: {
			cout << net_menu;
			int net_command = Input_Num("Choose a command ", 0, 7);
			switch (net_command) {
			case 1:
				cin >> net;
				break;
			case 2: {
				int del_ID = Input_Num("Choose a pipe ID to delete it from network ", 1, numeric_limits<int>::max());
				net.delete_Connection(del_ID);
				break;
			}
			case 3:
				cout << net;
				break;
			case 4: {
				auto order = net.topological_Sort();
				if (!order.empty()) {
					cout << "Topological order:\n";
					for (int cs : order)
						cout << cs << " ";
					cout << endl;
				}
				break;
			}
			case 5: { 
				int max_size = CSs.size();
				int from = Input_Num("Choose a start CS to find the shortest path: ", 1, max_size);
				int to = Input_Num("Choose a end CS to find the shortest path: ", 1, max_size);
				net.dijkstra(from, to);
				}break;
			case 6: {
				int max_size_1 = CSs.size();
				int source = Input_Num("Choose a source CS to find the max flow: ", 1, max_size_1);
				int sink = Input_Num("Choose a sink CS to find the max flow: ", 1, max_size_1);
				net.max_Flow(source, sink);
			} break;
			case 7:
				deleting_CSs(CSs, pipes, net);
				break;
			case 0:
				break;
			}
			break; 
		}
		case 0:
			return 0;
		}
		}
	}
	


