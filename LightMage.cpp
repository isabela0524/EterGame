#include "LightMage.h"
#include "Game.h"

LightMage::LightMage()
{
    m_description = "LightMage: Gains an extra Eter card that is immediately placed on the board.";
}

void LightMage::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool LightMage::usePower(Game& game) const
{
    std::cout << "LightMage uses power to gain an extra Eter card!" << std::endl;
    const uint8_t ETER{ 6 };

    size_t row, col;
    std::cout << "\nEnter xPos yPos Value for Eter card insert: ";
    std::cin >> row >> col;

    if (!game.accessBoard().validateInsertPosition({ row,col }))
    {
        std::cout << "Invalid Position!\n";
        return false;
    }

    if (game.accessGameboardAPI()[row][col].has_value())//fara valoare
    {
        std::cout << "\nEter card must be placed on an empty slot!";
        return false;
    }


    if (!game.accessBoard().isFirstMove() && !game.accessBoard().tryGridShiftForInsertPosition({ row,col }))
    {
        std::cout << "grid wrong\n";
        return false;
    }

    game.accessBoard().insertCard(Card{ ETER,game.getCurrentPlayerEnum() }, { row,col });

    if (game.accessBoard().getLockcase() < game.accessBoard().getMinLockcaseValue())
    {
        game.accessBoard().addPositionToValid({ row,col });
        game.accessBoard().setLockcase();
    }

    std::cout << "\n==============================================================\n";

    game.getCurrentPlayer().markMageUsed();
    return true;
}