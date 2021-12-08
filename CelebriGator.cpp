#include "Graph.h"
#include "Graph.cpp"
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std; 

void ConnectionStrength(vector<int> graphDistances, int index) {
	//initializing distance to be calculated as well as a vector to be sorted
	int distance = graphDistances[index];
	vector<int> tempVec = graphDistances; 

	//sorts the vector by distance in ascending order
	sort(tempVec.begin(), tempVec.end()); 

	int veryStrong = 9397; //represents the top 10th percentile of the data
	int strong = 23492; //represents the top 25th percentile 
	int medium = 46984; //represents the top 50th percentile
	int weak = 70476; //represents the top 75th percentile

	cout << "Relationship Strength: ";
	if (distance <= tempVec[veryStrong])
		cout << "Very Strong (Distance of " << distance << ")." << endl;
	else if (distance <= tempVec[strong])
		cout << "Strong (Distance of " << distance << ")." << endl;
	else if (distance <= tempVec[medium])
		cout << "Medium (Distance of " << distance << ")." << endl;
	else if (distance <= tempVec[weak]) 
		cout << "Weak (Distance of " << distance << ")." << endl;
	else
		cout << "Very Weak (Distance of " << distance << ")." << endl; //represents the lower 25th percentile
}

void CelebriGator() {
	cout << "CelebriGator\n" << endl; 
	
	cout << "This tool brings you closer to your favorite celebrities by providing useful" << endl;
	cout << "insights regarding the relationships of over 96,000 actors! CelebriGator utilizes" << endl; 
	cout << "information from the public IMDB database to approximate celebrity friendships " << endl; 
	cout << "based off of overlaps in notable work history. CelebriGator does not only keep you " << endl; 
	cout << "up to date on the celebrity relationships, but also empowers you to explore new " << endl; 
	cout << "titles and genres based on relatedness to actors/actresses you know and love!\n" << endl; 

	Graph graph; 

	cout << "CelebriGator currently supports two features: " << endl; 
	cout << "1. Bestfriend Calculation" << endl; 
	cout << "2. Friendship Strength Measurement\n" << endl; 

	int selection = 0;
	bool stop = false; 

	while (selection != -1) {
		cout << "Please make your selection: ";
		cin >> selection;

		if (selection == 1) { //Bestfriend Calculation
			string name;
			cout << "Please enter actor/actresses name: ";

			//clears the input from selection in order for getline to take input correctly
			cin.ignore();

			getline(cin, name);
			
			//gets the index of the name the user inputted
			int temp = graph.GetIndex(name);
			
			//gets the vector of distances
			vector<int> graphDistances = graph.DijkstrasAlgorithm(temp);

			//stores pairs of indices and distances under 4
			vector<pair<int,int>> directConnections;

			for (int i = 0; i < graphDistances.size(); i++) {
				if (graphDistances[i] <= 4 && i != temp) { //distances of 4 and under represent direct connections between actors and movies
					pair<int, int> tempPair = make_pair(i, graphDistances[i]);
					directConnections.push_back(tempPair);
				}
			}
			//stores the actors with the 5 shortest distances
			vector<pair<int, int>> numActors; 

			for (int i = 1; i < 5; i++) { //starts at 1, as 1 represents 4 movies in common
				for (int j = 0; j < directConnections.size(); j++) {
					if (numActors.size() == 5) //if 5 actors have been identified
						break;
					else { 
						if (directConnections[j].second == i)
							numActors.push_back(directConnections[j]);
					}
				}
			}

			cout << "\nThe following actors have had the largest number of direct collaborations\n";
			cout << "with " << name << " from their most known for appearances:\n" << endl;

			for (int i = 0; i < 5; i++) {
				int appearances = numActors[i].second; //distances are inversely proportional to appearances
				string actorName = graph.GetName(numActors[i].first);
				if (appearances == 1) 
					cout << actorName << ": 4 Collaborations." << endl;
				else if (appearances == 2) 
					cout << actorName << ": 3 Collaborations." << endl;
				else if (appearances == 3) 
					cout << actorName << ": 2 Collaborations." << endl;
				else //if appearances == 4
					cout << actorName << ": 1 Collaboration." << endl;
			}

		}
		else if (selection == 2) { //Friendship Strength Measurement
			string name1, name2; 
			cout << "Please enter name of first actor/actress: ";
			cin.ignore(); //clears the input from selection for getline to work properly
			getline(cin, name1);
			cout << "Please enter name of second actor/actress: ";
			getline(cin, name2);

			//determines index of the first actor/actress
			int index1 = graph.GetIndex(name1);

			chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(); //start time
			vector<int> graphDistances = graph.DijkstrasAlgorithm(index1); //runs Dijkstra's Algorithm with the first actor/actress as the source vertex
			chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); //end time

			//determines runtime for Dijkstra's Algorithm in milliseconds
			int DijkstraTime = chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

			//determines index of the second actor/actress
			int index2 = graph.GetIndex(name2);

			//if the two actors have no connection
			if (graphDistances[index2] == INT_MAX) //INT_MAX represents no connection between actors
				cout << "\n" << name1 << " and " << name2 << " are not connected." << endl;

			//if the two actors are connected
			else {
				cout << "\n" << name1 << " and " << name2 << " are connected." << endl;
				ConnectionStrength(graphDistances, index2);
			}

			begin = std::chrono::steady_clock::now();
			graph.BFS(index1, index2); //runs a breadth-first search
			end = std::chrono::steady_clock::now();
			
			int BFSTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
			cout << "\nTime for BFS: " << BFSTime << " milliseconds." << endl;

			begin = std::chrono::steady_clock::now();
			graph.DFS(index1, index2); //runs a depth-first search
			end = std::chrono::steady_clock::now();
			
			int DFSTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
			cout << "Time for DFS: " << DFSTime << " milliseconds." << endl;

			//outputs Dijkstra's Algorithm from earlier calculation
			cout << "Time for Dijkstra's Algorithm: " << DijkstraTime << " milliseconds." << endl;

			begin = std::chrono::steady_clock::now();
			vector<int> bellmanFordSort = graph.bellman_Ford(index1); //runs the Bellman-Ford Algorithm with the first actor/actress as the source vertex
			end = std::chrono::steady_clock::now();		

			int BellmanFordTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
			cout << "Time for Bellman-Ford Algorithm: " << BellmanFordTime << " milliseconds." << endl;	

		}
		else { //invalid menu input
			cout << "Please enter valid selection (1 or 2)" << endl; 
			continue; 
		}

		cout << "Enter -1 to STOP or 1 to CONTINUE: "; 
		cin >> selection; 
	}
	
	cout << "\nThank you for utilizing CelebriGator! - Aman's A-Listers" << endl; 
}

int main() {
	CelebriGator(); 
    return 0;
}