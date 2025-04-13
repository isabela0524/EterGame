#include "WaterMage.h"
#include "Game.h"

WaterMage::WaterMage()
{
	m_description = "WaterMage: Covers an opponent's card with one's own card of a strictly lower value from hand.";
}

void WaterMage::getDescription() const
{
	std::cout << m_description << std::endl;
}

bool WaterMage::usePower(Game& game) const
{
	std::cout << "WaterMage uses power to cover an opponent's card!" << std::endl;

	const uint8_t ETER{ 6u };

	int value1;
	size_t row, col;
	uint8_t value;
	std::cout << "\nEnter xPos yPos Value for insert: ";
	std::cin >> row >> col >> value1;
	value = static_cast<uint8_t>(value1);

	if (value == ETER)
	{
		std::cout << "Cannot play Eter card!";
		return false;
	}


	if (!game.accessBoard().validateInsertPosition({ row,col }))
	{
		std::cout << "wrong!\n";
		return false;
	}
	if (!game.accessBoard().validateValue(value))
	{
		std::cout << "value wrong!\n";
		return false;
	}
	if (game.accessBoard().validateStackRule({ row,col }, value))
	{
		std::cout << "Value needs to be lesser!\n";
		return false;
	}
	if (!game.getCurrentPlayer().removeCard(value))
	{
		std::cout << "Value available wrong!\n";
		return false; /*restart loop*/
	}

	//second ring of verification
	if (!game.accessBoard().isFirstMove() && !game.accessBoard().tryGridShiftForInsertPosition({ row,col }))
	{
		std::cout << "grid wrong\n";
		return false;
	}

	game.accessBoard().insertCard(Card{ value,game.getCurrentPlayerEnum() }, { row,col });

	if (game.accessBoard().getLockcase() < game.accessBoard().getMinLockcaseValue())
	{
		game.accessBoard().addPositionToValid({ row,col });
		game.accessBoard().setLockcase();
	}

	std::cout << "\n==============================================================\n";

	game.getCurrentPlayer().markMageUsed();
	return true;
}
