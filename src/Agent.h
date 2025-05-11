/**
 * @file Agent.h
 * @brief Defines the Agent class and related structures for pathfinding agents.
 * 
 * This file contains the data structures and interfaces for representing
 * agents that navigate through the map. It includes the AgentData structure
 * for storing agent properties and the Agent class for managing agent behavior.
 */

#ifndef AGENT_H
#define AGENT_H

#include <memory>
#include <vector>
#include "Map.h"

/**
 * @brief Structure containing the essential data for an agent.
 * 
 * This structure holds the basic properties of an agent including its
 * position, goal, and optimal path information.
 */
struct AgentData {
  int agent_id;          ///< Unique identifier for the agent
  int start_row;         ///< Starting row position
  int start_col;         ///< Starting column position
  int dest_row;          ///< Destination row position
  int dest_col;          ///< Destination column position
  double optimal_distance; ///< Pre-computed optimal path distance
};

/**
 * @brief Represents an agent that navigates through the map.
 * 
 * The Agent class manages an agent's properties and path information,
 * providing functionality for path storage and retrieval.
 */
class Agent {
public:
    /**
     * @brief Constructs a new Agent object.
     * 
     * @param _agent_id Unique identifier for the agent
     * @param _start_row Starting row position
     * @param _start_col Starting column position
     * @param _dest_row Destination row position
     * @param _dest_col Destination column position
     * @param _optimal_distance Pre-computed optimal path distance
     * @param _map Shared pointer to the map
     */
    Agent(int _agent_id, int _start_row, int _start_col, int _dest_row, int _dest_col, 
          double _optimal_distance, std::shared_ptr<Map> _map);

    /**
     * @brief Gets the agent's data.
     * 
     * @return Reference to the agent's data structure
     */
    AgentData& GetAgentData();

    /**
     * @brief Gets the agent's data (const version).
     * 
     * @return Const reference to the agent's data structure
     */
    const AgentData& GetAgentData() const;

    /**
     * @brief Gets the agent's current path.
     * 
     * @return Vector of cells representing the agent's path
     */
    std::vector<std::shared_ptr<Cell>> GetPath() const;

    /**
     * @brief Sets the agent's path.
     * 
     * @param new_path Vector of cells representing the new path
     */
    void SetPath(std::vector<std::shared_ptr<Cell>> new_path);

private:
    AgentData data;  ///< Agent's data structure
    std::vector<std::shared_ptr<Cell>> path;  ///< Agent's current path
    std::shared_ptr<Map> map;  ///< Shared pointer to the map
};

#endif // AGENT_H
