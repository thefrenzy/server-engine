#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "mapmanager.h"
#include "Logger.h"

using json = nlohmann::json;

// --- CONFIGURATION LOADING ---
void LoadMapConfig(MapState &ms) {
    std::ifstream file("mapsconfiguration.json");
    if (!file.is_open()) {
        Logger::Log("[MAP ERROR] mapsconfiguration.json not found!");
        return;
    }

    json data;
    try {
        file >> data;
        ms.mapNames.clear();

        for (const auto& name : data["maps"]) {
            ms.mapNames.push_back(name.get<std::string>());
        }

        ms.currentMapIndex = 0;
        Logger::Log("[MAP] Successfully loaded " + std::to_string(ms.mapNames.size()) + " maps.");
        
    } catch (const std::exception& e) {
        Logger::Log("[MAP ERROR] Parse Error: " + std::string(e.what()));
    }
}

// --- NAVIGATION FUNCTIONS ---

std::string NextMap(MapState &ms) {
    if (ms.mapNames.empty()) return "No Maps Loaded";
    
    ms.currentMapIndex = (ms.currentMapIndex + 1) % (int)ms.mapNames.size();
    std::string newMap = ms.mapNames[ms.currentMapIndex];
    
    Logger::Log("[MAP] Switched to Next: " + newMap);
    return newMap;
}

std::string PreviousMap(MapState &ms) {
    if (ms.mapNames.empty()) return "No Maps Loaded";
    
    ms.currentMapIndex = (ms.currentMapIndex - 1 + (int)ms.mapNames.size()) % (int)ms.mapNames.size();
    std::string newMap = ms.mapNames[ms.currentMapIndex];
    
    Logger::Log("[MAP] Switched to Previous: " + newMap);
    return newMap;
}

// --- UTILITY FUNCTIONS ---

std::string SelectMap(MapState &ms, const std::string& name) {
    for (int i = 0; i < (int)ms.mapNames.size(); i++) {
        if (ms.mapNames[i] == name) {
            ms.currentMapIndex = i;
            Logger::Log("[MAP] Manually Selected: " + name);
            return name;
        }
    }
    Logger::Log("[MAP ERROR] Map not found: " + name);
    return "";
}

std::string RestartMap(MapState &ms) {
    if (ms.mapNames.empty()) return "No Map to Restart";
    
    std::string current = ms.mapNames[ms.currentMapIndex];
    Logger::Log("[MAP] Restarting current: " + current);
    return current;
}

void DisplayMaps(const MapState &ms) {
    if (ms.mapNames.empty()) {
        Logger::Log("[SERVER] [MAP] Registry is empty.");
        return;
    }

    Logger::Log("[SERVER] --- Active MAP Registry ---");
    for (int i = 0; i < (int)ms.mapNames.size(); i++) {
        if (i == ms.currentMapIndex) {
            Logger::Log("[SERVER] [MAP] [ACTIVE] -> " + ms.mapNames[i]);
        } else {
            Logger::Log("[SERVER] [MAP]          " + ms.mapNames[i]);
        }
    }
}