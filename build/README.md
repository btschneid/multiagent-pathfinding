# Pathfinding Program Build and Usage Guide

## Building the Program

1. Navigate to the build directory:
   ```bash
   cd build
   ```

2. Generate the build files using CMake:
   ```bash
   cmake ..
   ```

3. Build the program:
   ```bash
   cmake --build .
   ```

## Running the Program

The program can be run with various command-line options to customize the pathfinding simulation.

### Basic Usage

Run with default settings (1 agent, default map and scenario):
```bash
./MultiAgentPathfinding
```

### Command Line Options

The program supports the following options:

- `-a, --agents <number>`: Number of agents (default: 1)
- `-m, --map <name>`: Map name (default: maze-32-32-4)
- `-f, --folder <name>`: Scenario folder (default: maze-32-32-2)
- `-s, --scenario <name>`: Scenario name (default: maze-32-32-2-even-1)
- `-h, --help`: Show help message

### Example Usage

1. Run with 5 agents:
   ```bash
   ./MultiAgentPathfinding -a 5
   ```

2. Use a different map and scenario:
   ```bash
   ./MultiAgentPathfinding -m maze-32-32-4 -f maze-32-32-2 -s maze-32-32-2-even-1
   ```

3. Combine multiple options:
   ```bash
   ./MultiAgentPathfinding -a 3 -m maze-32-32-4 -f maze-32-32-2 -s maze-32-32-2-even-1
   ```

4. Show help message:
   ```bash
   ./MultiAgentPathfinding --help
   ```

### Available Maps and Scenarios

The program looks for map files in the `maps/` directory and scenario files in the `scenarios/<folder>/` directory. Make sure these files exist before running the program.

Common map names:
- maze-32-32-4
- maze-32-32-2
- (Add other available maps)

Common scenario folders:
- maze-32-32-2
- (Add other available scenario folders)

### Error Handling

The program will validate all inputs and provide clear error messages if:
- Invalid number of agents is specified
- Map file is not found
- Scenario file is not found
- Any other runtime errors occur

### Visualization

The program will show two visualizations side by side:
1. Initial map state
2. Final map state after pathfinding

These visualizations will appear in separate windows when the program runs successfully.