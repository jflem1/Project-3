#include "Graph.h"

using namespace std; 

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
	
		if (selection == 1) {
			string name; 
			cout << "Please enter actor/actresses name: "; 
			cin >> name; 
		}
		else if (selection == 2) {
			string name1, name2; 
			cout << "Please enter name of first actor/actress: ";
			cin >> name1;
			cout << "Please enter name of second actor/actress: ";
			cin >> name2;
		}
		else {
			cout << "Please enter valid selection (1 or 2)" << endl; 
			continue; 
		}

		cout << "Enter -1 to STOP or 1 to CONTINUE: "; 
		cin >> selection; 
	}
	
	cout << "\nThank you for utilizing CelebriGator! - Aman's A-Listers" << endl; 
}

int main()
{
	cout << "Initializing Graph!" << endl; 
	Graph graph; 
	cout << "Graph Complete!\n" << endl; 

	vector<int> dij = graph.DijkstrasAlgorithm(10);
	cout << "Dijkstra's Complete!" << endl; 

	auto start = chrono::steady_clock::now(); 
	vector<int> bell = graph.BellmanFord(10);
	auto end = chrono::steady_clock::now(); 
	cout << "Bellman Ford Elapsed Time: " << chrono::duration_cast<chrono::seconds>(end - start).count() << "s\n" << endl; 

	cout << "Distances From Node 10:" << endl; 
	for (int i = 0; i < 1000; i++) {
		cout << i << ": " << bell[i] << " && " << dij[i] << endl;
	}
}