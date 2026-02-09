#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "PlayersManager.h"
#include "WeaponsManager.h"
#include "network_engine.h"
#include "mapmanager.h"
#include "Panel.h"
#include "Logger.h" // 1. Must include the new Logger header

using namespace std;

int main() {
    PlayerManager pl;
    InitPlayers(pl);
    
    MapState ms;
    LoadMapConfig(ms);

    WeaponManager wm;
    InitWeapons(wm);

    net_init(8888);
    net_start();

    thread uiThread(RunGraphicsPanel, ref(ms));

    string incoming_data;
    bool running = true;

    // 2. Changed AddLog to Logger::Log
    Logger::Log("[SYSTEM] Server Engine Started on Port 8888");

    while (running) {
        while (net_get_next_message(incoming_data)) {
            // 3. Changed AddLog to Logger::Log
            Logger::Log("Packet Recv: " + incoming_data);
            
            if (incoming_data == "exit") {
                running = false;
            }
        }

        this_thread::sleep_for(chrono::milliseconds(10));
    }

    net_stop();

    if (uiThread.joinable()) {
        uiThread.detach(); 
    }

    return 0;
}