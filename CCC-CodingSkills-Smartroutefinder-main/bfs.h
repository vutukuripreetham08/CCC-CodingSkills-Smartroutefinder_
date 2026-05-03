#pragma once
#include "graph.h"
#include <queue>

// ─────────────────────────────────────────
// BFS (Breadth-First Search) Algorithm
// Finds the shortest path by fewest hops/cities
// Time Complexity: O(V + E)
// ─────────────────────────────────────────
RouteResult bfsRoute(const Graph& g, int src, int dst) {
    vector<int> prev(g.numNodes, -1);
    vector<bool> visited(g.numNodes, false);
    queue<int> q;

    visited[src] = true;
    q.push(src);

    bool found = false;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (u == dst) {
            found = true;
            break;
        }

        for (const auto& e : g.adj[u]) {
            if (!visited[e.to]) {
                visited[e.to] = true;
                prev[e.to] = u;
                q.push(e.to);
            }
        }
    }

    RouteResult result;
    result.algorithm = "BFS (Fewest Hops)";
    result.found = found;
    result.totalDistance = 0;
    result.totalTime = 0;

    if (found) {
        for (int v = dst; v != -1; v = prev[v])
            result.path.push_back(v);
        reverse(result.path.begin(), result.path.end());

        for (int i = 0; i + 1 < (int)result.path.size(); i++) {
            int u = result.path[i], v = result.path[i+1];
            for (const auto& e : g.adj[u]) {
                if (e.to == v) { 
                    result.totalDistance += e.distance;
                    result.totalTime += e.travelTime; 
                    break; 
                }
            }
        }
    }
    return result;
}
