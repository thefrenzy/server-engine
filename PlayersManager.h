#ifndef PLAYERS_MANAGER_H
#define PLAYERS_MANAGER_H

#include <array>
#include <string>

inline constexpr int MAX_IDS = 10;
inline constexpr int MAX_PLAYER_ATTRIBUTES = 4;

extern const std::array<std::string, MAX_PLAYER_ATTRIBUTES> attribute;

struct PlayerManager {
    std::array<int,   MAX_IDS> identifier;
    std::array<float, MAX_IDS> health;
    std::array<float, MAX_IDS> armor;
    std::array<int,   MAX_IDS> viprank;
};

void InitPlayers(PlayerManager& pl);

#endif