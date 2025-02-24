#ifndef AGENT_H
#define AGENT_H

#include "Map.h"
#include "AStar.h"
#include "CBS.h"
#include <memory>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>

struct AgentData {
  int agent_id;
  int start_x;
  int start_y;
  int dest_x;
  int dest_y;
  double optimal_distance;
};

class Agent {
  public:
    Agent(int _agent_id, int _start_x, int _start_y, int _dest_x, int _dest_y, double _optimal_distance, std::shared_ptr<Map> _map);

    std::vector<std::shared_ptr<Cell>> Pathfind();

  private:
    AgentData data;
    std::vector<std::shared_ptr<Cell>> path;
    std::shared_ptr<Map> map;
};

#endif // AGENT_H
