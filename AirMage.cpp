#include "AirMage.h"
#include "Game.h"

AirMage::AirMage()
{
    m_description = "AirMage: Transforms an empty space on the board into a pit.";
}

void AirMage::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool AirMage::usePower(Game& game) const
{
    std::cout << "AirMage uses power to create a pit!" << std::endl;

    size_t row, col;
    std::cout << "\nEnter the row and column where you want to create a pit: ";
    std::cin >> row >> col;

    if (!game.accessBoard().validateInsertPosition({ row,col }))
    {
        std::cout << "\nInvalid Position!";
        return false;
    }

    if (game.accessGameboardAPI()[row][col].has_value())//cu valoare
    {
        std::cout << "\nThis space is not empty! Please choose an empty space to create a pit.\n";
        return false;
    }

    const uint8_t pitValue{ 10u };

    game.accessBoard().insertCard(Card(pitValue, game.getCurrentPlayerEnum()), { row,col });
    game.accessBoard().removePositionFromValid({ row,col });

    game.getCurrentPlayer().markMageUsed();
    return true;
}
