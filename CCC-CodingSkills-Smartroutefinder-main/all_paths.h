#pragma once
#include "graph.h"
#include <iostream>

void dfsPaths(const Graph& g, int u, int dst, vector<int>& path, vector<bool>& visited, int& count, int maxPaths, int currentDist, int currentTime) {
    if (count >= maxPaths) return;
    
    path.push_back(u);
    visited[u] = true;

    if (u == dst) {
        count++;
        cout << "\n🛣️  Path " << count << ": ";
        for (int i = 0; i < path.size(); i++) {
            cout << g.nodeNames[path[i]];
            if (i != path.size() - 1) cout << " → ";
        }
        cout << "\n📏 Total Distance: " << currentDist << " km";
        cout << "\n⏱ Total Time: " << currentTime << " min\n";
    } else {
        for (const auto& e : g.adj[u]) {
            if (!visited[e.to]) {
                dfsPaths(g, e.to, dst, path, visited, count, maxPaths, currentDist + e.distance, currentTime + e.travelTime);
            }
        }
    }

    path.pop_back();
    visited[u] = false;
}

void showAllPaths(const Graph& g, int src, int dst) {
    vector<int> path;
    vector<bool> visited(g.numNodes, false);
    int count = 0;
    int maxPaths = 10; // Capped to prevent freezing
    cout << "\n=====================================\n";
    cout << "Finding multiple possible paths (capped at 10 to avoid overload)...\n";
    dfsPaths(g, src, dst, path, visited, count, maxPaths, 0, 0);
    if (count == 0) cout << "❌ No paths found!\n";
    cout << "=====================================\n";
}
