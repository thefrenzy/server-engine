#ifndef WEAPONSMANAGER_H
#define WEAPONSMANAGER_H

#include <array>
#include <string>

using namespace std;

// Change this to a higher number to support more entities
inline const int MAX_WEAPONS = 50; 
inline const int MAX_WEAPONS_ATTRIBUTES = 6;

struct WeaponManager {
    array<int, MAX_WEAPONS> weaponidentifier;
    array<string, MAX_WEAPONS> weaponname;
    array<float, MAX_WEAPONS> weapondamage;
    array<int, MAX_WEAPONS> magazinecapacity;
    array<int, MAX_WEAPONS> ammocapacity;
    array<float, MAX_WEAPONS> reloadtime;
};

void InitWeapons(WeaponManager &wm);
void displayWeapon(const string& weaponname, WeaponManager &wm);
void displayAllWeapons(WeaponManager &wm); // Add this line

#endif