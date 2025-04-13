#include "GamemodeDistributor.h"
#include "MageDistributor.h"
#include "MagicDistributor.h"
#include <stdexcept>

GamemodeDistributor::AllGamemodes GamemodeDistributor::stringToEnum(std::string_view gamemodeName)
{
    if (gamemodeName == "Training") return AllGamemodes::Training;
    if (gamemodeName == "Mage") return AllGamemodes::Mage;
    if (gamemodeName == "Magic") return AllGamemodes::Magic;
    if (gamemodeName == "MageAndMagic") return AllGamemodes::MageAndMagic;
    if (gamemodeName == "Timer") return AllGamemodes::Timer;
    throw std::invalid_argument("Invalid gamemode name");
}

Gamemode GamemodeDistributor::generateGamemode(AllGamemodes gamemodeType, bool hasIllusions, bool hasExplosions, int seconds)
{
    /*is4x4   cardList   noOfGames  mages  magicPower  hasIlusions  hasExplosion  hasTimer*/
    constexpr uint8_t ETER = 6;
    switch (gamemodeType)
    {
    case AllGamemodes::Training:
        return Gamemode(false, { 1,1,2,2,3,3,4 }, 3, {}, {}, hasIllusions, hasExplosions, seconds);
    case AllGamemodes::Mage:
        return Gamemode(true, { 1,1,2,2,2,3,3,3,4,ETER }, 5, MageDistributor::generateMages(), {}, hasIllusions, hasExplosions, seconds);
    case AllGamemodes::Magic:
        return Gamemode(true, { 1,2,2,2,3,3,3,4,ETER }, 5, {}, MagicDistributor::generateMagicPowers(hasIllusions), hasIllusions, hasExplosions, seconds);
    case AllGamemodes::MageAndMagic:
        return Gamemode(true, { 1,2,2,2,3,3,3,4,ETER }, 5, MageDistributor::generateMages(), MagicDistributor::generateMagicPowers(hasIllusions), hasIllusions, hasExplosions, seconds);
    case AllGamemodes::Timer:
        return Gamemode(false, {}, 0, {}, {}, hasIllusions, hasExplosions, seconds);
    }
    throw std::logic_error("Unhandled gamemode type");
}

Gamemode GamemodeDistributor::generateGamemode(std::string_view gamemodeName, bool hasIllusions, bool hasExplosions, int seconds)
{
    return generateGamemode(stringToEnum(gamemodeName), hasIllusions, hasExplosions, seconds);
}
