#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <algorithm>
#include "SkipList.h"
#include "SkipList.cpp"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::queue;
using std::pair;
using std::ifstream;
using std::make_pair;

vector<string> BFS(const SkipList<string> &list, const string from, const string to) {
    queue<Node<string> *> myQueue;
    Node<string> *start = list.find(from);
    Node<string> *end = list.find(to);
    myQueue.push(start);
    vector<string> path;
    while (!myQueue.empty()) {
        Node<string> *current = myQueue.front();
        myQueue.pop();
        if (current->data == end->data) {
            break;
        }
        if (current->skip && list.check(current->skip, end)) {
            path.push_back(current->data);
            myQueue.push(current->skip);
            continue;
        }
        if (current->next) {
            path.push_back(current->data);
            myQueue.push(current->next);
        }
    }
    return path;
}


struct city {
    int u;
    int v;
    int time;
    city() {u = 0; v = 0; time = -1;};
    city(int u, int v, int time) : u(u), v(v), time(time) {}
};
bool operator<(city a, city b) {
    return a.time > b.time;
}
#define STARTING_NODE "Railstation"

int getCityNumber(const vector<string>& cities, const string& name) {
    for(int i = 0; i < cities.size(); i++) {
        if(cities[i] == name) return i;
    }
    return -1;
}
void dijkstra(int current, const int& n, const vector<vector<city>>& graph, vector<int>& distance) {
    std::priority_queue<city> q;
    for (city curr: graph[current]) q.push(curr);
    for (int i = 0; i <= n; i++) distance[i] = -1;
    distance[current] = 0;
    while (!q.empty()) {
        city next = q.top();
        q.pop();
        if (distance[next.v] == -1 || distance[next.v] > distance[next.u] + next.time) {
            distance[next.v] = distance[next.u] + next.time;
            for (city v: graph[next.v]) {
                q.push(v);
            }
        }
    }

}

void furthestCity(int current, vector<int> path, int currentTime, const int& allowedTime, vector<bool> visited, const vector<int>& distance,
                  int& maxCities, int &distanceTravelled,
                  vector<int>& bestGo, const vector<vector<city>>& graph, int lastEdge) {

    visited[current] = true;
    if (currentTime + distance[current] > allowedTime) {
        if (path.size() - 1 > maxCities) {
            maxCities = path.size() - 1;
            distanceTravelled = currentTime - lastEdge;
            bestGo.clear();
            for (int index = 0; index < path.size() - 1; index++) {
                bestGo.push_back(path[index]);
            }
        }
        return;
    }
    for (city next: graph[current]) {
        if (visited[next.v] == 0) {
            path.push_back(next.v);
            furthestCity(next.v, path, currentTime + next.time,
                         allowedTime,visited,distance,maxCities,distanceTravelled,bestGo,graph,
                         next.time);
            path.pop_back();
        }
    }
    visited[current] = false;
}

void getBack(const int& reach, int current, vector<int> path, int currentTime, const int& allowedTime,
             int& minPath, vector<int>& bestBack, vector<bool> visited,
             const vector<vector<city>>& graph) {
    if (current == reach && currentTime <= allowedTime) {
        if (minPath == -1 || minPath > path.size()) {
            minPath = path.size();
            bestBack.clear();
            for (int index: path) bestBack.push_back(index);
        }
    }
    visited[current] = true;
    for (city next: graph[current]) {
        if (!visited[next.v]) {
            path.push_back(next.v);
            getBack(reach, next.v, path, currentTime + next.time,
                    allowedTime,minPath, bestBack, visited, graph);
            path.pop_back();
        }
    }
    visited[current] = false;
}

void print(const vector<string>& path) {
    size_t size = path.size();
    for (int i = 0; i < size; ++i) {
        cout<< path[i] << " ";
    }
}

void printCities(vector<string> cities, const vector<int>& path) {
    size_t size = path.size();
    for (int i = 0; i < size; ++i) {
        cout << cities[path[i]] << " ";
    }
}
void calculateStops(const string& fileName)
{
// 1. Read the file
    vector<string> cities;
    vector<vector<city> > graph;
    vector<int> distance;
    vector<int> bestBack;
    vector<int> bestGo;
    vector<bool> visited;
    vector<int> path;

    int timeAllowed;
    int startingNode;
    int maxCities = 0;
    int distanceTravelled = 0;
    int minPath = -1;

    ifstream file(fileName + ".txt");
    if(!file.is_open())
    {
        return;
    }
    // first line in file is two numbers numberOfNodes and numberOfEdges
    int numberOfNodes, numberOfEdges;

    file >> numberOfNodes >> numberOfEdges;
    for(int i = 0; i < numberOfNodes; i ++) {
        graph.emplace_back();
        distance.emplace_back();
        visited.emplace_back();
    }

    string from, to;
    unsigned minutes;
    int x,y;
    for (int i = 0; i < numberOfEdges; ++i) {
        // each line contains two strings followed by an integer
        file >> from >> to >> minutes;
        if(getCityNumber(cities,to) == -1) cities.push_back(to);
        if(getCityNumber(cities,from) == -1) cities.push_back(from);
        x = getCityNumber(cities, from);
        y = getCityNumber(cities, to);
        graph[x].emplace_back(x,y,minutes);
        graph[y].emplace_back(y,x,minutes);
    }
    // last line contains total number of minutes available
    file >> timeAllowed;

    startingNode = getCityNumber(cities, STARTING_NODE);

    dijkstra(startingNode, numberOfNodes,graph,distance);

    path.push_back(startingNode);
    furthestCity(startingNode, path, 0, timeAllowed,visited,distance,maxCities,distanceTravelled,
                 bestGo,graph,0);

    for (int i = 0; i <= numberOfNodes; i++) visited[i] = false;
    path.clear();
    getBack(startingNode, bestGo.back(), path, distanceTravelled, timeAllowed,minPath,
            bestBack, visited, graph);

    printCities(cities, bestGo);
    printCities(cities, bestBack);
    cout << endl;

}

void findRailStations() {
    unsigned n;
    cin >> n;
    string input;
    SkipList<string> list;
    string begin, end;
    for (int i = 0; i < n; ++i) {
        cin >> input;
        if(i == 0)
        {
            begin = input;
        }
        if(i == n-1)
        {
            end = input;
        }
        list.insert(input);
    }
    unsigned connections;
    cin >> connections;
    string from, to;
    for (int i = 0; i < connections; ++i) {
        cin >> from >> to;
        list.forward(from,to);
    }
    unsigned numberOfCities;
    cin >> numberOfCities;
    vector<string> citiesToVisit;
    for (int i = 0; i < numberOfCities; ++i) {
        cin >> input;
        citiesToVisit.push_back(input);
    }
    if(citiesToVisit[0] != begin)
    {
        citiesToVisit.insert(citiesToVisit.begin(),begin);
    }
    if(citiesToVisit[citiesToVisit.size()-1] != end)
    {
        citiesToVisit.push_back(end);
    }
    vector<string> printPath;
    for (int i = 0; i < numberOfCities + 1; ++i) {
        printPath = BFS(list,citiesToVisit[i], citiesToVisit[i+1]);
        print(printPath);
    }
    cout << endl;
}

int main() {
    //Task A
    findRailStations();
    //Task B
    cout<<"Best railstation stops:\n";
    calculateStops("Railstation");
    return 0;
}

