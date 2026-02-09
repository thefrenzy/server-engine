#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "mapmanager.h"
#include "Logger.h"

using json = nlohmann::json;

void LoadMapConfig(MapState &ms) {
    std::ifstream file("mapsconfiguration.json");
    if (!file.is_open()) {
        Logger::Log("[MAPS] mapsconfiguration.json not found!");
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
        Logger::Log("[MAPS] Successfully loaded " + std::to_string(ms.mapNames.size()) + " maps.");
    } catch (const std::exception& e) {
        Logger::Log("[MAPS] Parse Error: " + std::string(e.what()));
    }
}

std::string NextMap(MapState &ms) {
    if (ms.mapNames.empty()) return "No Maps";
    ms.currentMapIndex = (ms.currentMapIndex + 1) % static_cast<int>(ms.mapNames.size());
    return ms.mapNames[ms.currentMapIndex];
}

std::string PreviousMap(MapState &ms) {
    if (ms.mapNames.empty()) return "No Maps";
    ms.currentMapIndex = (ms.currentMapIndex - 1 + static_cast<int>(ms.mapNames.size())) % static_cast<int>(ms.mapNames.size());
    return ms.mapNames[ms.currentMapIndex];
}

// Don't forget these two! Even if you don't use them yet,
// the linker needs them because they are in the header.
std::string SelectMap(MapState &ms, const std::string& name) {
    for (size_t i = 0; i < ms.mapNames.size(); ++i) {
        if (ms.mapNames[i] == name) {
            ms.currentMapIndex = static_cast<int>(i);
            return ms.mapNames[i];
        }
    }
    return "";
}

std::string RestartMap(MapState &ms) {
    if (ms.mapNames.empty()) return "";
    return ms.mapNames[ms.currentMapIndex];
}

void DisplayMaps(const MapState &ms) {
    for (const auto& m : ms.mapNames) {
        Logger::Log(" - " + m);
    }
}