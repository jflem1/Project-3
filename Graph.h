#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <utility>
#include <iostream>
#include <chrono>
#include <set>
#include <queue>
#include <stack>

#pragma once
using namespace std;

class Graph
{
    //mutable
    int size = 93968;

    unordered_map<int, vector<pair<int,int>>> adjList;
    unordered_map<int, string> names;
    unordered_map<string, int> indices;

public:
    //data member initialization
    Graph();

    //general API functionality
    const vector<pair<int, int>>& GetNeighbors(int index);
    int GetIndex(string name);
    string GetName(int index);

    //graph algorithms
    bool DFS(int first, int second);
    bool BFS(int first, int second);
    vector<int> DijkstrasAlgorithm(int source);
    vector<int> bellman_Ford(int source);
    //vector<int> bellman_Ford_Helper(const string& srcName, const string& toName); // Calls Bellman Ford Algorithm and Outputs
};