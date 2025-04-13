#include "Blizzard.h"
#include "Game.h"

Blizzard::Blizzard()
{
    m_description = "Blizzard: Returns a visible opponent's card to their hand.";
}

void Blizzard::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool Blizzard::usePower(Game& game)
{
    auto& board = game.accessBoard();
    std::cout << "Blizzard power activated: Returns a visible opponent's card to their hand." << std::endl;
    
    size_t posX, posY;
    std::cout << "\nInput the position of the card to be returned: ";
    std::cin >> posX >> posY;

    if (!board.validateInsertPosition({ posX, posY })) 
    {
        std::cout << "\nInvalid Position!";
        return false;
    }

    auto& cell = game.accessGameboardAPI()[posX][posY];
    if (!cell.has_value() || cell->empty())
    {
        std::cout << "\nThis space is empty!\n";
        return false;
    }
    
    auto opponentEnum = (game.getCurrentPlayerEnum() == PlayerEnum::Player1 ? PlayerEnum::Player2 : PlayerEnum::Player1);
    if (cell->top().getPlayerID() != opponentEnum)
    {
        std::cout << "\nCannot return own card\n";
        return false;
    }
    auto value = cell->top().getValue();

    game.accessBoard().removeCard({posX,posY});
    game.getCurrentOpponent().addCard(value);

    markUsed();
    return true;
}
