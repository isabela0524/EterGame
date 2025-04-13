#include "Support.h"
#include "Game.h"

Support::Support()
{
    m_description = "Support: Increase the value of one of your own cards (1/2/3) by 1.";
}

void Support::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool Support::usePower(Game& game)
{
    std::cout << "Support power activated: Increase the value of one of your own cards by 1." << std::endl;

    size_t xpos, ypos;
    std::cout << "\nEnter the position of the card: ";
    std::cin >> xpos >> ypos;

    if (!game.accessBoard().validateInsertPosition({ xpos,ypos }))
    {
        return false;
    }

    if (!game.accessGameboardAPI()[xpos][ypos].has_value())
    {
        return false;
    }


    if (game.accessGameboardAPI()[xpos][ypos]->top().getPlayerID() != game.getCurrentPlayerEnum())
    {
        return false;
    }


    if (game.accessGameboardAPI()[xpos][ypos]->top().getValue() > 3)
    {
        return false;
    }

    auto card = game.accessGameboardAPI()[xpos][ypos]->top();
    uint8_t cardValue = card.getValue();
    cardValue += 1u;

    game.accessBoard().removeCard({ xpos,ypos });
    game.accessBoard().insertCard({ cardValue,card.getPlayerID()}, {xpos,ypos});
    game.accessBoard().addPositionToValid({ xpos,ypos });


    markUsed();
    game.getCurrentPlayer().markMagicUsed();
    return true;
}
