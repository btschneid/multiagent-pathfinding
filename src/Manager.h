/**
 * @file Manager.h
 * @brief Defines the Manager class for coordinating pathfinding operations.
 * 
 * This file implements the Manager class which coordinates the interaction
 * between the map, agents, and pathfinding algorithms.
 */

#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <map>
#include <string>
#include <vector>

#include "Map.h"
#include "Agent.h"
#include "CBS.h"

/**
 * @brief Manages the coordination of pathfinding operations.
 * 
 * The Manager class is responsible for:
 * - Loading and managing the map
 * - Creating and managing agents
 * - Coordinating pathfinding operations
 * - Visualizing results
 */
class Manager {
  public:
    /**
     * @brief Path to the folder containing map scenarios.
     */
    static const std::string SCENARIO_FOLDER_PATH;

    /**
     * @brief Constructs a new Manager object.
     */
    Manager();

    /**
     * @brief Loads a map from a file.
     * 
     * @param filename Path to the map file
     * @return true if the map was loaded successfully
     */
    bool LoadMap(const std::string& filename);

    /**
     * @brief Creates an agent with specified parameters.
     * 
     * @param start_row Starting row
     * @param start_col Starting column
     * @param goal_row Goal row
     * @param goal_col Goal column
     * @param agent_id Unique identifier for the agent
     * @return true if the agent was created successfully
     */
    bool CreateAgent(int start_row, int start_col, int goal_row, int goal_col, int agent_id);

    /**
     * @brief Finds paths for all agents using CBS algorithm.
     * 
     * @return true if paths were found for all agents
     */
    bool FindPaths();

    /**
     * @brief Prints the current state of the map to the console.
     */
    void PrintMap() const;

    /**
     * @brief Visualizes the current state of the map in a window.
     */
    void VisualizeMap() const;

    /**
     * @brief Visualizes the map with a custom title.
     * 
     * @param title Title to display in the visualization window
     */
    void VisualizeMapWithTitle(const std::string& title) const;

    /**
     * @brief Initializes the map with a given name.
     * 
     * @param _map_name Name of the map to initialize
     */
    void InitializeMap(const std::string& _map_name);

    /**
     * @brief Initializes a scenario with specified parameters.
     * 
     * @param _map_name Name of the map
     * @param _scenario_folder_name Name of the scenario folder
     * @param _scenario_name Name of the scenario
     * @param agents_count Number of agents to create
     */
    void InitializeScenario(const std::string& _map_name, 
                          const std::string& _scenario_folder_name, 
                          const std::string& _scenario_name, 
                          const int agents_count);

    /**
     * @brief Starts the pathfinding process for all agents.
     */
    void StartPathfind();

  private:
    std::shared_ptr<Map> map;                    ///< Shared pointer to the map
    std::vector<std::shared_ptr<Agent>> agents;  ///< Vector of agent pointers
    int next_agent_id;                           ///< Next available agent ID
    std::shared_ptr<CBS> cbs_solver;            ///< Shared pointer to the CBS solver
};

#endif // MANAGER_H