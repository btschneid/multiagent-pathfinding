cmake_minimum_required(VERSION 3.10)
project(MultiAgentPathfinding)

set(CMAKE_CXX_STANDARD 17)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(MultiAgentPathfinding
  src/main.cpp
  src/CBS.cpp
  src/AStar.cpp
  src/Map.cpp
  src/Agent.cpp
  src/Manager.cpp
)

# Link SFML libraries
target_link_libraries(MultiAgentPathfinding sfml-graphics sfml-window sfml-system)
