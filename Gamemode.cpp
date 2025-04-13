#include "Gamemode.h"

Gamemode::Gamemode(bool is4x4, const std::list<uint8_t>& cardList, uint8_t maxNumberOfGames, const std::vector<std::shared_ptr<AbstractMage>>& mages,
    const std::vector<std::shared_ptr<AbstractMagic>>& magicPowers, bool hasIlusions, bool hasExplosions, int timerSeconds)
    : m_is4x4(is4x4),
    m_cardList(cardList),
    m_maxNumberOfGames(maxNumberOfGames),
    m_mages(mages),
    m_magicPowers(magicPowers),
    m_hasIlusions(hasIlusions),
    m_hasExplosions(hasExplosions),
    m_timerSeconds(timerSeconds)
{
}

bool Gamemode::getIs4x4() const
{
    return m_is4x4;
}

const std::list<uint8_t>& Gamemode::getCardList() const
{
    return m_cardList;
}

uint8_t Gamemode::getMaxNumberOfGames() const
{
    return m_maxNumberOfGames;
}

const std::vector<std::shared_ptr<AbstractMage>>& Gamemode::getMages() const
{
    return m_mages;
}

const std::vector<std::shared_ptr<AbstractMagic>>& Gamemode::getMagicPowers() const
{
    return m_magicPowers;
}

bool Gamemode::getHasIlusions() const
{
    return m_hasIlusions;
}

bool Gamemode::getHasExplosions() const
{
    return m_hasExplosions;
}

int Gamemode::getTimerSeconds() const
{
    return m_timerSeconds;
}

void Gamemode::setHasIlusions(bool value)
{
    m_hasIlusions = value;
}

void Gamemode::setHasExplosions(bool value)
{
    m_hasExplosions = value;
}

void Gamemode::printSetup() const
{
    std::cout << "\n=== Gamemode Setup ===\n";
    std::cout << "Is 4x4 layout: " << (m_is4x4 ? "Yes" : "No") << std::endl;
    std::cout << "Max number of games: " << static_cast<int>(m_maxNumberOfGames) << std::endl;

    std::cout << "Card list: ";
    for (auto card : m_cardList)
    {
        std::cout << static_cast<int>(card) << " ";
    }
    std::cout << std::endl;

    std::cout << "Mages: \n";
    if (m_mages.empty()) {
        std::cout << "None";
    }
    else {
        for (const auto& mage : m_mages)
        {
            mage->getDescription();
            std::cout << '\n';
        }
    }
    std::cout << std::endl;

    std::cout << "Magic Powers: \n";
    if (m_magicPowers.empty()) {
        std::cout << "None";
    }
    else {
        for (const auto& magic : m_magicPowers)
        {
            magic->getDescription();
            std::cout << "\n";
        }
    }
    std::cout << std::endl;

    std::cout << "Has Illusions: " << (m_hasIlusions ? "Yes" : "No") << std::endl;
    std::cout << "Has Explosions: " << (m_hasExplosions ? "Yes" : "No") << std::endl;
    std::cout << "Has Timer: " << (m_timerSeconds != 0 ? "Yes" : "No") << std::endl;
    std::cout << "========================\n";
}

void Gamemode::resetUsage()
{
    for (auto& magic : m_magicPowers)
    {
        magic.get()->resetUsage();
    }
}
