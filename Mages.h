#pragma once
#include "Mage.h"
#include "Game.h"

class RemoveCardMage : public Mage //Mage 1: removing opponent's card
{
public:
	RemoveCardMage();
	void Activate(Game& game) override;
};

class  RemoveRowMage : public Mage //Mage 2
{
public:
	RemoveRowMage();
	void Activate(Game& game) override;
private:
	bool IsValidRow(Game& game, int row);
};

class CoverCardMage : public Mage
{
public:
	CoverCardMage();
	void Activate(Game& game) override;
};

class CreatePitMage : public Mage
{
public:
	CreatePitMage();
	void Activate(Game& game) override;
};

class MoveStackMage : public Mage
{
public:
	MoveStackMage();
	void Activate(Game& game) override;
};

class GetEterCardMage : public Mage
{
public:
	GetEterCardMage();
	void Activate(Game& game) override;
};

class MoveCardMage : public Mage
{
public:
	MoveCardMage();
	void Activate(Game& game) override;
};

class MoveRowMage : public Mage
{
public:
	MoveRowMage();
	void Activate(Game& game) override;
};
