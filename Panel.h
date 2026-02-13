#ifndef PANEL_H
#define PANEL_H

#include "mapmanager.h"
#include <vector>
#pragma once
#include <string>
#include <atomic>

extern std::atomic<bool> g_running;
extern std::vector<std::string> server_logs;

void AddLog(std::string msg);
void ExecuteCommand(std::string cmd, MapState &ms);
void RunGraphicsPanel(MapState &ms);
void RunTextTerminal(MapState &ms);

#endif