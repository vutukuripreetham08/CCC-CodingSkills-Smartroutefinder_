#pragma once
#include "graph.h"
#include <unordered_map>
#include <iomanip>

// ─────────────────────────────────────────
// DP Route Cache
// Uses memoization to cache previously computed
// shortest paths — avoids recomputation
// Useful when querying same src/dst repeatedly
// Time Complexity: O(1) on cache hit, O((V+E)logV) on miss
// ─────────────────────────────────────────

struct CacheKey {
    int src, dst, algoId;
    bool operator==(const CacheKey& o) const {
        return src == o.src && dst == o.dst && algoId == o.algoId;
    }
};

struct CacheKeyHash {
    size_t operator()(const CacheKey& k) const {
        return hash<int>()(k.src) ^ (hash<int>()(k.dst) << 8) ^ (hash<int>()(k.algoId) << 16);
    }
};

class DPCache {
    unordered_map<CacheKey, RouteResult, CacheKeyHash> cache;
    int hits = 0, misses = 0;

public:
    bool has(int src, int dst, int algoId) {
        return cache.count({src, dst, algoId});
    }

    RouteResult get(int src, int dst, int algoId) {
        hits++;
        return cache.at({src, dst, algoId});
    }

    void put(int src, int dst, int algoId, const RouteResult& r) {
        misses++;
        cache[{src, dst, algoId}] = r;
    }

    void clear() { cache.clear(); hits = misses = 0; }

    void printStats() const {
        cout << "\n  ┌─ DP Cache Stats ──────────────────────\n";
        cout << "  │  Cached routes : " << cache.size() << "\n";
        cout << "  │  Cache hits    : " << hits << "\n";
        cout << "  │  Cache misses  : " << misses << "\n";
        int total = hits + misses;
        double ratio = total > 0 ? (100.0 * hits / total) : 0.0;
        cout << "  │  Hit ratio     : " << fixed << setprecision(1) << ratio << "%\n";
        cout << "  └──────────────────────────────────────\n";
    }
};

// Global cache instance
DPCache dpCache;
