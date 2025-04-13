#include "Mage.h"

const std::string& Mage::getName() const
{
	return m_name;
}

void Mage::MarkUsedPower()
{
	 powerUsed = true;
}

bool Mage::IsUsedPower()
{
	 return powerUsed; 
}
bool Mage::IsOwnCard(Game& game, int row, int col)
{
	return false;
	//return game.e(row, col) == this;
}
