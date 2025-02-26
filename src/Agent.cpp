#include "Agent.h"

Agent::Agent(int _agent_id, int _start_row, int _start_col, int _dest_row, int _dest_col, double _optimal_distance, std::shared_ptr<Map> _map)
  : data{_agent_id, _start_row, _start_col, _dest_row, _dest_col, _optimal_distance}, map(_map) {}

// Getters
AgentData& Agent::GetAgentData() { return data; }
const AgentData& Agent::GetAgentData() const { return data; }
std::vector<std::shared_ptr<Cell>> Agent::GetPath() const { return path; }

// Setters
void Agent::SetPath(std::vector<std::shared_ptr<Cell>> new_path) { path = std::move(new_path); }
