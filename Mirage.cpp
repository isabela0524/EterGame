#include "Mirage.h"
#include "Game.h"

Mirage::Mirage()
{
    m_description = "Mirage: Replace your placed illusion with another illusion.";
}

void Mirage::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool Mirage::usePower(Game& game)
{
    std::cout << "Mirage power activated: Replace your illusion with another illusion." << std::endl;

    size_t xpos, ypos;
    std::cout << "\nEnter the position your own illusion: ";
    std::cin >> xpos >> ypos;

    if (!game.accessBoard().validateInsertPosition({ xpos,ypos }))
    {
        return false;
    }

    if (!game.accessGameboardAPI()[xpos][ypos].has_value())
    {
        return false;
    }

    if (!game.accessGameboardAPI()[xpos][ypos]->top().isIllusion() || game.accessGameboardAPI()[xpos][ypos]->top().getPlayerID() != game.getCurrentPlayerEnum())
    {
        return false;
    }
    game.accessBoard().removeCard({ xpos,ypos });
    game.placeIllusion();
    markUsed();
    game.getCurrentPlayer().markMagicUsed();


    return false;
}
