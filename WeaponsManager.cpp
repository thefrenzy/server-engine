#include <iostream>
#include <string>
#include <fstream>
#include <cstdarg>
#include <nlohmann/json.hpp>
#include "WeaponsManager.h"
#include "Logger.h"

using json = nlohmann::json;

// Helper
std::string string_format(const std::string& fmt_str, ...) {
    va_list args;
    va_start(args, fmt_str);
    char buf[512];
    vsnprintf(buf, sizeof(buf), fmt_str.c_str(), args);
    va_end(args);
    return std::string(buf);
}

void InitWeapons(WeaponManager &wm) {
    for (int i = 0; i < MAX_WEAPONS; i++) {
        wm.weaponname[i] = "";
        wm.weaponidentifier[i] = 0;     // reset int
    }

    std::ifstream file("weaponsconfiguration.json");
    if (!file.is_open()) {
        Logger::Log("[WEAPON ERROR] Cannot open weaponsconfiguration.json");
        return;
    }

    json data;
    try {
        file >> data;
    } catch (const json::exception& e) {
        Logger::Log("[WEAPON ERROR] JSON parse failed: " + std::string(e.what()));
        return;
    }

    int index = 0;
    for (const auto& w : data["weapons"]) {
        if (index >= MAX_WEAPONS) break;

        wm.weaponidentifier[index] = w.value("id", 0);              // ← int
        wm.weaponname[index]       = w.value("name", "");
        wm.weapondamage[index]     = w.value("damage", 0);
        wm.ammocapacity[index]     = w.value("ammo_cap", 0);
        wm.magazinecapacity[index] = w.value("mag_cap", 0);
        wm.reloadtime[index]       = w.value("reload", 0.0f);

        index++;
    }

    Logger::Log("[WEAPON] Loaded " + std::to_string(index) + " weapons");
}

void displayAllWeapons(WeaponManager &wm) {
    Logger::Log("--- CURRENT WEAPON DATABASE ---");

    Logger::Log(
        string_format("%-5s %-15s %-8s %-10s %s",
                      "ID", "NAME", "DMG", "AMMO CAP", "RELOAD")
    );

    Logger::Log("--------------------------------------------------");

    int count = 0;
    for (int i = 0; i < MAX_WEAPONS; i++) {
        if (!wm.weaponname[i].empty()) {
            count++;
            Logger::Log(string_format("%-5d %-15s %-8d %-10d %.1fs",
                                      wm.weaponidentifier[i],          // ← int
                                      wm.weaponname[i].c_str(),
                                      wm.weapondamage[i],
                                      wm.ammocapacity[i],
                                      wm.reloadtime[i]));
        }
    }

    Logger::Log("--------------------------------------------------");
    Logger::Log(count ? "Total: " + std::to_string(count) : "(no weapons loaded)");
}

void displayWeapon(const std::string& weaponname, WeaponManager &wm) {
    for (int i = 0; i < MAX_WEAPONS; i++) {
        if (wm.weaponname[i] == weaponname) {
            Logger::Log("Detailed View → " + wm.weaponname[i] +
                        "  [ID: " + std::to_string(wm.weaponidentifier[i]) + "]");
            return;
        }
    }
    Logger::Log("[WEAPON] Weapon '" + weaponname + "' not found");
}