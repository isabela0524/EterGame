#include "IceMage.h"
#include "Game.h"

IceMage::IceMage()
{
    m_description = "IceMage: Moves a stack of cards with the opponent's card on top to another empty position on the board.";
}

void IceMage::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool IceMage::usePower(Game& game) const
{
    std::cout << "IceMage uses power to move an opponent's stack of cards!" << std::endl;

    size_t rowStack, colStack;
    std::cout << "\nWhich stack do you want to move? Enter row followed by column: ";
    std::cin >> rowStack >> colStack;

    if (!game.accessBoard().validateInsertPosition({ rowStack,colStack }))
    {
        std::cout << "\nInvalid Position!";
        return false;
    }

    if (!game.accessGameboardAPI()[rowStack][colStack].has_value())//fara valoare
    {
        std::cout << "\nThis space is empty! Please choose a non-empty space to move.\n";
        return false;
    }

    if (game.accessGameboardAPI()[rowStack][colStack]->top().getPlayerID() == game.getCurrentPlayerEnum())
    {
        std::cout << "\nThis stack has your card on top! You cannot move your own stack.\n";
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

    if (game.accessGameboardAPI()[rowEmpty][colEmpty].has_value())//cu valoare
    {
        std::cout << "\nThis position is not empty! Please choose an empty space.\n";
        return false;
    }

    
    game.accessGameboardAPI()[rowEmpty][colEmpty] = std::move(game.accessGameboardAPI()[rowStack][colStack]);
    game.accessGameboardAPI()[rowStack][colStack].reset();


    game.getCurrentPlayer().markMageUsed();
    return true;
}