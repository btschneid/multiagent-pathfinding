#include "Agent.h"

// Constructor for Agent
Agent::Agent(int _agent_id, int _start_x, int _start_y, int _dest_x, int _dest_y, double _optimal_distance, std::shared_ptr<Map> _map)
    : data({_agent_id, _start_x, _start_y, _dest_x, _dest_y, _optimal_distance}), map(_map) {}

// Pathfind method using BFS
std::vector<std::shared_ptr<Cell>> Agent::Pathfind() {
    // Directions for 4-way movement (Manhattan distance)
    std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    // Queue to hold positions to visit (stores coordinates and parent node)
    std::queue<std::pair<int, int>> frontier;
    frontier.push({data.start_x, data.start_y});

    // Map to keep track of visited nodes
    std::unordered_set<std::string> visited;

    // To reconstruct the path
    std::unordered_map<std::string, std::pair<int, int>> came_from;

    // Mark the start position as visited
    visited.insert(std::to_string(data.start_x) + "," + std::to_string(data.start_y));

    // BFS loop
    while (!frontier.empty()) {
        auto current = frontier.front();
        frontier.pop();
        int x = current.first;
        int y = current.second;


        // If we reached the destination, reconstruct the path
        if (x == data.dest_x && y == data.dest_y) {
            std::vector<std::shared_ptr<Cell>> path;
            while (came_from.find(std::to_string(x) + "," + std::to_string(y)) != came_from.end()) {
                // std::cout << "Cell: " << map->GetCell(x, y)->icon << "\n";
                path.push_back(map->GetCell(x, y)); // Get the cell from map and add to path
                auto prev = came_from[std::to_string(x) + "," + std::to_string(y)];
                x = prev.first;
                y = prev.second;
            }
            // std::cout << "Cell: " << map->GetCell(data.start_x, data.start_y)->icon << "\n";
            path.push_back(map->GetCell(data.start_x, data.start_y)); // Add start cell to path
            std::reverse(path.begin(), path.end()); // Reverse the path to go from start to destination
            return path;
        }

        // Explore neighbors
        for (const auto& direction : directions) {
            int new_x = x + direction.first;
            int new_y = y + direction.second;

            // Check if the new position is within bounds and not an obstacle
            if ((new_x == data.dest_x && new_y == data.dest_y) || (map->IsInBounds(new_x, new_y) && !map->IsObstacle(new_x, new_y) && visited.find(std::to_string(new_x) + "," + std::to_string(new_y)) == visited.end())) {
                
                frontier.push({new_x, new_y});
                visited.insert(std::to_string(new_x) + "," + std::to_string(new_y));
                came_from[std::to_string(new_x) + "," + std::to_string(new_y)] = {x, y}; // Track the parent
            }
        }
    }

    return {}; // Return empty path if no path is found
}

