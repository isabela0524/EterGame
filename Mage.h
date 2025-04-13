#pragma once
#include "Game.h"
#include <string>

class Mage
{
public:
	Mage(const std::string& name) : m_name(name), powerUsed(false){}
	virtual ~Mage() = default;
	virtual void Activate(Game& game) = 0;
	const std::string& getName() const;

protected:
	std::string m_name;
	bool powerUsed = false;

	void MarkUsedPower();
	bool IsUsedPower();
	bool IsOwnCard(Game& game, int row, int col);
};