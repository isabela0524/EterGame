#include "Destruction.h"
#include "Game.h"

Destruction::Destruction()
{
    m_description = "Destruction: Removes the last card played by the opponent from the game.";
}

void Destruction::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool Destruction::usePower(Game& game) 
{
    std::cout << "Destruction power activated: Removes the last card played by the opponent." << std::endl;
    
    game.accessBoard().removeCard(game.getCurrentOpponent().getLastCardPlacedCoords());
    markUsed();
    game.getCurrentPlayer().markMagicUsed();
    return true;
}
