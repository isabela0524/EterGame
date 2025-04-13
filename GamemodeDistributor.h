#pragma once

#include "Gamemode.h"
#include <string_view>

class GamemodeDistributor
{
private:
    enum class AllGamemodes
    {
        Training,
        Mage,
        Magic,
        MageAndMagic,
        Timer
    };

    static Gamemode     generateGamemode(AllGamemodes gamemodeType, bool hasIllusions, bool hasExplosions, int seconds);

    static AllGamemodes stringToEnum(std::string_view gamemodeName);

public:
    static Gamemode     generateGamemode(std::string_view gamemodeName, bool hasIllusions = false, bool hasExplosions = false, int seconds = -1);
};
