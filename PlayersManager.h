#include<array>
#include<string>

using namespace std;

inline const int MAX_IDS = 10;
inline const int MAX_PLAYER_ATTRIBUTES = 4;

extern array<string, MAX_PLAYER_ATTRIBUTES> attribute;

struct PlayerManager{
    array<int, MAX_IDS> identifier;
    array<float, MAX_IDS> health;
    array<float, MAX_IDS> armor;
    array<int, MAX_IDS> viprank;
};

void InitPlayers(PlayerManager &pl);