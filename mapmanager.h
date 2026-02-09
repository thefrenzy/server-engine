#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <string>
#include <vector>

struct MapState {
    std::vector<std::string> mapNames;
    int currentMapIndex = 0;
};

void LoadMapConfig(MapState &ms);
void DisplayMaps(const MapState &ms);
std::string SelectMap(MapState &ms, const std::string& name);
std::string NextMap(MapState &ms);
std::string PreviousMap(MapState &ms);
std::string RestartMap(MapState &ms);


#endif