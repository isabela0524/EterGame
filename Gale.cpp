#include "Gale.h"
#include "Game.h"

Gale::Gale()
{
    m_description = "Gale: Moves any visible card horizontally or vertically to a position adjacent to a card with a lower number.";
}

void Gale::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool Gale::usePower(Game& game) 
{
    std::cout << "Gale power activated: Moves a visible card to a new position adjacent to a lower number card." << std::endl;

    size_t xpos, ypos;
    std::cout << "\nEnter the card position: ";
    std::cin >> xpos >> ypos;

    if (!game.accessBoard().validateInsertPosition({ xpos,ypos }))
    {
        return false;
    }

    if (!game.accessGameboardAPI()[xpos][ypos].has_value())
    {
        return false;
    }

    size_t xposInsert, yposInsert;
    std::cout << "\nEnter new position to move the card: ";
    std::cin >> xposInsert >> yposInsert;


    if (!game.accessBoard().validateInsertPosition({ xposInsert,yposInsert }))
    {
        return false;
    }


    for(size_t i = xposInsert - 1; i <= xposInsert + 1; i++)
    {
        for (size_t j = yposInsert - 1; j <= yposInsert + 1; j++)
        {
            if (!game.accessBoard().validateInsertPosition({ i,j }))
            {
                continue;
            }

            if (game.accessGameboardAPI()[i][j].has_value())
            {
                if (game.accessGameboardAPI()[i][j]->top().getValue() < game.accessGameboardAPI()[xpos][ypos]->top().getValue())
                {
                    auto card = game.accessGameboardAPI()[xpos][ypos]->top();
                    game.accessBoard().removeCard({ xpos,ypos });
                    game.accessBoard().insertCard({ card.getValue(),card.getPlayerID() }, { xposInsert,yposInsert });
                    game.accessBoard().addPositionToValid({ xpos,ypos });
                    markUsed();
                    game.getCurrentPlayer().markMagicUsed();
                    return true;
                }
            }
        }
    }



    return false;
}
