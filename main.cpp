#include <iostream>
#include <string>
#include <array>
#include <chrono>
#include <thread>

#include "PlayersManager.h"
#include "WeaponsManager.h"
#include "network_engine.h"

using namespace std;

int main() {
    PlayerManager pl;
    InitPlayers(pl);

    WeaponManager wm;
    InitWeapons(wm);
	displayAllWeapons(wm);
    displayWeapon("AK-47", wm);
	

    net_init(8888);
    net_start();

    string incoming_data;
    bool running = true;

    while (running) {
        while (net_get_next_message(incoming_data)) {
            cout << "Data: " << incoming_data << endl;
            
            if (incoming_data == "exit") running = false;
        }

        this_thread::sleep_for(chrono::milliseconds(1));
    }

    net_stop();
    return 0;
}