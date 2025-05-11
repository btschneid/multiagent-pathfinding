/**
 * @file CBS.h
 * @brief Defines the Conflict-Based Search (CBS) algorithm and related structures.
 * 
 * This file implements the CBS algorithm for multi-agent pathfinding, including
 * constraint structures, conflict detection, and the main CBS solver class.
 */

#ifndef CBS_H
#define CBS_H

#include <vector>
#include <memory>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include "Map.h"
#include "Agent.h"
#include "AStar.h"

/**
 * @brief Represents a vertex constraint in the CBS algorithm.
 * 
 * A vertex constraint prevents an agent from being at a specific position
 * at a specific time step.
 */
struct VertexConstraint {
    int agent_id;  ///< ID of the constrained agent
    int row;       ///< Row coordinate of the constraint
    int col;       ///< Column coordinate of the constraint
    int time;      ///< Time step of the constraint

    /**
     * @brief Equality comparison operator.
     * 
     * @param other The constraint to compare with
     * @return true if constraints are equal
     */
    bool operator==(const VertexConstraint& other) const;
};

/**
 * @brief Represents an edge constraint in the CBS algorithm.
 * 
 * An edge constraint prevents an agent from moving between two positions
 * at a specific time step.
 */
struct EdgeConstraint {
    int agent_id;  ///< ID of the constrained agent
    int row1;      ///< Starting row coordinate
    int col1;      ///< Starting column coordinate
    int row2;      ///< Ending row coordinate
    int col2;      ///< Ending column coordinate
    int time;      ///< Time step of the constraint

    /**
     * @brief Equality comparison operator.
     * 
     * @param other The constraint to compare with
     * @return true if constraints are equal
     */
    bool operator==(const EdgeConstraint& other) const;
};

// Hash functions for constraints
namespace std {
    template <>
    struct hash<VertexConstraint> {
      size_t operator()(const VertexConstraint& c) const {
        return hash<int>()(c.agent_id) ^ hash<int>()(c.row) ^ 
                hash<int>()(c.col) ^ hash<int>()(c.time);
      }
    };

    template <>
    struct hash<EdgeConstraint> {
      size_t operator()(const EdgeConstraint& c) const {
        return hash<int>()(c.agent_id) ^ hash<int>()(c.row1) ^ 
                hash<int>()(c.col1) ^ hash<int>()(c.row2) ^ 
                hash<int>()(c.col2) ^ hash<int>()(c.time);
      }
    };
}

/**
 * @brief Represents a conflict between two agents.
 * 
 * A conflict can be either a vertex conflict (two agents at same position)
 * or an edge conflict (agents swapping positions).
 */
struct Conflict {
    enum Type { VERTEX, EDGE };  ///< Type of conflict
    
    Type type;      ///< Type of conflict (vertex or edge)
    int agent1_id;  ///< ID of first agent in conflict
    int agent2_id;  ///< ID of second agent in conflict
    int row;        ///< Row coordinate (for vertex conflict)
    int col;        ///< Column coordinate (for vertex conflict)
    int row1;       ///< Starting row (for edge conflict)
    int col1;       ///< Starting column (for edge conflict)
    int row2;       ///< Ending row (for edge conflict)
    int col2;       ///< Ending column (for edge conflict)
    int time;       ///< Time step of the conflict
};

/**
 * @brief Represents a set of constraints for an agent.
 * 
 * Contains both vertex and edge constraints that apply to an agent.
 */
struct Constraints {
    std::unordered_set<VertexConstraint> vertex_constraints;  ///< Set of vertex constraints
    std::unordered_set<EdgeConstraint> edge_constraints;      ///< Set of edge constraints

    /**
     * @brief Checks if a vertex constraint exists.
     * 
     * @param agent_id Agent ID to check
     * @param row Row coordinate
     * @param col Column coordinate
     * @param time Time step
     * @return true if constraint exists
     */
    bool hasVertexConstraint(int agent_id, int row, int col, int time) const;

    /**
     * @brief Checks if an edge constraint exists.
     * 
     * @param agent_id Agent ID to check
     * @param row1 Starting row
     * @param col1 Starting column
     * @param row2 Ending row
     * @param col2 Ending column
     * @param time Time step
     * @return true if constraint exists
     */
    bool hasEdgeConstraint(int agent_id, int row1, int col1, int row2, int col2, int time) const;
};

/**
 * @brief Represents a node in the CBS search tree.
 * 
 * Each node contains a set of constraints, a solution for all agents,
 * and information about any conflicts in the solution.
 */
struct CBSNode {
    Constraints constraints;  ///< Set of constraints for this node
    std::vector<std::vector<std::shared_ptr<Cell>>> solution;  ///< Paths for all agents
    int cost;  ///< Sum of path costs
    std::shared_ptr<Conflict> conflict;  ///< First conflict found in solution
    
    /**
     * @brief Comparison operator for priority queue ordering.
     * 
     * @param other Node to compare with
     * @return true if this node has higher cost
     */
    bool operator>(const CBSNode& other) const;
};

/**
 * @brief Implements the Conflict-Based Search algorithm.
 * 
 * The CBS class provides functionality for finding conflict-free paths
 * for multiple agents using the CBS algorithm.
 */
class CBS {
public:
    /**
     * @brief Constructs a new CBS solver.
     * 
     * @param _map Shared pointer to the map
     * @param _agents Vector of shared pointers to agents
     */
    CBS(std::shared_ptr<Map> _map, std::vector<std::shared_ptr<Agent>>& _agents);

    /**
     * @brief Finds conflict-free paths for all agents.
     */
    void PathFind();

private:
    std::shared_ptr<Map> map;  ///< Shared pointer to the map
    std::vector<std::shared_ptr<Agent>>& agents;  ///< Vector of agents
    
    /**
     * @brief Finds conflicts in the current solution.
     * 
     * @param solution Current solution to check
     * @return Pointer to first conflict found, or nullptr if no conflicts
     */
    std::shared_ptr<Conflict> FindConflict(const std::vector<std::vector<std::shared_ptr<Cell>>>& solution);
    
    /**
     * @brief Checks for conflicts between two agents' paths.
     * 
     * @param path1 First agent's path
     * @param path2 Second agent's path
     * @param conflict_time Output parameter for time of conflict
     * @param conflict Output parameter for conflict details
     * @return true if a conflict exists
     */
    bool HasConflict(const std::vector<std::shared_ptr<Cell>>& path1, 
                    const std::vector<std::shared_ptr<Cell>>& path2, 
                    int& conflict_time,
                    std::shared_ptr<Conflict>& conflict);
    
    /**
     * @brief Finds a path for a single agent with constraints.
     * 
     * @param agent_id ID of the agent
     * @param constraints Set of constraints to satisfy
     * @return Vector of cells representing the path
     */
    std::vector<std::shared_ptr<Cell>> FindPath(int agent_id, const Constraints& constraints);
    
    /**
     * @brief Calculates the total cost of all paths.
     * 
     * @param solution Current solution to evaluate
     * @return Total cost of all paths
     */
    int CalculateSolutionCost(const std::vector<std::vector<std::shared_ptr<Cell>>>& solution);
    
    /**
     * @brief Updates the map cells with agents' paths.
     * 
     * @param solution Current solution to visualize
     */
    void UpdateMapWithSolution(const std::vector<std::vector<std::shared_ptr<Cell>>>& solution);
};

#endif // CBS_H