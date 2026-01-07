#include "Network.h"
#include "Utils.h"
#include<iostream>
#include<queue>
#include<fstream>
using namespace std;

Network::Network(unordered_map<int, pipe>& pipes,
	unordered_map<int, CS>& CSs)
	: pipes(pipes), CSs(CSs) {
}

bool diametr_Check(const pipe& p, const int& d) {
	return p.get_Diametr() == d;
}

vector<int> Network::find_All_pipes(int id) {
	vector<int> pipes_deletion;
	for (const auto& [pipeID, ends] : pipes_Connection) {
		if (ends.first == id || ends.second == id) {
			pipes_deletion.push_back(pipeID);
		}
	}
	return pipes_deletion;
}

bool contains_All(const unordered_set<int>& a, const set<int>& b) {
	for (int x : b)
		if (!a.count(x))
			return false;
	return true;
}

vector<int> Network::topological_Sort() const {
	unordered_map<int, int> indegree;

	for (const auto& [csID, _] : CSs)
		indegree[csID] = 0;

	for (const auto& [from, tos] : adjacency) {
		for (int to : tos) {
			indegree[to]++;
		}
	}

	queue<int> q;

	for (const auto& [csID, deg] : indegree) {
		if (deg == 0)
			q.push(csID);
	}

	vector<int> topo;

	while (!q.empty()) {
		int u = q.front();
		q.pop();
		topo.push_back(u);

		auto it = adjacency.find(u);
		if (it != adjacency.end()) {
			for (int v : it->second) {
				if (--indegree[v] == 0)
					q.push(v);
			}
		}
	}
	return topo;
}

int Network::pipe_Weight(int from, int to) const {
	for (const auto& [pID, ends] : pipes_Connection) {
		if (ends.first == from && ends.second == to && !pipes.at(pID).get_Status())
			return pipes.at(pID).get_Length();
	}
	return INT_MAX;
}
int Network::pipe_Capacity(int from, int to) const {
	for (const auto& [pID, ends] : pipes_Connection) {
		if (ends.first == from && ends.second == to && !pipes.at(pID).get_Status()){
			int l = pipes.at(pID).get_Length();
			int d = pipes.at(pID).get_Diametr();
			return d ^ 5 / l;
		}
	}
	return INT_MAX;
}

void Network::dijkstra(int start, int target) const {
	const int INF = numeric_limits<int>::max();

	unordered_map<int, int> dist;
	unordered_map<int, int> parent;

	for (const auto& [csID, _] : CSs)
		dist[csID] = INF;

	dist[start] = 0;

	priority_queue<
		pair<long long, int>,
		vector<pair<long long, int>>,
		greater<>
	> pq;

	pq.push({ 0, start });

	while (!pq.empty()) {
		auto [currentDist, u] = pq.top();
		pq.pop();

		if (currentDist > dist[u])
			continue;

		if (u == target)
			break;

		auto it = adjacency.find(u);
		if (it == adjacency.end())
			continue;

		for (int v : it->second) {
			int w = pipe_Weight(u, v);
			if (dist[u] + w < dist[v]) {
				dist[v] = dist[u] + w;
				parent[v] = u;
				pq.push({ dist[v], v });
			}
		}
	}

	if (dist[target] == INF) {
		cout << "There is no path between these two CSs\n";
		return;
	}

	vector<int> path;
	for (int v = target; v != start; v = parent[v])
		path.push_back(v);
	path.push_back(start);
	reverse(path.begin(), path.end());

	cout << "Shortest path:\n";
	for (size_t i = 0; i < path.size(); ++i) {
		cout << "CS(" << path[i] << ")";
		if (i + 1 < path.size())
			cout << " -> ";
	}
	cout << "\nTotal distance: " << dist[target] << " km\n";
}

bool Network::max_Capacity_Path(
	int source,
	int sink,
	unordered_map<int, int>& parent,
	unordered_map<int, int>& parentPipe,
	unordered_map<int, int>& dist,
	const unordered_map<int, int>& residual
) const {
	dist.clear();
	parent.clear();
	parentPipe.clear();

	for (const auto& [csID, _] : CSs) {
		dist[csID] = 0;
		parent[csID] = -1;
		parentPipe[csID] = -1;
	}

	priority_queue<pair<int, int>> pq;
	dist[source] = INT_MAX;
	pq.push({ INT_MAX, source });

	while (!pq.empty()) {
		auto [cap, u] = pq.top();
		pq.pop();

		if (cap < dist[u])
			continue;

		auto it = adjacency.find(u);
		if (it == adjacency.end())
			continue;

		for (auto v : it->second) {
			int pipeID = -1;
			for (const auto& [pID, ends] : pipes_Connection) {
				if (ends.first == u && ends.second == v && residual.at(pID) > 0) {
					pipeID = pID;
					break;
				}
			}
			if (pipeID == -1) continue;

			int resCap = residual.at(pipeID);
			if (resCap <= 0)
				continue;

			int newCap = min(cap, resCap);

			if (newCap > dist[v]) {
				dist[v] = newCap;
				parent[v] = u;
				for (const auto& [pID, ends] : pipes_Connection) {
					if (ends.first == u && ends.second == v)
						parentPipe[v] = pID;
				}
				pq.push({ newCap, v });
			}
		}
	}

	return dist[sink] > 0;
}

void Network::max_Flow(int source, int sink)const {
	if (!CSs.count(source) || !CSs.count(sink) || source == sink) {
		cout << "Invalid source or sink\n";
		return;
	}

	unordered_map<int, int> residual;
	for (const auto& [pID, pair] : pipes_Connection)
		residual[pID] = pipe_Capacity(pair.first, pair.second);

	unordered_map<int, int> parent;
	unordered_map<int, int> parentPipe;
	unordered_map<int, int> dist;

	int maxFlow = 0;

	while (max_Capacity_Path(source, sink, parent, parentPipe, dist, residual)) {
		int pathFlow = dist[sink];

		for (int v = sink; v != source; v = parent[v]) {
			int pipeID = parentPipe[v];
			residual[pipeID] -= pathFlow;
		}

		maxFlow += pathFlow;
	}


	cout << "\nMaximum flow from " << source
		<< " to " << sink << " is: " << maxFlow
		<< " units\n" << endl;
	return;
}

int Network::find_Free_pipe(int d) {
	auto res_IDs = Find_by_filter<int, pipe, int>(pipes, diametr_Check, d);
	if (res_IDs.empty() || contains_All(used_Pipes, res_IDs)) {
		pipe pi;
		cin >> pi;
		pi.set_Diametr(d);
		pipes.emplace(pi.get_ID(), pi);
		return pi.get_ID();
	}
	else {
		Print_filtered(pipes, res_IDs);
		int pID = Input_Num("Choose a pipe to connect CSs by ID: ", 0, numeric_limits<int>::max());
		while (!res_IDs.count(pID) || used_Pipes.count(pID)) {
			pID = Input_Num("A selected ID is out of list or already used. Try again: ", 0, numeric_limits<int>::max());
		}
		return pID;
	}
}

void Network::delete_Connection(int pID) {
	if (!pipes_Connection.count(pID)) {
		cout << "This pipe not in a network";
	}

	int from = pipes_Connection[pID].first;
	auto& del_vec = adjacency[from];

	used_Pipes.erase(pID);
	pipes_Connection.erase(pID);
	erase(del_vec, pID);
}

bool Network::path_Exists(int start, int target) const
{
	if (start == target)
		return true;
	unordered_set<int> visited;
	vector<int> queue{ start };

	while (!queue.empty()) {
		int u = queue.back();
		queue.pop_back();

		if (u == target)
			return true;

		if (visited.insert(u).second) {
			auto temp = adjacency.find(u);
			if (temp != adjacency.end()) {
				for (int v : temp->second)
					queue.push_back(v);
			}
		}
			
	}
	return false;
}


istream& operator >> (istream& in, Network& net) {
	int	CSfrom = Input_Num("Select what CS you want to add to a network by ID: ", 0, numeric_limits<int>::max());
	int	CSto = Input_Num("What CS you want to connect to previously by ID: ", 0, numeric_limits<int>::max());
	while(!net.CSs.count(CSfrom) || !net.CSs.count(CSto) || net.path_Exists(CSfrom, CSto) || net.path_Exists(CSto, CSfrom)) {
		cout << "One of the CSs doesn't exist or the connection creates cycle.Try again" << endl;
		CSfrom = Input_Num("Select what CS you want to add to a network by ID: ", 0, numeric_limits<int>::max());
		CSto = Input_Num("What CS you want to connect to previously by ID: ", 0, numeric_limits<int>::max());
	} 
	int d = Input_Num("Choose diametr from a list(1 - 500, 2 - 700, 3 - 1000, 4 - 1400): ", 1, 4);
	int pID = net.find_Free_pipe(net.allowed_Diameters[d-1]);

	net.used_Pipes.insert(pID);
	net.pipes_Connection[pID] = { CSfrom, CSto };
	net.adjacency[CSfrom].push_back(CSto);
	return in;
}

ostream& operator << (ostream& out, const Network& net) {
	if (net.pipes_Connection.empty()) {
		cout << "No existing network";
	}
	
	for (auto& kv : net.pipes_Connection) {
		int pID = kv.first;
		int CSfrom = kv.second.first;
		int CSto = kv.second.second;
		int diameter = net.pipes.at(pID).get_Diametr();
		cout << "CS(" << CSfrom << ") --(Pipe ID:" << pID << ")-->CS(" << CSto << ") diametr: " << diameter << endl;
	}
	return out;
}

void Network::Save(ostream& out) {
	for (auto kv : pipes_Connection) {
		int pID = kv.first;
		int CSfrom = kv.second.first;
		int CSto = kv.second.second;
		int d = pipes.at(pID).get_Diametr();
		out << "Connection" << endl
			<< pID << endl
			<< CSfrom << endl
			<< CSto << endl
			<< d << endl ;
	}
}


void Network::Load(istream& in) {
	int p1, from, to, d;
	in >> p1 >> from >> to >> d;
	used_Pipes.insert(p1);
	pipes_Connection[p1] = { from, to };
	adjacency[from].push_back(to);
}