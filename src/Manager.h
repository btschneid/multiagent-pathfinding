#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iomanip>

#include "Map.h"
#include "Agent.h"

class Manager {
  public:
    // Constant for the map folder path
    static const std::string SCENARIO_FOLDER_PATH;

    Manager();

    void InitializeMap(const std::string& _map_name);
    void PrintMap() const;

    void InitializeScenario(const std::string& _map_name, const std::string& _scenario_folder_name, const std::string& _scenario_name, const int agents_count);


  private:
    std::unique_ptr<Map> map; // Unique pointer to Map
};


#endif // MANAGER_H