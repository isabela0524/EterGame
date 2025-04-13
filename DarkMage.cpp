#include "DarkMage.h"
#include "Game.h"

DarkMage::DarkMage()
{
    m_description = "DarkMage: Moves a stack of cards with one's own card on top to another empty position on the board.";
}

void DarkMage::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool DarkMage::usePower(Game& game) const
{
    std::cout << "DarkMage uses power to move a stack of cards!" << std::endl;

    size_t rowStack, colStack;
    std::cout << "\nWhich stack do you want to move? Enter row followed by column: ";
    std::cin >> rowStack >> colStack;

    if (!game.accessBoard().validateInsertPosition({ rowStack,colStack }))
    {
        std::cout << "\nInvalid Position!";
        return false;
    }

    auto& cell = game.accessGameboardAPI()[rowStack][colStack];

    if (!cell.has_value() || cell->empty()) {
        std::cout << "\nThis space is empty! Please choose a non-empty space to move.\n";
        return false;
    }


    if (game.accessGameboardAPI()[rowStack][colStack]->top().getPlayerID() != game.getCurrentPlayerEnum())
    {
        std::cout << "\nThis stack doesn't have your card on top! You can only move your own stack.\n";
        return false;
    }

    size_t rowEmpty, colEmpty;
    std::cout << "\nEnter the empty spot where you want to move the stack: ";
    std::cin >> rowEmpty >> colEmpty;

    if (!game.accessBoard().validateInsertPosition({ rowEmpty,colEmpty }))
    {
        std::cout << "\nInvalid Position!";
        return false;
    }

    if (game.accessGameboardAPI()[rowEmpty][colEmpty].has_value())
    {
        std::cout << "\nThis position is not empty! Please choose an empty space.\n";
        return false;
    }

    game.accessGameboardAPI()[rowEmpty][colEmpty] = std::move(game.accessGameboardAPI()[rowStack][colStack]);
    game.accessGameboardAPI()[rowStack][colStack].reset();


    game.getCurrentPlayer().markMageUsed();
    return true;
}
