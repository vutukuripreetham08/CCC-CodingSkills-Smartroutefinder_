# 🚦 Smart Traffic Route Optimizer

A C++ console application that finds optimal routes through a city graph using **Dijkstra**, **A\***, **Greedy**, and **Dynamic Programming (memoization cache)**.

---

## 📁 Project Structure

```
smart-traffic-optimizer/
├── main.cpp            ← Entry point + city map + menu
├── Makefile            ← Build system
└── src/
    ├── graph.h         ← Graph, Edge, RouteResult structs
    ├── dijkstra.h      ← Dijkstra algorithm (shortest path)
    ├── astar.h         ← A* algorithm (fastest path)
    ├── greedy.h        ← Greedy algorithm (quick decision)
    └── dp_cache.h      ← DP memoization cache
```

---

## ⚙️ Algorithms Used

| Algorithm  | Optimizes       | Complexity       | Guaranteed Optimal? |
|------------|-----------------|------------------|---------------------|
| Dijkstra   | Distance (km)   | O((V+E) log V)   | ✅ Yes               |
| A*         | Travel Time     | O(E log V)       | ✅ Yes (admissible h)|
| Greedy     | Local best edge | O(V × E)         | ❌ No                |
| DP Cache   | Repeat queries  | O(1) on hit      | ✅ (reuses result)   |

---

## 🏙️ City Map (10 Nodes, 18 Roads)

```
Nodes: Airport, Downtown, University, Hospital, Market,
       Stadium, Park, Mall, Harbor, Suburb

Traffic Levels:
  0 = Low       (×1 multiplier)
  1 = Moderate  (×2 multiplier)
  2 = Heavy     (×4 multiplier)

Travel Time = Distance × Traffic Multiplier
```

---

## 🚀 How to Run

### Prerequisites
- g++ with C++17 support

### Compile & Run
```bash
git clone https://github.com/YOUR_USERNAME/smart-traffic-optimizer.git
cd smart-traffic-optimizer

# Using Makefile
make run

# Or manually
g++ -std=c++17 -O2 -o traffic_optimizer main.cpp
./traffic_optimizer
```

### Windows (MinGW)
```bash
g++ -std=c++17 -O2 -o traffic_optimizer.exe main.cpp
traffic_optimizer.exe
```

---

## 🎮 Features

- **Find Route** — pick source, destination, and algorithm
- **Compare All** — runs all 3 algorithms and shows a summary table with winner
- **Show City Map** — prints the full graph with edges, distances, and traffic
- **Randomize Traffic** — simulates real-time traffic changes
- **DP Cache Stats** — shows hit/miss ratio for memoized routes
- **Clear Cache** — resets DP cache

---

## 📸 Sample Output

```
  [Dijkstra (Shortest Path)]
  │  Path   : Airport → Hospital → University → Park → Stadium → Downtown
  │  Distance: 23 km
  │  Est. Time: 38 min

  [A* (Fastest Path)]
  │  Path   : Airport → Hospital → Market → Downtown
  │  Distance: 16 km
  │  Est. Time: 28 min

  🏆 Shortest Distance : Dijkstra (16 km)
  ⚡ Fastest Time     : A*       (28 min)
```

---

## 🎓 Academic Concepts Demonstrated

- Graph representation (Adjacency List)
- Priority Queue / Min-Heap
- Heuristic search (A*)
- Greedy local optimization
- Memoization / Dynamic Programming
- Time complexity analysis

---

## 📄 License
MIT License
