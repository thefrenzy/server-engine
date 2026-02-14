#ifndef PANEL_H
#define PANEL_H

#include <string>
#include <atomic>
#include <vector>

struct MapState; 

extern std::atomic<bool> g_running;

void ExecuteCommand(std::string cmd, MapState &ms);
void RunGraphicsPanel(MapState &ms);
void Traffic_Run();

#endif