# 🗺️ Smart Route Finder — India Edition

A C++ pathfinding application that models **120 Indian cities** as a weighted graph and finds optimal routes using six different algorithms. Includes a web-based frontend for interactive exploration.

---

## 📌 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Algorithms](#algorithms)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Tech Stack](#tech-stack)
- [Graph Model](#graph-model)
- [Sample Output](#sample-output)
- [Contributing](#contributing)

---

## Overview

Smart Route Finder simulates road navigation across 120 major Indian cities. Each city is a node in a graph, and roads (edges) carry distance (km) and traffic level attributes. The system lets you find routes optimised for distance, travel time, or hop count — and compare how different algorithms perform on the same journey.

---

## Features

- 🏙️ **120 Indian cities** — from Delhi and Mumbai to Jaisalmer and Alappuzha
- 🔍 **Fuzzy city search** — find cities by partial name
- 🚦 **Traffic simulation** — Low / Moderate / Heavy, randomisable at runtime
- ⚡ **Six pathfinding algorithms** — choose the one that fits your use case
- 📊 **DP Cache** — memoises previously computed routes for instant replay
- 🌐 **Web frontend** — HTML/CSS/JS interface alongside the CLI

---

## Algorithms

| Algorithm | Optimises For | Time Complexity | Notes |
|-----------|--------------|-----------------|-------|
| **Dijkstra** | Shortest distance (km) | O((V + E) log V) | Guaranteed optimal for non-negative weights |
| **A\*** | Fastest travel time | O((V + E) log V) | Heuristic-guided; faster in practice than Dijkstra |
| **Greedy** | Local minimum distance | O(V · E) | Fast but not always globally optimal |
| **BFS** | Fewest hops (stops) | O(V + E) | Unweighted; good for minimising transfers |
| **Bellman-Ford** | Fastest travel time | O(V · E) | Handles negative weights; used here as DP baseline |
| **DP Cached Dijkstra** | Shortest distance (km) | O(1) on cache hit | Memoises results; instant for repeated queries |

---

## Project Structure

```
CCC-CodingSkills-Smartroutefinder/
├── main.cpp              # Entry point — CLI menu and user interaction
├── graph.h               # Graph data structure (nodes, edges, traffic)
├── dijkstra.h            # Dijkstra's algorithm
├── astar.h               # A* algorithm
├── greedy.h              # Greedy best-first search
├── bfs.h                 # Breadth-first search (fewest hops)
├── bellman_ford.h        # Bellman-Ford algorithm
├── dp_cache.h            # Memoisation cache for routes
├── all_paths.h           # Enumerate all paths (capped)
├── Makefile              # Build configuration
├── frontend/             # Web-based UI (HTML / CSS / JS)
│   └── ...
└── README.md
```

---

## Getting Started

### Prerequisites

- A C++17-compatible compiler (`g++` recommended)
- `make` (optional, for Makefile build)

### Build & Run

**Using Make:**
```bash
make
./main
```

**Manual compile:**
```bash
g++ -std=c++17 -O2 -o main main.cpp
./main
```

**Frontend:**
Open `frontend/index.html` directly in any modern browser — no server required.

---

## Usage

After launching, you'll see the main menu:

```
========== SMART ROUTE FINDER (INDIA EDITION) ==========
--- DIRECTORY ---
1. View All Supported Cities
2. Search for a City
--- NAVIGATION ---
3. Find Shortest Path (Distance-based)
4. Find Fastest Path (Time-based)
5. Find Route (Advanced - Choose Algorithm)
6. Show All Possible Paths (Capped)
--- SYSTEM ---
7. Randomize Traffic Conditions
8. View DP Cache Stats
9. Exit
```

**Finding a route (option 3 or 4):**
1. Type a city name or partial name (e.g. `mum` → matches Mumbai)
2. Select from results if multiple matches appear
3. The route, distance, and travel time are displayed

**Comparing algorithms (option 5):**
Run the same source → destination query under any of the six algorithms and observe how results differ.

---

## Tech Stack

| Layer | Technology |
|-------|-----------|
| Core logic | C++17 |
| Data structures | STL (`vector`, `priority_queue`, `unordered_map`) |
| Frontend | HTML5, CSS3, Vanilla JavaScript |
| Build | GNU Make |

---

## Graph Model

The city graph is constructed deterministically using a seeded random number generator (`srand(42)`):

- Each city gets **4–7 connections** to other cities
- Edge **distance**: 100 – 1,600 km (random)
- Edge **traffic**: Low (1×), Moderate (2×), or Heavy (4×) multiplier on travel time
- Edges are **undirected** (travel works both ways)
- Traffic can be re-randomised at runtime via menu option 7

```
travelTime = distance × trafficMultiplier
```

---

## Sample Output

```
=====================================
Algorithm: Dijkstra (Shortest Path)
🚗 Route: Delhi → Jaipur → Jodhpur → Ahmedabad
📏 Distance: 1,204 km
⏱ Time: 1,204 min
=====================================
```

---

## Contributing

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/your-feature`
3. Commit your changes: `git commit -m "Add your feature"`
4. Push and open a Pull Request

Bug reports and suggestions are welcome via [GitHub Issues](https://github.com/Hariharan2006K/CCC-CodingSkills-Smartroutefinder/issues).

---

## Author

**Hariharan K** — [GitHub](https://github.com/Hariharan2006K)

*Built as part of CCC Coding Skills programme.*
