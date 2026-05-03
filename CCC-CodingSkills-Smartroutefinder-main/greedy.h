#pragma once
#include "graph.h"
#include <unordered_set>

// ─────────────────────────────────────────
// Greedy Algorithm
// At each step: pick neighbor with LOWEST
// travel time edge (locally optimal choice)
// Time Complexity: O(V * E)
// Note: Not guaranteed optimal, but very fast
// ─────────────────────────────────────────
RouteResult greedy(const Graph& g, int src, int dst) {
    vector<int> path;
    unordered_set<int> visited;

    int current = src;
    path.push_back(current);
    visited.insert(current);

    int totalDist = 0, totalTime = 0;

    while (current != dst) {
        int bestNext = -1;
        int bestTime = INF;

        for (const auto& e : g.adj[current]) {
            if (!visited.count(e.to) && e.travelTime < bestTime) {
                bestTime = e.travelTime;
                bestNext = e.to;
            }
        }

        if (bestNext == -1) {
            // Dead end — greedy failed
            RouteResult fail;
            fail.algorithm = "Greedy (Quick Decision)";
            fail.found = false;
            fail.totalDistance = 0;
            fail.totalTime = 0;
            return fail;
        }

        // Find distance to bestNext
        for (const auto& e : g.adj[current]) {
            if (e.to == bestNext) {
                totalDist += e.distance;
                totalTime += e.travelTime;
                break;
            }
        }

        visited.insert(bestNext);
        path.push_back(bestNext);
        current = bestNext;
    }

    RouteResult result;
    result.algorithm = "Greedy (Quick Decision)";
    result.found = true;
    result.path = path;
    result.totalDistance = totalDist;
    result.totalTime = totalTime;
    return result;
}
