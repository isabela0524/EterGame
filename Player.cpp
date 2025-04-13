#include "Player.h"
#include <numeric>
#include <algorithm>

Player::Player(const std::reference_wrapper<const Gamemode>& gamemode, const std::reference_wrapper<const Bridge>& bridge)
    :m_gamemode{ gamemode },
    m_bridge{ bridge },
    m_numberOfGamesWon{ 0 },
    m_hasUsedMage{ gamemode.get().getMages().empty() },
    m_hasUsedMagicPower{ gamemode.get().getMagicPowers().empty() },
    m_hasUsedIllusion{ false },
    m_canUseExplosions{ false },
    mtest_lastPlacedCardCoords{0,0}
{
    initialisePlayerCards();
}

void Player::initialisePlayerCards()
{
    m_cards = m_gamemode.get().getCardList();
}

void Player::printPlayerCards() const
{
    if (m_cards.empty())
    {
        std::cout << "No cards to display\n";
    }
    else {
        std::cout << "\n";
        for (const auto& cardValue : m_cards)
        {
            std::cout << static_cast<int>(cardValue) << " ";
        }
    }
}

uint8_t& Player::getNumberOfGamesWon()
{
    return m_numberOfGamesWon;
}

void Player::incrementGamesWon()
{
    ++m_numberOfGamesWon;
}

void Player::addCard(uint8_t card)
{
    m_cards.push_back(card);
}

bool Player::removeCard(uint8_t cardValue)
{
    auto it = std::find(m_cards.begin(), m_cards.end(), cardValue);
    if (it != m_cards.end())
    {
        static_cast<int>(*it);
        m_cards.erase(it);
        return true;
    }

    return false;
}

bool Player::hasUsedAllCards() const
{
    return m_cards.empty() == true;
}

bool Player::hasUsedMage() const
{
    return m_hasUsedMage == true;
}

bool Player::hasUsedMagic() const
{
    return m_hasUsedMagicPower == true;
}

const Gamemode& Player::getGamemode() const
{
    return m_gamemode;
}

bool Player::hasUsedIllusion() const
{
    return m_hasUsedIllusion;
}

void Player::markIllusionUsed()
{
    m_hasUsedIllusion = true;
}

void Player::markMageUsed()
{
    m_hasUsedMage = true;
}

void Player::markMagicUsed()
{
    m_hasUsedMagicPower = true;
}

void Player::resetPlayer()
{
    m_cards = m_gamemode.get().getCardList();
}

void Player::resetMagic()
{
    m_hasUsedMagicPower = false;
}

void Player::enableExplosion()
{
    m_canUseExplosions = true;
}

bool Player::canUseExplosion() const
{
    return m_canUseExplosions;
}

const std::pair<size_t, size_t>& Player::getLastCardPlacedCoords() const
{
    return mtest_lastPlacedCardCoords;
}

void Player::updateLastCardPlaced(const std::pair<size_t, size_t>& position)
{
    mtest_lastPlacedCardCoords = position;
}

