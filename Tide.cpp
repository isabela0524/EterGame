#include "Tide.h"
#include "Game.h"

Tide::Tide()
{
    m_description = "Tide: Swaps the positions of two stacks of cards.";
}

void Tide::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool Tide::usePower(Game& game)
{
    std::cout << "Tide power activated: Swaps the positions of two stacks of cards." << std::endl;

    size_t xpos, ypos;
    std::cout << "\nEnter the 1st position for swap: ";
    std::cin >> xpos >> ypos;

    if (!game.accessBoard().validateInsertPosition({ xpos,ypos }))
    {
        return false;
    }

    if (!game.accessGameboardAPI()[xpos][ypos].has_value())
    {
        return false;
    }

    size_t xpos2, ypos2;
    std::cout << "\nEnter the 2st position for swap: ";
    std::cin >> xpos2 >> ypos2;

    if (!game.accessBoard().validateInsertPosition({ xpos2,ypos2}))
    {
        return false;
    }

    if (!game.accessGameboardAPI()[xpos2][ypos2].has_value())
    {
        return false;
    }

    std::stack<Card> saveStack;//stack for 1st

    while (!game.accessGameboardAPI()[xpos][ypos]->empty())
    {
        saveStack.push(game.accessGameboardAPI()[xpos][ypos]->top());
        game.accessBoard().removeCard({ xpos,ypos });

        if (!game.accessGameboardAPI()[xpos][ypos].has_value())
        {
            break;
        }
    }

    std::stack<Card> reverseStack;//stack for 2nd

    while (!game.accessGameboardAPI()[xpos2][ypos2]->empty())
    {
        reverseStack.push(game.accessGameboardAPI()[xpos2][ypos2]->top());
        game.accessBoard().removeCard({ xpos2,ypos2 });

        if (!game.accessGameboardAPI()[xpos2][ypos2].has_value())
        {
            break;
        }

    }

    while (!saveStack.empty())
    {
        game.accessBoard().insertCard({ saveStack.top().getValue(),saveStack.top().getPlayerID() }, { xpos2,ypos2 });
        saveStack.pop();
    }

    while (!reverseStack.empty())
    {
        game.accessBoard().insertCard({ reverseStack.top().getValue(),reverseStack.top().getPlayerID() }, { xpos,ypos });
        reverseStack.pop();
    }

    game.accessBoard().addPositionToValid({ xpos,ypos });
    game.accessBoard().addPositionToValid({ xpos2,ypos2});

    markUsed();
    game.getCurrentPlayer().markMagicUsed();
    return true;
}
