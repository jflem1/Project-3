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
    // Samuel's CSV File Location C:\Users\smfal\Desktop\graph.csv
    // Joseph's CSV File Loctions: C:/Users/TheWa/VSCode_projects/Project3/graph.csv
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
    return this->adjList[index];
}

int Graph::GetIndex(string name) {
    auto iter = indices.find(name);
    if(iter == indices.end()){
        return -1;
    }
    else{
        return iter->second;
    }
}

string Graph::GetName(int index) {
    return this->names[index];
}

bool Graph::DFS(int first, int second) {
    //will return whether second can be reached from DFS starting at first
    bool connected = false;

    //track visited nodes and nodes to be visited
    set<int> visited;
    stack<int> actors;

    //initialization
    actors.push(first);
    visited.insert(first);

    //dfs
    while (!actors.empty()) {
        //update current actor
        int curr = actors.top();
        actors.pop();

        //if target node is current node
        if (curr == second) {
            connected = true;
            break;
        }
        else {
            //add unvisited neighbors to stack
            for (int i = 0; i < GetNeighbors(curr).size(); i++) {
                //if neighbor unvisited, push it to stack
                if (visited.find(GetNeighbors(curr)[i].first) == visited.end()) {
                    actors.push(GetNeighbors(curr)[i].first);
                    visited.insert(GetNeighbors(curr)[i].first);
                }
            }
        }
        //iterates until curr == second for better readability & future flexibility
    }

    return connected;
}

bool Graph::BFS(int first, int second) {
    //will return whether second can be reached from BFS starting at first
    bool connected = false;

    //track visited nodes and nodes to be visited
    set<int> visited;
    queue<int> actors;

    //initialization
    actors.push(first);
    visited.insert(first);

    //bfs
    while (!actors.empty()) {
        //update current actor
        int curr = actors.front();
        actors.pop();

        //if target node is current node
        if (curr == second) {
            connected = true;
            break;
        }
        else {
            //add unvisited neighbors to queue
            for (int i = 0; i < GetNeighbors(curr).size(); i++) {
                //if neighbor is unvisited, enqueue it
                if (visited.find(GetNeighbors(curr)[i].first) == visited.end()) {
                    actors.push(GetNeighbors(curr)[i].first);
                    visited.insert(GetNeighbors(curr)[i].first);
                }
            }
        }
    }
    return connected;
}

vector<int> Graph::DijkstrasAlgorithm(int source) {
    //priority queue implementation from the Stepik problem
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    //used to calculate the distances from the source vertex to each other vertex
    vector<int> distances(size, INT16_MAX);

    //initialization of the source vertex and pushing it to the queue
    pq.push(make_pair(0, source));
    distances[source] = 0;

    while (!pq.empty()) {
        auto curr = pq.top(); //front queue item
        pq.pop();

        //current vertex
        int u = curr.second;
        //neighboring vertices
        auto tempVect = GetNeighbors(u);

        for (int i = 0; i < tempVect.size(); i++) {
            int v = tempVect[i].first; //neighboring vertex
            int w = tempVect[i].second; //distance
            if (distances[v] > distances[u] + w) {
                distances[v] = distances[u] + w;
                pq.push(make_pair(distances[v], v));
            }
        }
    }
    return distances;
}

vector<int> Graph::bellman_Ford(int src) {
    vector<int> distance(size+1);
    vector<int> previous(size+1);
    unsigned int V = size;
    for(int i = 0; i <= V; i++){
        // In this vector of pairs, the first value represents the distance or weight of the path initialized to INT_MAX
        // and the second represents the previous vertex in the path initialized to -1
        distance[i] = INT16_MAX;
        previous[i] = -1;
    }
    distance[src] = 0;
    previous[src] = 0;
    // set the values for the SRC vertex in the distances vector to 0.
    for(int j = 0; j<=V-1; j++){
        bool updateMade = false;
        for(int k = 0; k <= V-1; k++){
            {
                for(int l = 0; l < adjList[k].size();l++){
                    if (distance[k] == INT16_MAX|| adjList[k].empty()){
                        // if no path has been established for vertex K
                        // or if vertex K has no adjacent vertices then skip to the next K
                        continue;
                    }
                    if(adjList[k][l].second + distance[k] < distance[adjList[k][l].first] ){
                        // check if using the edge from K to L will decrease the weight of the path from SRC to K
                        distance[adjList[k][l].first] = adjList[k][l].second + distance[k];
                        previous[adjList[k][l].first] = k;
                        updateMade = true;
                        // also establishes that there was a change made on this iteration
                    }
                }
            }
        }
        if(!updateMade){
            // if no change was made, then the algorithm has finished finding paths. So end the loop of V-1 iterations.
            break;
        }
    }
    //While this graph has no negative weight weights and therefore no negative cycles, it is a vital part of
    // The Bellman Ford Algorithm to check for negative weight cycles, and therefore is still included in this
    // implementation of bellman ford
    for(int m = 0; m<=V-1; m++){
        for (int l = 0; l < adjList[m].size(); l++) {
            if (adjList[m][l].second + distance[m] < distance[adjList[m][l].first] ) {
                return distance;
            }
        }
    }
    return distance;
}
/*
 * this section was used during debugging and testing to call only the bellman ford algorithm with the ability to test
 * the algorithm for both options
vector<int> Graph::bellman_Ford_Helper(const string& srcName, const string& toName ) {
    int src = GetIndex(srcName);
    if(toName.empty()) {
        vector<int> shortestDistances = bellman_Ford(src);
        cout << "Finished BF Algorithm! " << endl;
        // if only one name was given iterate through the Shortest Distances vector of pairs and output the actors with
        // the strongest connection to the given actor/actress
        cout<< "Table of Shortest Distances For: " << srcName << endl;
        for (int i = 0; i < shortestDistances.size(); i++) {
            if(i == src){
                //skip the output when it reaches the SRC Vertex
                continue;
            }
            int currentDistance = shortestDistances[i];
            string currentName = GetName(i);
            if (currentDistance <= 4) {
                cout << currentName << " : " << currentDistance << endl;
            }
        }
        return shortestDistances;
    }
    else{
        // if a second name is given, after the algorithm has run, search the table for the index of the second name
        // and return the connection factor of the two actors.
        int toIndex = GetIndex(toName);
        if(toIndex == -1){
            cout<< "Name 2 is not Valid! Try again" << endl;
            return vector<int> {-2};
        }
        //START TIME HERE!
        vector<int> shortestDistances = bellman_Ford(src);
        //END TIME HERE!
        cout << "Finished Bellman Ford Algorithm! " << endl;
        int currentDistance = shortestDistances[toIndex];
        cout << srcName << " And " << toName << " Have A Connection Factor Of : " << currentDistance << endl;
        return shortestDistances;
    }
}
*/