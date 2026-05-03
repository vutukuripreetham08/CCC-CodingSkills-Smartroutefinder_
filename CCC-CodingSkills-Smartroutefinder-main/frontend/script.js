/**
 * Smart Route Finder - Frontend Logic
 */

// LCG for reproducible random numbers (optional, to mock C++ srand)
class Random {
    constructor(seed) {
        this.seed = seed;
    }
    next() {
        this.seed = (this.seed * 1103515245 + 12345) % 2147483648;
        return this.seed;
    }
    nextInt(max) {
        return Math.abs(this.next()) % max;
    }
    nextIntRange(min, max) {
        return min + (Math.abs(this.next()) % (max - min + 1));
    }
}

// Data: 102 Indian Cities (Unique and Sorted)
const rawCities = [
    "Agra", "Ahmedabad", "Ajmer", "Akola", "Alappuzha", "Aligarh", "Allahabad", "Alwar", "Amaravathi", "Ambattur",
    "Amravati", "Amritsar", "Asansol", "Aurangabad", "Bangalore", "Bareilly", "Barmer", "Belgaum", "Bharatpur", "Bhavnagar",
    "Bhilai", "Bhilwara", "Bhiwandi", "Bhopal", "Bhubaneswar", "Bikaner", "Chandigarh", "Chennai", "Coimbatore", "Cuttack",
    "Dehradun", "Delhi", "Dhanbad", "Durgapur", "Erode", "Faridabad", "Firozabad", "Ghaziabad", "Gorakhpur", "Gulbarga",
    "Guntur", "Gurgaon", "Guwahati", "Gwalior", "Howrah", "Hubli-Dharwad", "Hyderabad", "Indore", "Jabalpur", "Jaipur",
    "Jalandhar", "Jaisalmer", "Jammu", "Jamnagar", "Jamshedpur", "Jhansi", "Jodhpur", "Kakinada", "Kalyan-Dombivli", "Kannur",
    "Kanpur", "Kishangarh", "Kochi", "Kolhapur", "Kolkata", "Kollam", "Kota", "Kottayam", "Kozhikode", "Kurnool",
    "Loni", "Lucknow", "Ludhiana", "Madurai", "Malappuram", "Mangalore", "Meerut", "Mira-Bhayandar", "Moradabad", "Mumbai",
    "Mysore", "Nagpur", "Nanded", "Nashik", "Navi Mumbai", "Nellore", "Noida", "Palakkad", "Pali", "Patna",
    "Pimpri-Chinchwad", "Pune", "Raipur", "Rajahmundry", "Rajkot", "Ranchi", "Rourkela", "Saharanpur", "Salem", "Sikar",
    "Siliguri", "Solapur", "Sri Ganganagar", "Srinagar", "Surat", "Thane", "Thiruvananthapuram", "Thrissur", "Tiruchirappalli", "Tirunelveli",
    "Udaipur", "Ujjain", "Ulhasnagar", "Vadodara", "Varanasi", "Vasai-Virar", "Vijayawada", "Vizag", "Warangal"
];

// Graph Implementation
class Graph {
    constructor(cities) {
        this.nodes = cities;
        this.numNodes = cities.length;
        this.adj = Array.from({ length: this.numNodes }, () => []);
    }

    addEdge(u, v, dist, traffic) {
        const getTravelTime = (d, t) => d * (t === 0 ? 1 : t === 1 ? 2 : 4);
        this.adj[u].push({ to: v, distance: dist, traffic: traffic, travelTime: getTravelTime(dist, traffic) });
        this.adj[v].push({ to: u, distance: dist, traffic: traffic, travelTime: getTravelTime(dist, traffic) });
    }

    hasEdge(u, v) {
        return this.adj[u].some(e => e.to === v);
    }

    generateRandomGraph() {
        const rand = new Random(42); // Use fixed seed to always generate same topology
        
        for (let i = 0; i < this.numNodes; i++) {
            let targetEdges = 4 + rand.nextInt(4); // 4 to 7 edges
            while (this.adj[i].length < targetEdges) {
                let target = rand.nextInt(this.numNodes);
                if (target !== i && !this.hasEdge(i, target)) {
                    let dist = rand.nextIntRange(100, 1600); // 100 to 1600 km
                    let traffic = rand.nextInt(3);
                    this.addEdge(i, target, dist, traffic);
                }
            }
        }
    }

    randomizeTraffic() {
        for (let u = 0; u < this.numNodes; u++) {
            for (let e of this.adj[u]) {
                e.traffic = Math.floor(Math.random() * 3);
                e.travelTime = e.distance * (e.traffic === 0 ? 1 : e.traffic === 1 ? 2 : 4);
            }
        }
    }
}

// Priority Queue for Algorithms
class PriorityQueue {
    constructor() {
        this.elements = [];
    }
    enqueue(element, priority) {
        this.elements.push({ element, priority });
        this.elements.sort((a, b) => a.priority - b.priority);
    }
    dequeue() {
        return this.elements.shift();
    }
    isEmpty() {
        return this.elements.length === 0;
    }
}

// Algorithms
const Algorithms = {
    // Dijkstra (Shortest Path by distance)
    dijkstra: (graph, src, dest) => {
        let dist = Array(graph.numNodes).fill(Infinity);
        let parent = Array(graph.numNodes).fill(-1);
        let edgeUsed = Array(graph.numNodes).fill(null);
        let pq = new PriorityQueue();

        dist[src] = 0;
        pq.enqueue(src, 0);

        while (!pq.isEmpty()) {
            let { element: u, priority: d } = pq.dequeue();
            if (d > dist[u]) continue;

            if (u === dest) break;

            for (let e of graph.adj[u]) {
                if (dist[u] + e.distance < dist[e.to]) {
                    dist[e.to] = dist[u] + e.distance;
                    parent[e.to] = u;
                    edgeUsed[e.to] = e;
                    pq.enqueue(e.to, dist[e.to]);
                }
            }
        }
        return buildResult(graph, dest, parent, edgeUsed);
    },

    // A* (Fastest Path by time) - In this graph without coords, A* operates like Dijkstra on travelTime
    astar: (graph, src, dest) => {
        let time = Array(graph.numNodes).fill(Infinity);
        let parent = Array(graph.numNodes).fill(-1);
        let edgeUsed = Array(graph.numNodes).fill(null);
        let pq = new PriorityQueue();

        time[src] = 0;
        pq.enqueue(src, 0);

        while (!pq.isEmpty()) {
            let { element: u, priority: t } = pq.dequeue();
            if (t > time[u]) continue;

            if (u === dest) break;

            for (let e of graph.adj[u]) {
                if (time[u] + e.travelTime < time[e.to]) {
                    time[e.to] = time[u] + e.travelTime;
                    parent[e.to] = u;
                    edgeUsed[e.to] = e;
                    pq.enqueue(e.to, time[e.to]);
                }
            }
        }
        return buildResult(graph, dest, parent, edgeUsed);
    },

    // Greedy (Quickest Local Decisions based on time)
    greedy: (graph, src, dest) => {
        let parent = Array(graph.numNodes).fill(-1);
        let edgeUsed = Array(graph.numNodes).fill(null);
        let visited = Array(graph.numNodes).fill(false);
        
        let curr = src;
        visited[curr] = true;

        while (curr !== dest) {
            let bestTime = Infinity;
            let nextNode = -1;
            let bestEdge = null;

            for (let e of graph.adj[curr]) {
                if (!visited[e.to] && e.travelTime < bestTime) {
                    bestTime = e.travelTime;
                    nextNode = e.to;
                    bestEdge = e;
                }
            }

            if (nextNode === -1) return { found: false }; // Stuck

            parent[nextNode] = curr;
            edgeUsed[nextNode] = bestEdge;
            visited[nextNode] = true;
            curr = nextNode;
        }
        return buildResult(graph, dest, parent, edgeUsed);
    },

    // BFS (Fewest Hops)
    bfs: (graph, src, dest) => {
        let parent = Array(graph.numNodes).fill(-1);
        let edgeUsed = Array(graph.numNodes).fill(null);
        let visited = Array(graph.numNodes).fill(false);
        let queue = [src];
        visited[src] = true;

        while (queue.length > 0) {
            let u = queue.shift();
            if (u === dest) break;

            for (let e of graph.adj[u]) {
                if (!visited[e.to]) {
                    visited[e.to] = true;
                    parent[e.to] = u;
                    edgeUsed[e.to] = e;
                    queue.push(e.to);
                }
            }
        }
        return buildResult(graph, dest, parent, edgeUsed);
    }
};

function buildResult(graph, dest, parent, edgeUsed) {
    if (parent[dest] === -1) return { found: false };

    let path = [];
    let edges = [];
    let curr = dest;

    while (curr !== -1) {
        path.push(curr);
        if (edgeUsed[curr]) edges.push(edgeUsed[curr]);
        curr = parent[curr];
    }
    
    path.reverse();
    edges.reverse();

    let totalDist = edges.reduce((acc, e) => acc + e.distance, 0);
    let totalTime = edges.reduce((acc, e) => acc + e.travelTime, 0);

    return { found: true, path, edges, totalDistance: totalDist, totalTime: totalTime };
}

// App Initialization & UI Logic
document.addEventListener("DOMContentLoaded", () => {
    // 1. Initialize Data
    const graph = new Graph(rawCities);
    graph.generateRandomGraph();

    // 2. Populate Dropdowns
    const sourceSelect = document.getElementById("source");
    const destSelect = document.getElementById("destination");
    
    rawCities.forEach((city, index) => {
        sourceSelect.add(new Option(city, index));
        destSelect.add(new Option(city, index));
    });

    // Default selection
    sourceSelect.value = "31"; // Delhi
    destSelect.value = "73"; // Mumbai

    // 3. Setup Elements
    const swapBtn = document.getElementById("swap-btn");
    const findRouteBtn = document.getElementById("find-route-btn");
    const randomizeTrafficBtn = document.getElementById("randomize-traffic-btn");
    const algorithmSelect = document.getElementById("algorithm");

    const loadingDiv = document.getElementById("loading");
    const noResultDiv = document.getElementById("no-result");
    const routeResultDiv = document.getElementById("route-result");

    const resDistance = document.getElementById("res-distance");
    const resTime = document.getElementById("res-time");
    const pathTimeline = document.getElementById("path-timeline");

    // 4. Events
    swapBtn.addEventListener("click", () => {
        let temp = sourceSelect.value;
        sourceSelect.value = destSelect.value;
        destSelect.value = temp;
    });

    randomizeTrafficBtn.addEventListener("click", () => {
        graph.randomizeTraffic();
        // Visual feedback
        const originalText = randomizeTrafficBtn.innerHTML;
        randomizeTrafficBtn.innerHTML = '<i class="fa-solid fa-check"></i> Traffic Updated!';
        setTimeout(() => randomizeTrafficBtn.innerHTML = originalText, 2000);
    });

    findRouteBtn.addEventListener("click", () => {
        const src = parseInt(sourceSelect.value);
        const dest = parseInt(destSelect.value);
        const algo = algorithmSelect.value;

        if (src === dest) {
            alert("Source and destination cannot be the same!");
            return;
        }

        // Show loading
        noResultDiv.classList.add("hidden");
        routeResultDiv.classList.add("hidden");
        loadingDiv.classList.remove("hidden");

        // Simulate short compute delay for UI effect
        setTimeout(() => {
            const result = Algorithms[algo](graph, src, dest);
            displayResult(result, graph);
        }, 400);
    });

    function displayResult(result, graph) {
        loadingDiv.classList.add("hidden");

        if (!result.found) {
            noResultDiv.innerHTML = `
                <div class="icon-bg" style="color: var(--accent);"><i class="fa-solid fa-triangle-exclamation"></i></div>
                <h3>Route Not Found</h3>
                <p>No valid path exists between the selected cities.</p>
            `;
            noResultDiv.classList.remove("hidden");
            return;
        }

        // Update stats
        resDistance.innerText = result.totalDistance;
        resTime.innerText = result.totalTime;

        // Build Timeline
        pathTimeline.innerHTML = '';
        
        result.path.forEach((nodeIdx, i) => {
            const isStart = i === 0;
            const isEnd = i === result.path.length - 1;
            
            let edgeInfoHTML = '';
            if (!isEnd) {
                const edge = result.edges[i];
                const trafficClass = edge.traffic === 0 ? 'traffic-low' : (edge.traffic === 1 ? 'traffic-mod' : 'traffic-high');
                const trafficLabel = edge.traffic === 0 ? 'Low' : (edge.traffic === 1 ? 'Moderate' : 'Heavy');
                
                edgeInfoHTML = `
                    <div class="edge-info">
                        <span><i class="fa-solid fa-road"></i> ${edge.distance} km</span>
                        <span class="${trafficClass}"><i class="fa-solid fa-car"></i> ${trafficLabel} Traffic</span>
                        <span><i class="fa-solid fa-stopwatch"></i> ${edge.travelTime} min</span>
                    </div>
                `;
            }

            const item = document.createElement("div");
            item.className = `timeline-item ${isStart ? 'start' : ''} ${isEnd ? 'end' : ''}`;
            
            item.innerHTML = `
                <div class="timeline-line"></div>
                <div class="timeline-dot"><i class="fa-solid ${isStart ? 'fa-location-dot' : (isEnd ? 'fa-flag-checkered' : 'fa-circle-small')}"></i></div>
                <div class="timeline-content">
                    <div>
                        <div class="city-name">${graph.nodes[nodeIdx]}</div>
                        ${edgeInfoHTML}
                    </div>
                </div>
            `;
            
            pathTimeline.appendChild(item);
        });

        routeResultDiv.classList.remove("hidden");
    }
});
