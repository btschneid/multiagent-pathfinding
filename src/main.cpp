/**
 * @file main.cpp
 * @brief Main entry point for the multi-agent pathfinding program.
 * 
 * This file implements the main program flow, including:
 * - Command line argument parsing
 * - Map and scenario initialization
 * - Pathfinding execution
 * - Visualization of results
 */

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <filesystem>
#include "Manager.h"
#include <SFML/Graphics.hpp>

// Default configuration values
struct Config {
    std::string map_name = "maze-32-32-4";
    std::string scenario_folder = "maze-32-32-2";
    std::string scenario_name = "maze-32-32-2-even-1";
    int agents_count = 1;
};

// Helper function to print usage information
void PrintUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [options]\n"
              << "Options:\n"
              << "  -a, --agents <number>    Number of agents (default: 1)\n"
              << "  -m, --map <name>         Map name (default: maze-32-32-4)\n"
              << "  -f, --folder <name>      Scenario folder (default: maze-32-32-2)\n"
              << "  -s, --scenario <name>    Scenario name (default: maze-32-32-2-even-1)\n"
              << "  -h, --help               Show this help message\n";
}

// Helper function to parse command line arguments
Config ParseArguments(int argc, char* argv[]) {
    Config config;
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            PrintUsage(argv[0]);
            exit(0);
        }
        else if (arg == "-a" || arg == "--agents") {
            if (i + 1 < argc) {
                try {
                    config.agents_count = std::stoi(argv[++i]);
                    if (config.agents_count <= 0) {
                        throw std::invalid_argument("Agent count must be positive");
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error: Invalid agent count. " << e.what() << std::endl;
                    exit(1);
                }
            }
        }
        else if (arg == "-m" || arg == "--map") {
            if (i + 1 < argc) {
                config.map_name = argv[++i];
            }
        }
        else if (arg == "-f" || arg == "--folder") {
            if (i + 1 < argc) {
                config.scenario_folder = argv[++i];
            }
        }
        else if (arg == "-s" || arg == "--scenario") {
            if (i + 1 < argc) {
                config.scenario_name = argv[++i];
            }
        }
    }
    
    return config;
}

// Helper function to validate file existence
bool ValidateFiles(const Config& config) {
    namespace fs = std::filesystem;
    
    // Check map file
    std::string map_path = "../tests/maps/" + config.map_name + ".map";
    if (!fs::exists(map_path)) {
        std::cerr << "Error: Map file not found: " << map_path << std::endl;
        return false;
    }
    
    // Check scenario file
    std::string scenario_path = "../tests/scenarios/" + config.scenario_folder + "/" + config.scenario_name + ".scen";
    if (!fs::exists(scenario_path)) {
        std::cerr << "Error: Scenario file not found: " << scenario_path << std::endl;
        return false;
    }
    
    return true;
}

// Helper function to run pathfinding
void RunPathfinding(Manager& manager, const Config& config) {
    try {
        // Initialize the map
        manager.InitializeMap(config.map_name);
        
        // Print initial map
        std::cout << "Initial Map:" << std::endl;
        manager.PrintMap();
        std::cout << "==========================================================\n";
        
        // Create a copy of the initial map state
        Manager initial_manager;
        initial_manager.InitializeMap(config.map_name);
        
        // Initialize the scenario
        manager.InitializeScenario(config.map_name, config.scenario_folder, 
                                 config.scenario_name, config.agents_count);
        
        // Start pathfinding
        manager.StartPathfind();
        
        // Print final map
        std::cout << "\nFinal Map:" << std::endl;
        manager.PrintMap();
        
        // Show visualizations
        std::thread initial_thread([&initial_manager]() {
            initial_manager.VisualizeMapWithTitle("Initial Map State");
        });
        
        std::thread final_thread([&manager]() {
            manager.VisualizeMapWithTitle("Final Map State");
        });
        
        initial_thread.join();
        final_thread.join();
        
    } catch (const std::exception& e) {
        std::cerr << "Error during pathfinding: " << e.what() << std::endl;
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    // Initialize SFML
    sf::RenderWindow window(sf::VideoMode(1, 1), "Initialization");
    window.close();
    
    // Parse command line arguments
    Config config = ParseArguments(argc, argv);
    
    // Validate input files
    if (!ValidateFiles(config)) {
        return 1;
    }
    
    // Create and run pathfinding
    Manager manager;
    RunPathfinding(manager, config);
    
    return 0;
}
