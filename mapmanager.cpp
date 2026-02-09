#include "MapManager.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void LoadMapConfig(MapState &ms) {
    std::ifstream file("mapsconfiguration.json");
    if (!file.is_open()) {
        std::cerr << "[MapManager] Error: mapsconfiguration.json not found!" << std::endl;
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
    } catch (const std::exception& e) {
        std::cerr << "[MapManager] Parse Error: " << e.what() << std::endl;
    }
}

std::string SelectMap(MapState &ms, const std::string& name) {
    for (int i = 0; i < (int)ms.mapNames.size(); i++) {
        if (ms.mapNames[i] == name) {
            ms.currentMapIndex = i;
            return ms.mapNames[ms.currentMapIndex];
        }
    }
    return "";
}

std::string NextMap(MapState &ms) {
    if (ms.mapNames.empty()) return "";
    ms.currentMapIndex = (ms.currentMapIndex + 1) % (int)ms.mapNames.size();
    return ms.mapNames[ms.currentMapIndex];
}

std::string PreviousMap(MapState &ms) {
    if (ms.mapNames.empty()) return "";
    ms.currentMapIndex = (ms.currentMapIndex - 1 + (int)ms.mapNames.size()) % (int)ms.mapNames.size();
    return ms.mapNames[ms.currentMapIndex];
}

std::string RestartMap(MapState &ms) {
    if (ms.mapNames.empty()) return "";
    return ms.mapNames[ms.currentMapIndex];
}

void DisplayMaps(const MapState &ms) {
    if (ms.mapNames.empty()) {
        std::cout << "[MapManager] Registry is empty." << std::endl;
        return;
    }

    std::cout << "\n--- Server Map Registry ---" << std::endl;
    for (int i = 0; i < (int)ms.mapNames.size(); i++) {
        if (i == ms.currentMapIndex) {
            std::cout << "[ACTIVE] -> " << ms.mapNames[i] << std::endl;
        } else {
            std::cout << "          " << ms.mapNames[i] << std::endl;
        }
    }
    std::cout << "---------------------------\n" << std::endl;
}