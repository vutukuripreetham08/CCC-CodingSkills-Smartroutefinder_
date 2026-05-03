#pragma once
#include "graph.h"

// ─────────────────────────────────────────
// Bellman-Ford Algorithm (Dynamic Programming)
// Finds the shortest path by distance.
// Known for handling negative weights (though not used here).
// Uses dynamic programming to relax all edges V-1 times.
// Time Complexity: O(V * E)
// ─────────────────────────────────────────
RouteResult bellmanFord(const Graph& g, int src, int dst) {
    vector<int> dist(g.numNodes, INF);
    vector<int> prev(g.numNodes, -1);
    dist[src] = 0;

    for (int i = 0; i < g.numNodes - 1; ++i) {
        bool updated = false;
        for (int u = 0; u < g.numNodes; ++u) {
            if (dist[u] == INF) continue;
            for (const auto& e : g.adj[u]) {
                if (dist[u] + e.distance < dist[e.to]) {
                    dist[e.to] = dist[u] + e.distance;
                    prev[e.to] = u;
                    updated = true;
                }
            }
        }
        if (!updated) break; // Optimization: stop early if no updates
    }

    RouteResult result;
    result.algorithm = "Bellman-Ford (Dynamic Programming)";
    result.found = dist[dst] != INF;
    result.totalDistance = dist[dst] == INF ? 0 : dist[dst];

    if (result.found) {
        for (int v = dst; v != -1; v = prev[v])
            result.path.push_back(v);
        reverse(result.path.begin(), result.path.end());

        result.totalTime = 0;
        for (int i = 0; i + 1 < (int)result.path.size(); i++) {
            int u = result.path[i], v = result.path[i+1];
            for (const auto& e : g.adj[u]) {
                if (e.to == v) { result.totalTime += e.travelTime; break; }
            }
        }
    }
    return result;
}
