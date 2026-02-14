#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <string>

#include "PlayersManager.h"
#include "WeaponsManager.h"
#include "network_engine.h"
#include "mapmanager.h"
#include "Logger.h"
#include "Panel.h" 

using namespace std;

std::atomic<bool> g_running{ true };
std::vector<std::string> server_logs;

int main() {
    PlayerManager pl; InitPlayers(pl);
    MapState ms;      LoadMapConfig(ms);
    WeaponManager wm; InitWeapons(wm);

    net_init(8888);
    net_start();

    thread uiThread(RunGraphicsPanel, ref(ms));

    Traffic_Run();

    net_stop();
    if (uiThread.joinable()) {
        uiThread.join();
    }

    return 0;
}