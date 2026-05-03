#pragma once
#include "graph.h"
#include <queue>
#include <cmath>

// ─────────────────────────────────────────
// A* Algorithm
// Finds FASTEST PATH considering travel time
// Uses heuristic: straight-line time estimate
// Time Complexity: O(E log V) with good heuristic
// ─────────────────────────────────────────

// Heuristic: estimate remaining time from node u to dst
// We use node index distance as a simple admissible heuristic
int heuristic(int u, int dst) {
    return abs(u - dst) * 2; // lightweight estimate in minutes
}

RouteResult astar(const Graph& g, int src, int dst) {
    vector<int> gScore(g.numNodes, INF); // actual time cost from src
    vector<int> fScore(g.numNodes, INF); // gScore + heuristic
    vector<int> prev(g.numNodes, -1);

    // min-heap: (fScore, node)
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> open;

    gScore[src] = 0;
    fScore[src] = heuristic(src, dst);
    open.push({fScore[src], src});

    while (!open.empty()) {
        auto [f, u] = open.top(); open.pop();
        if (u == dst) break;
        if (f > fScore[u]) continue;

        for (const auto& e : g.adj[u]) {
            int tentative = gScore[u] + e.travelTime;
            if (tentative < gScore[e.to]) {
                gScore[e.to] = tentative;
                fScore[e.to] = tentative + heuristic(e.to, dst);
                prev[e.to] = u;
                open.push({fScore[e.to], e.to});
            }
        }
    }

    RouteResult result;
    result.algorithm = "A* (Fastest Path)";
    result.found = gScore[dst] != INF;
    result.totalTime = gScore[dst] == INF ? 0 : gScore[dst];

    if (result.found) {
        for (int v = dst; v != -1; v = prev[v])
            result.path.push_back(v);
        reverse(result.path.begin(), result.path.end());

        result.totalDistance = 0;
        for (int i = 0; i + 1 < (int)result.path.size(); i++) {
            int u = result.path[i], v = result.path[i+1];
            for (const auto& e : g.adj[u]) {
                if (e.to == v) { result.totalDistance += e.distance; break; }
            }
        }
    }
    return result;
}
