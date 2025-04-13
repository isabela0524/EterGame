#include "Card.h"

Card::Card(uint8_t value, PlayerEnum playerID, bool isIllusion)
	:m_cardValue{ value }, m_playerID{ playerID }, m_isIllusion{ isIllusion }
{
}

bool Card::operator==(const Card& other) const
{
	return (m_cardValue == other.m_cardValue) && (m_playerID == other.m_playerID);
}

uint8_t Card::getValue() const
{
	return m_cardValue;
}

PlayerEnum Card::getPlayerID() const
{
	return m_playerID;
}

bool Card::isIllusion() const
{
	return m_isIllusion;
}

void Card::discoverIllusion()
{
	m_hasBeenDiscovered = true;
	m_isIllusion = false;
}

int Card::getRealValue() const
{
	int value;

	if (m_isIllusion || m_hasBeenDiscovered)
	{
		value = 1;
	}
	else
	{
		value = static_cast<int>(m_cardValue);
	}

	return (m_playerID == PlayerEnum::Player1) ? value : -value;
}
