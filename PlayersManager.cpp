#include<iostream>
#include<string>

#include "PlayersManager.h"

using namespace std;

array<string, MAX_PLAYER_ATTRIBUTES> attribute{"identifier","health","armor","viprank"};

void InitPlayers(PlayerManager &pl){
    for (int i = 0; i<MAX_IDS ; i++){
        pl.identifier[i]= i;
        pl.health[i] = 100;
        pl.armor[i] = 100;
        pl.viprank[i] = i+1;
    }
    cout<< "players initialzied successfuly"<<endl;
}