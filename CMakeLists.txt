cmake_minimum_required(VERSION 3.10)
project(MultiAgentPathfinding)

set(CMAKE_CXX_STANDARD 17)

add_executable(MultiAgentPathfinding
  src/main.cpp
  src/CBS.cpp
  src/AStar.cpp
  src/Map.cpp
  src/Agent.cpp
  src/DebuggingUtils.cpp
  src/Manager.cpp
)
