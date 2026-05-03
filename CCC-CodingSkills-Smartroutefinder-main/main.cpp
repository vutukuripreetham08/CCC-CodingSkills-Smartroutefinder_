#include "graph.h"
#include "dijkstra.h"
#include "astar.h"
#include "greedy.h"
#include "dp_cache.h"
#include "bellman_ford.h"
#include "bfs.h"
#include "all_paths.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

void printResult(const RouteResult& r, const Graph& g) {
    cout << "\n=====================================\n";
    cout << "Algorithm: " << r.algorithm << "\n";
    if (!r.found) {
        cout << "❌ No route found!\n";
    } else {
        cout << "🚗 Route: ";
        for (int i = 0; i < r.path.size(); i++) {
            cout << g.nodeNames[r.path[i]];
            if (i != r.path.size() - 1) cout << " → ";
        }
        cout << "\n📏 Distance: " << r.totalDistance << " km\n";
        cout << "⏱ Time: " << r.totalTime << " min\n";
    }
    cout << "=====================================\n";
}

int getCityInput(const vector<string>& cities, const string& promptMsg) {
    while (true) {
        cout << "\n" << promptMsg;
        string query;
        getline(cin, query);
        
        if (query.empty()) continue;

        string lowerQuery = query;
        transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
        
        vector<int> matches;
        for (int i = 0; i < cities.size(); i++) {
            string lowerCity = cities[i];
            transform(lowerCity.begin(), lowerCity.end(), lowerCity.begin(), ::tolower);
            if (lowerCity.find(lowerQuery) != string::npos) {
                matches.push_back(i);
            }
        }
        
        if (matches.empty()) {
            cout << "❌ No cities found matching '" << query << "'. Please try again.\n";
        } else if (matches.size() == 1) {
            cout << "✅ Selected: " << cities[matches[0]] << "\n";
            return matches[0];
        } else {
            cout << "Multiple cities found. Please choose one:\n";
            for (int i = 0; i < matches.size(); i++) {
                cout << i + 1 << ". " << cities[matches[i]] << "\n";
            }
            cout << "Enter your choice (1-" << matches.size() << ") or 0 to search again: ";
            string choiceStr;
            getline(cin, choiceStr);
            int choice = 0;
            try { choice = stoi(choiceStr); } catch (...) { choice = -1; }

            if (choice < 0 || choice > matches.size()) {
                cout << "❌ Invalid choice. Searching again...\n";
                continue;
            }
            if (choice == 0) continue;
            
            cout << "✅ Selected: " << cities[matches[choice - 1]] << "\n";
            return matches[choice - 1];
        }
    }
}

void waitForUser() {
    cout << "\nPress Enter to return to menu...";
    string dummy;
    getline(cin, dummy);
}

int main() {
    vector<string> cities = {
        "Delhi", "Mumbai", "Bangalore", "Hyderabad", "Chennai", "Kolkata", "Pune", "Ahmedabad", "Surat", "Jaipur",
        "Lucknow", "Kanpur", "Nagpur", "Indore", "Thane", "Bhopal", "Vizag", "Pimpri-Chinchwad", "Patna", "Vadodara",
        "Ghaziabad", "Ludhiana", "Agra", "Nashik", "Faridabad", "Meerut", "Rajkot", "Kalyan-Dombivli", "Vasai-Virar", "Varanasi",
        "Srinagar", "Aurangabad", "Dhanbad", "Amritsar", "Navi Mumbai", "Allahabad", "Ranchi", "Howrah", "Coimbatore", "Jabalpur",
        "Gwalior", "Vijayawada", "Jodhpur", "Madurai", "Raipur", "Kota", "Guwahati", "Chandigarh", "Solapur", "Hubli-Dharwad",
        "Bareilly", "Moradabad", "Mysore", "Gurgaon", "Aligarh", "Jalandhar", "Tiruchirappalli", "Bhubaneswar", "Salem", "Mira-Bhayandar",
        "Warangal", "Guntur", "Bhiwandi", "Saharanpur", "Gorakhpur", "Bikaner", "Amravati", "Noida", "Jamshedpur", "Bhilai",
        "Cuttack", "Firozabad", "Kochi", "Nellore", "Bhavnagar", "Dehradun", "Durgapur", "Asansol", "Rourkela", "Nanded",
        "Kolhapur", "Ajmer", "Akola", "Gulbarga", "Jamnagar", "Ujjain", "Loni", "Siliguri", "Jhansi", "Ulhasnagar",
        "Jammu", "Mangalore", "Erode", "Belgaum", "Kurnool", "Ambattur", "Rajahmundry", "Tirunelveli", "Amaravathi", "Kakinada",
        "Thiruvananthapuram", "Kozhikode", "Thrissur", "Malappuram", "Kannur", "Kollam", "Kottayam", "Palakkad", "Alappuzha", "Udaipur",
        "Bhilwara", "Alwar", "Bharatpur", "Sikar", "Pali", "Sri Ganganagar", "Kishangarh", "Barmer", "Jaisalmer", "Bikaner"
    };
    
    // Sort cities alphabetically for easier display
    sort(cities.begin(), cities.end());
    // Remove duplicate cities just in case
    cities.erase(unique(cities.begin(), cities.end()), cities.end());
    
    Graph g(cities.size(), cities);

    auto hasEdge = [&](int u, int v) {
        for(auto& e : g.adj[u]) if(e.to == v) return true;
        return false;
    };

    srand(42); 
    for(int i=0; i<g.numNodes; i++) {
        int targetEdges = 4 + rand() % 4; // Each city has 4 to 7 connections
        while(g.adj[i].size() < targetEdges) {
            int target = rand() % g.numNodes;
            if(target != i && !hasEdge(i, target)) {
                int dist = rand() % 1500 + 100; // 100 km to 1600 km distance
                int traffic = rand() % 3;
                g.addEdge(i, target, dist, traffic);
            }
        }
    }

    g.randomizeTraffic();

    while (true) {
        cout << "\n========== SMART ROUTE FINDER (INDIA EDITION) ==========\n";
        cout << "--- DIRECTORY ---\n";
        cout << "1. View All Supported Cities\n";
        cout << "2. Search for a City\n";
        cout << "--- NAVIGATION ---\n";
        cout << "3. Find Shortest Path (Distance-based)\n";
        cout << "4. Find Fastest Path (Time-based)\n";
        cout << "5. Find Route (Advanced - Choose Algorithm)\n";
        cout << "6. Show All Possible Paths (Capped)\n";
        cout << "--- SYSTEM ---\n";
        cout << "7. Randomize Traffic Conditions\n";
        cout << "8. View DP Cache Stats\n";
        cout << "9. Exit\n";
        cout << "========================================================\n";
        cout << "Enter choice: ";
        
        string choiceStr;
        getline(cin, choiceStr);
        int choice = 0;
        try { choice = stoi(choiceStr); } catch (...) { choice = -1; }

        if (choice == 1) {
            cout << "\nCities Supported (" << cities.size() << " cities):\n";
            for (int i=0; i<cities.size(); i++) {
                cout << left << setw(20) << cities[i];
                if ((i + 1) % 4 == 0) cout << "\n";
            }
            cout << "\n";
            waitForUser();
        } else if (choice == 2) {
            cout << "Enter city name or part of it to search: ";
            string query;
            getline(cin, query);
            
            string lowerQuery = query;
            transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
            
            cout << "\nSearch Results:\n";
            bool found = false;
            for (const string& city : cities) {
                string lowerCity = city;
                transform(lowerCity.begin(), lowerCity.end(), lowerCity.begin(), ::tolower);
                if (lowerCity.find(lowerQuery) != string::npos) {
                    cout << "- " << city << "\n";
                    found = true;
                }
            }
            if (!found) {
                cout << "❌ No cities found matching '" << query << "'.\n";
            }
            waitForUser();
        } else if (choice >= 3 && choice <= 6) {
            int src = getCityInput(cities, "Enter source city (or part of name): ");
            int dest = getCityInput(cities, "Enter destination city (or part of name): ");

            if (choice == 6) {
                showAllPaths(g, src, dest);
            } else {
                RouteResult r;
                if (choice == 3) {
                    r = dijkstra(g, src, dest);
                } else if (choice == 4) {
                    r = astar(g, src, dest);
                } else if (choice == 5) {
                    cout << "\nChoose Algorithm:\n";
                    cout << "1. Dijkstra (Shortest Path by distance)\n";
                    cout << "2. A* (Fastest Path by time)\n";
                    cout << "3. Greedy (Quickest local decisions)\n";
                    cout << "4. DP Cached Dijkstra\n";
                    cout << "5. Bellman-Ford (Dynamic Programming)\n";
                    cout << "6. BFS (Fewest Hops)\n";
                    cout << "Enter algorithm choice: ";
                    
                    string algoChoiceStr;
                    getline(cin, algoChoiceStr);
                    int algoChoice = 0;
                    try { algoChoice = stoi(algoChoiceStr); } catch (...) { algoChoice = -1; }

                    if (algoChoice == 1) {
                        r = dijkstra(g, src, dest);
                    } else if (algoChoice == 2) {
                        r = astar(g, src, dest);
                    } else if (algoChoice == 3) {
                        r = greedy(g, src, dest);
                    } else if (algoChoice == 4) {
                        if (dpCache.has(src, dest, 1)) {
                            cout << "\n✅ Route found in Cache!\n";
                            r = dpCache.get(src, dest, 1);
                        } else {
                            cout << "\n❌ Route not in Cache. Computing with Dijkstra...\n";
                            r = dijkstra(g, src, dest);
                            r.algorithm = "Dijkstra (Cached)";
                            dpCache.put(src, dest, 1, r);
                        }
                    } else if (algoChoice == 5) {
                        r = bellmanFord(g, src, dest);
                    } else if (algoChoice == 6) {
                        r = bfsRoute(g, src, dest);
                    } else {
                        cout << "❌ Invalid algorithm choice.\n";
                        waitForUser();
                        continue;
                    }
                }
                printResult(r, g);
            }
            waitForUser();
        } else if (choice == 7) {
            g.randomizeTraffic();
            dpCache.clear();
            cout << "\n🚗 Traffic conditions randomized and cache cleared!\n";
            waitForUser();
        } else if (choice == 8) {
            dpCache.printStats();
            waitForUser();
        } else if (choice == 9) {
            cout << "\n✨ Thank you for using Smart Route Finder!\n";
            break;
        } else {
            cout << "❌ Invalid choice!\n";
            waitForUser();
        }
    }
    return 0;
}