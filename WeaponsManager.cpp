#include <iostream>
#include <string>
#include <fstream>
#include <iomanip> // For better text alignment
#include <nlohmann/json.hpp>
#include "WeaponsManager.h"

using namespace std;
using json = nlohmann::json;

void InitWeapons(WeaponManager &wm) {
    // Clear the arrays first to ensure no garbage data
    for(int i = 0; i < MAX_WEAPONS; i++) {
        wm.weaponname[i] = "";
    }

    ifstream file("weaponsconfiguration.json");
    if (!file.is_open()) {
        cerr << "Error: Could not find weaponsconfiguration.json" << endl;
        return;
    }

    json data;
    file >> data;

    int index = 0;
    for (auto& w : data["weapons"]) {
        if (index >= MAX_WEAPONS) break; 

        wm.weaponidentifier[index] = w["id"];
        wm.weaponname[index] = w["name"];
        wm.weapondamage[index] = w["damage"];
        wm.ammocapacity[index] = w["ammo_cap"];
        wm.magazinecapacity[index] = w["mag_cap"];
        wm.reloadtime[index] = w["reload"];
        index++;
    }
    cout << "Loaded " << index << " weapons into the system." << endl;
}

// New function to display ALL loaded weapons
void displayAllWeapons(WeaponManager &wm) {
    cout << "\n--- CURRENT WEAPON DATABASE ---" << endl;
    cout << left << setw(5)  << "ID" 
         << setw(15) << "NAME" 
         << setw(10) << "DMG" 
         << setw(10) << "AMMO" 
         << "RELOAD" << endl;
    cout << "-----------------------------------------------" << endl;

    for (int i = 0; i < MAX_WEAPONS; i++) {
        // Only print if the name is not empty
        if (!wm.weaponname[i].empty()) {
            cout << left << setw(5)  << wm.weaponidentifier[i]
                 << setw(15) << wm.weaponname[i]
                 << setw(10) << wm.weapondamage[i]
                 << setw(10) << wm.ammocapacity[i]
                 << wm.reloadtime[i] << "s" << endl;
        }
    }
    cout << "-----------------------------------------------\n" << endl;
}

void displayWeapon(const string& weaponname, WeaponManager &wm) {
    for (int i = 0; i < MAX_WEAPONS; i++) {
        if (wm.weaponname[i] == weaponname) {
            cout << "Detailed View: " << wm.weaponname[i] 
                 << " [ID: " << wm.weaponidentifier[i] << "]" << endl;
            return;
        }
    }
    cout << "Weapon '" << weaponname << "' not found." << endl;
}