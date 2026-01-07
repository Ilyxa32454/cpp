#pragma once
#include<unordered_map>
#include<unordered_set>
#include<vector>

#include "Pipe.h"
#include "CompressionStation.h"

class Network {
private:
	std::unordered_map<int, pipe>& pipes;
	std::unordered_map<int, CS>& CSs;
	const std::vector<int> allowed_Diameters = { 500, 700, 1000, 1400 };
	std::unordered_map<int, std::vector<int>> adjacency;
	std::unordered_map<int, std::pair<int, int>> pipes_Connection;
	std::unordered_set<int> used_Pipes;
public:
	Network(std::unordered_map<int, pipe>& pipes,
		std::unordered_map<int, CS>& CSs);

	int find_Free_pipe(int d);
	void delete_Connection(int pID);
	bool path_Exists(int start, int target) const;
	vector<int> topological_Sort() const;
	int pipe_Weight(int from, int to) const;
	int pipe_Capacity(int from, int to) const;
	void dijkstra(int start, int target) const;
	bool max_Capacity_Path(
		int source,
		int sink,
		unordered_map<int, int>& parent,
		unordered_map<int, int>& parentPipe,
		unordered_map<int, int>& dist,
		const unordered_map<int, int>& residual) const;
	void max_Flow(int source, int sink)const;
	vector<int> find_All_pipes(int id);

	void Load(istream& in);
	void Save(ostream& out);
	friend ostream& operator << (ostream& out, const Network& net);
	friend istream& operator >> (istream& in, Network& net);
};
