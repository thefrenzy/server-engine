#include "PlayersManager.h"
#include "Logger.h"
#include <string>

const std::array<std::string, MAX_PLAYER_ATTRIBUTES> attribute = {
    "identifier",
    "health",
    "armor",
    "viprank"
};

void InitPlayers(PlayerManager& pl) {
    for (int i = 0; i < MAX_IDS; ++i) {
        pl.identifier[i] = i;
        pl.health[i]     = 100.0f;
        pl.armor[i]      = 100.0f;
        pl.viprank[i]    = 0;
    }

    Logger::Log("[PLAYER] Initialized " + std::to_string(MAX_IDS) + " player slots");
}