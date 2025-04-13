#pragma once
#include "PlayerEnum.h"
#include <stdint.h>
class Card
{
private:
	uint8_t		m_cardValue;
	PlayerEnum	m_playerID;
	bool		m_isIllusion;
	bool		m_hasBeenDiscovered{ false };
public:
	Card(uint8_t value, PlayerEnum playerID, bool isIllusion = false);
	bool operator==(const Card& card) const;

	uint8_t		getValue() const;
	PlayerEnum	getPlayerID() const;
	bool		isIllusion() const;
	void		discoverIllusion();
	int		getRealValue() const;
};

