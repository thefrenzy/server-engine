#include <iostream>
#include <string>
#include "Panel.h"
#include "mapmanager.h"
#include "Logger.h"

void ExecuteCommand(std::string cmd, MapState &ms) {
    if (cmd == "/nextmap") {
        Logger::Log("[Map System] " + NextMap(ms));
    }
    else if (cmd == "/prevmap") {
        Logger::Log("[Map System] " + PreviousMap(ms));
    }
    else if (cmd == "/list") {
        Logger::Log("--- Registered Maps ---");
        DisplayMaps(ms);
    }
}

void RunTextTerminal(MapState &ms) {
    std::string input;
    while (true) {
        if (!std::getline(std::cin, input)) break;
        if (input == "/exit") break;
        if (input.empty()) continue;
        
        ExecuteCommand(input, ms);
    }
}