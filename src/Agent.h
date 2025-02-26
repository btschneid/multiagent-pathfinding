#ifndef AGENT_H
#define AGENT_H

#include <memory>
#include <vector>
#include "Map.h"

struct AgentData {
  int agent_id;
  int start_row;
  int start_col;
  int dest_row;
  int dest_col;
  double optimal_distance;
};

class Agent {
public:
    Agent(int _agent_id, int _start_row, int _start_col, int _dest_row, int _dest_col, double _optimal_distance, std::shared_ptr<Map> _map);

    // Getters
    AgentData& GetAgentData();
    const AgentData& GetAgentData() const;
    std::vector<std::shared_ptr<Cell>> GetPath() const;

    // Setters
    void SetPath(std::vector<std::shared_ptr<Cell>> new_path);

private:
    AgentData data;
    std::vector<std::shared_ptr<Cell>> path;
    std::shared_ptr<Map> map;
};

#endif // AGENT_H
