#ifndef CBS_H
#define CBS_H

#include <vector>
#include <memory>
#include "Agent.h"
#include "AStar.h"

class CBS {
  public:
    CBS(std::shared_ptr<Map> _map, std::vector<std::shared_ptr<Agent>>& _agents);

    void PathFind();

  private:
    std::shared_ptr<Map> map;
    std::vector<std::shared_ptr<Agent>>& agents;
};


#endif // CBS_H
