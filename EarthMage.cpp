#include "EarthMage.h"
#include "Game.h"

EarthMage::EarthMage()
{
    m_description = "EarthMage: Removes an opponent's card from the board that is covering one's own card.";
}

void EarthMage::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool EarthMage::usePower(Game& game) const
{
    std::cout << "EarthMage uses power to remove an opponent's card!" << std::endl;

    size_t row, col;
    std::cout << "\nEnter the row and column where you want to remove a card: ";
    std::cin >> row >> col;

    if (!game.accessBoard().validateInsertPosition({ row,col }))
    {
        std::cout << "\nInvalid Position!";
        return false;
    }

    if (!game.accessGameboardAPI()[row][col].has_value())//fara valoare
    {
        std::cout << "\nThis space is empty! Please choose an non-empty space to remove a card.\n";
        return false;
    }


    if (game.accessGameboardAPI()[row][col]->top().getPlayerID() == game.getCurrentPlayerEnum())
    {
        std::cout << "\nCannot remove own card!\n";
        return false;
    }

    if (game.accessBoard().isOwnCardCoveredByEnemy({ row,col }))
    {
        game.accessBoard().removeCard({ row,col });
    }

    game.getCurrentPlayer().markMageUsed();
    return true;
}
