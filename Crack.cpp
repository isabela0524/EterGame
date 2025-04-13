#include "Crack.h"
#include "Game.h"

Crack::Crack()
{
    m_description = "Crack: Decrease the value of an opponent's card (2/3/4) by 1.";
}

void Crack::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool Crack::usePower(Game& game)
{
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


    if (game.accessGameboardAPI()[xpos][ypos]->top().getPlayerID() == game.getCurrentPlayerEnum())
    {
        return false;
    }


    if (game.accessGameboardAPI()[xpos][ypos]->top().getValue() <= 1 || game.accessGameboardAPI()[xpos][ypos]->top().getValue() > 4)//2 3 4 <=1 || >4
    {
        return false;
    }

    auto card = game.accessGameboardAPI()[xpos][ypos]->top();
    uint8_t cardValue = card.getValue();
    cardValue -= 1u;

    game.accessBoard().removeCard({ xpos,ypos });
    game.accessBoard().insertCard({ cardValue,card.getPlayerID() }, { xpos,ypos });
    game.accessBoard().addPositionToValid({ xpos,ypos });


    markUsed();
    game.getCurrentPlayer().markMagicUsed();
}
