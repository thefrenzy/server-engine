#ifndef WEAPONSMANAGER_H
#define WEAPONSMANAGER_H

#include <array>
#include <string>

inline const int MAX_WEAPONS = 50; 
inline const int MAX_WEAPONS_ATTRIBUTES = 6;

struct WeaponManager {
    std::array<int, MAX_WEAPONS> weaponidentifier;
    std::array<std::string, MAX_WEAPONS> weaponname;
    std::array<float, MAX_WEAPONS> weapondamage;
    std::array<int, MAX_WEAPONS> magazinecapacity;
    std::array<int, MAX_WEAPONS> ammocapacity;
    std::array<float, MAX_WEAPONS> reloadtime;
};

void InitWeapons(WeaponManager &wm);
void displayWeapon(const std::string& weaponname, WeaponManager &wm);
void displayAllWeapons(WeaponManager &wm);

#endif