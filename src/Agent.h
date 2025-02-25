#ifndef AGENT_H
#define AGENT_H

#include "Map.h"
#include <memory>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>

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
    AgentData data;
    std::vector<std::shared_ptr<Cell>> path;
  private:
    std::shared_ptr<Map> map;
};

#endif // AGENT_H
