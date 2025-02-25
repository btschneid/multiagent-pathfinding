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

#include "Map.h"
#include "Agent.h"
#include "CBS.h"

class Manager {
  public:
    // Constant for the map folder path
    static const std::string SCENARIO_FOLDER_PATH;

    Manager();

    void InitializeMap(const std::string& _map_name);
    void PrintMap() const;

    void InitializeScenario(const std::string& _map_name, const std::string& _scenario_folder_name, const std::string& _scenario_name, const int agents_count);

    void StartPathfind();
  private:
    std::shared_ptr<Map> map; // Unique pointer to Map
    std::vector<std::shared_ptr<Agent>> agents; // Agent id -> Agent Obj
    int next_agent_id; // Next id to be associated with the next agent creation
};


#endif // MANAGER_H