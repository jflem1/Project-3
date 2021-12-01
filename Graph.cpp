#include "Graph.h"

/* === BEGIN HELPERS === */

int GetRelationship(unordered_map<int, vector<pair<int, int>>>& adjList, int from, int to) {
	int exists = -1;

	for (int i = 0; i < adjList[from].size(); i++) {
		if (adjList[from][i].first == to) {
			exists = i;
		}
	}
	
	return exists;
}

/* === END HELPERS === */

Graph::Graph() {
	//read graph.csv, calculate name/id maps, calculate groupings, calculate adjList 
	ifstream inFile("data/graph.csv");

	if (inFile.is_open()) {
		//graph.csv labels (database attributes)
		string attributes;
		getline(inFile, attributes);

		//will store movie ID's and actor indices
		unordered_map<string, vector<int>> groups;

		//iterate through each actor, update group membership in hashtable accordingly
		for (int i = 0; i < size; i++) {
			string name, m1, m2, m3, m4;

			//read in actor's name
			getline(inFile, name, ',');
			names[i] = name;
			indices[name] = i;

			//read in the 4 movies which the actor is known for
			getline(inFile, m1, ',');
			m1.erase(0, 1);
			groups[m1].push_back(i);
			getline(inFile, m2, ',');
			groups[m2].push_back(i);
			getline(inFile, m3, ',');
			groups[m3].push_back(i);
			getline(inFile, m4, '\n');
			m4.pop_back();
			groups[m4].push_back(i);
		}

		//remove excessively small and large groups
		auto iter = groups.begin();
		while (iter != groups.end()) {
			if (iter->second.size() == 1 || iter->second.size() > 20) {
				iter = groups.erase(iter);
				continue;
			}
			iter++;
		}

		//iterate over each movie
		auto iterator = groups.begin();
		while (iterator != groups.end()) {
			//iterate through actors in those movies
			for (int x = 0; x < (iterator->second).size(); x++) {
				for (int y = x + 1; y < (iterator->second).size(); y++) {
					//returns -1 if no edge, else index of existing edge
					int idx = GetRelationship(adjList, iterator->second[x], iterator->second[y]);
					int idx2 = GetRelationship(adjList, iterator->second[y], iterator->second[x]);

					if (idx != -1) {
						adjList[iterator->second[x]][idx].second--;
						adjList[iterator->second[y]][idx2].second--;
					}
					else {
						pair<int, int> curr(iterator->second[y], 4);
						adjList[iterator->second[x]].push_back(curr);
						pair<int, int> curr2(iterator->second[x], 4);
						adjList[iterator->second[y]].push_back(curr2);
					}
				}
			}
			iterator++;
		}
	}

	inFile.close();
}

const vector<pair<int, int>>& Graph::GetNeighbors(int index) {
	return adjList[index]; 
}

int Graph::GetIndex(string name) {
	return indices[name];
}

string Graph::GetName(int index) {
	return names[index]; 
}
