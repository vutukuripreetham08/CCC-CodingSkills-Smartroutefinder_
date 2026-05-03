#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <queue>
#include <cstdlib>
#include <ctime>

using namespace std;

const int INF = numeric_limits<int>::max();

struct Edge {
    int to;
    int distance;
    int traffic;
    int travelTime;

    string trafficLabel() const {
        if (traffic == 0) return "Low";
        if (traffic == 1) return "Moderate";
        return "Heavy";
    }

    int trafficMultiplier() const {
        return (traffic == 0 ? 1 : (traffic == 1 ? 2 : 4));
    }
};

struct RouteResult {
    string algorithm;
    bool found;
    vector<int> path;
    int totalDistance;
    int totalTime;
};

class Graph {
public:
    int numNodes;
    vector<string> nodeNames;
    vector<vector<Edge>> adj;

    Graph(int n, vector<string> names) : numNodes(n), nodeNames(names), adj(n) {}

    void addEdge(int u, int v, int dist, int traffic) {
        int t = dist * (traffic == 0 ? 1 : traffic == 1 ? 2 : 4);
        adj[u].push_back({v, dist, traffic, t});
        adj[v].push_back({u, dist, traffic, t});
    }

    void randomizeTraffic() {
        srand(time(0));
        for (int u = 0; u < numNodes; u++) {
            for (auto &e : adj[u]) {
                e.traffic = rand() % 3;
                e.travelTime = e.distance * e.trafficMultiplier();
            }
        }
    }

    void printGraph() const {
        cout << "\n========== CITY MAP ==========\n";
        for (int i = 0; i < numNodes; i++) {
            cout << nodeNames[i] << ":\n";
            for (auto &e : adj[i]) {
                cout << "  -> " << nodeNames[e.to]
                     << " | " << e.distance << " km"
                     << " | Traffic: " << e.trafficLabel()
                     << " | Time: " << e.travelTime << " min\n";
            }
        }
    }

    int getIndex(string name) {
        for (int i = 0; i < numNodes; i++)
            if (nodeNames[i] == name) return i;
        return -1;
    }

    void shortestPath(int src, int dest) {
        vector<int> dist(numNodes, INF), parent(numNodes, -1);
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;

        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();

            for (auto &e : adj[u]) {
                if (dist[u] + e.travelTime < dist[e.to]) {
                    dist[e.to] = dist[u] + e.travelTime;
                    parent[e.to] = u;
                    pq.push({dist[e.to], e.to});
                }
            }
        }

        if (dist[dest] == INF) {
            cout << "\n❌ No route found!\n";
            return;
        }

        vector<int> path;
        for (int v = dest; v != -1; v = parent[v])
            path.push_back(v);

        reverse(path.begin(), path.end());

        cout << "\n🚗 Best Route:\n   ";
        for (int i = 0; i < path.size(); i++) {
            cout << nodeNames[path[i]];
            if (i != path.size()-1) cout << " → ";
        }

        cout << "\n⏱ Estimated Time: " << dist[dest] << " min\n";
    }
};