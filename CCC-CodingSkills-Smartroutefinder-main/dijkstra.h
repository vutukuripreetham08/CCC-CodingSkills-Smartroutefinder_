#pragma once
#include "graph.h"
#include <queue>

// ─────────────────────────────────────────
// Dijkstra's Algorithm
// Finds the SHORTEST PATH by distance (km)
// Time Complexity: O((V + E) log V)
// ─────────────────────────────────────────
RouteResult dijkstra(const Graph& g, int src, int dst) {
    vector<int> dist(g.numNodes, INF);
    vector<int> prev(g.numNodes, -1);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        if (u == dst) break;

        for (const auto& e : g.adj[u]) {
            int nd = dist[u] + e.distance;
            if (nd < dist[e.to]) {
                dist[e.to] = nd;
                prev[e.to] = u;
                pq.push({nd, e.to});
            }
        }
    }

    RouteResult result;
    result.algorithm = "Dijkstra (Shortest Path)";
    result.found = dist[dst] != INF;
    result.totalDistance = dist[dst] == INF ? 0 : dist[dst];

    if (result.found) {
        // Reconstruct path
        for (int v = dst; v != -1; v = prev[v])
            result.path.push_back(v);
        reverse(result.path.begin(), result.path.end());

        // Calculate total travel time along chosen path
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
