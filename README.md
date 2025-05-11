# Multi-Agent Pathfinding with Conflict-Based Search (CBS)

This project implements and visualizes the Conflict-Based Search (CBS) algorithm for multi-agent pathfinding problems. It provides an interactive way to observe how multiple agents navigate through a shared environment while avoiding conflicts.

## Project Overview

The project allows you to:
- Visualize multi-agent pathfinding in real-time
- Test different scenarios with varying numbers of agents
- Compare initial and final states of the environment
- Use different maps and scenarios from the MovingAI benchmark

## Conflict-Based Search (CBS) Algorithm

CBS is a two-level search algorithm designed to solve multi-agent pathfinding problems optimally. Here's how it works:

### High-Level Search
- Maintains a constraint tree where each node contains:
  - A set of constraints (conflicts between agents)
  - A solution (paths for all agents that satisfy the constraints)
  - A cost (sum of all path costs)

### Low-Level Search
- Uses A* algorithm to find optimal paths for individual agents
- Takes into account the constraints from the high-level search
- Ensures agents don't collide with each other or obstacles

### Process
1. Start with no constraints and find individual optimal paths
2. If conflicts exist:
   - Identify the first conflict
   - Create two new nodes with additional constraints
   - Resolve the conflict by forcing agents to avoid the conflict location
3. Continue until a conflict-free solution is found

## Building the Project

### Prerequisites
- CMake (version 3.10 or higher)
- C++ compiler with C++17 support
- SFML library for visualization

### Build Steps

1. Enter the build directory:
   ```bash
   cd build
   ```

2. Generate build files:
   ```bash
   cmake ..
   ```

3. Build the project:
   ```bash
   cmake --build .
   ```

## Running the Program

The program supports various command-line options:

```bash
./MultiAgentPathfinding [options]
```

### Available Options:
- `-a, --agents <number>`: Number of agents (default: 1)
- `-m, --map <name>`: Map name (default: maze-32-32-4)
- `-f, --folder <name>`: Scenario folder (default: maze-32-32-2)
- `-s, --scenario <name>`: Scenario name (default: maze-32-32-2-even-1)
- `-h, --help`: Show help message

### Example Usage:
```bash
# Run with 5 agents
./MultiAgentPathfinding -a 5

# Use specific map and scenario
./MultiAgentPathfinding -m maze-32-32-4 -f maze-32-32-2 -s maze-32-32-2-even-1

# Combine options
./MultiAgentPathfinding -a 3 -m maze-32-32-4 -f maze-32-32-2 -s maze-32-32-2-even-1
```

## Map and Scenario Format

The program uses the MovingAI benchmark format for maps and scenarios:

### Map Format (.map)
```
type octile
height 32
width 32
map
........................
................%.......
.......%...............%
.....@..................
........................
```
- `.`: Walkable terrain
- `@`: Hard obstacle
- `%`: Soft obstacle

### Scenario Format (.scen)
```
version 1
3	maze-32-32-2.map	32	32	17	21	15	16	13.82842712
```
- Contains start and goal positions for agents
- Includes optimal path costs

## Visualization

The program provides two visualizations:
1. Initial map state
2. Final map state after pathfinding

These visualizations appear in separate windows, allowing you to compare the states before and after the pathfinding process.

## Project Structure

```
.
├── src/
│   ├── AStar.cpp/h      # A* pathfinding implementation
│   ├── Agent.cpp/h      # Agent class definition
│   ├── CBS.cpp/h        # Conflict-Based Search implementation
│   ├── Manager.cpp/h    # Program management and visualization
│   ├── Map.cpp/h        # Map handling and parsing
│   └── main.cpp         # Program entry point
├── maps/                # Map files
├── scenarios/           # Scenario files
└── build/              # Build directory
```