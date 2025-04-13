#include "Wave.h"
#include "Game.h"

Wave::Wave()
{
    m_description = "Wave: Move a stack to an adjacent empty position. Play a card on the new empty position.";
}

void Wave::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool Wave::usePower(Game& game)
{
    std::cout << "Wave power activated: Move a stack to an adjacent empty position. Play a card on the new empty position." << std::endl;
    size_t xpos, ypos;
    std::cout << "\nEnter the position of the stack: ";
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
    std::cout << "\nEnter the position of the adjacent position: ";
    std::cin >> xpos2 >> ypos2;

    if (!game.accessBoard().validateInsertPosition({ xpos,ypos }))
    {
        return false;
    }

    if (!game.accessGameboardAPI()[xpos][ypos].has_value())
    {
        return false;
    }

    if (!((xpos2 == xpos - 1 && ypos2 == ypos) ||  // Up
        (xpos2 == xpos + 1 && ypos2 == ypos) ||  // Down
        (xpos2 == xpos && ypos2 == ypos - 1) ||  // Left
        (xpos2 == xpos && ypos2 == ypos + 1) ||  // Right
        (xpos2 == xpos - 1 && ypos2 == ypos - 1) ||  // Up-Left
        (xpos2 == xpos - 1 && ypos2 == ypos + 1) ||  // Up-Right
        (xpos2 == xpos + 1 && ypos2 == ypos - 1) ||  // Down-Left
        (xpos2 == xpos + 1 && ypos2 == ypos + 1)))   // Down-Right
    {
        return false;
    }

    if (game.accessGameboardAPI()[xpos2][ypos2].has_value())
    {
        return false;
    }

    std::stack<Card> saveStack;
    
    while (!game.accessGameboardAPI()[xpos][ypos]->empty())
    {
        saveStack.push(game.accessGameboardAPI()[xpos][ypos]->top());
        game.accessBoard().removeCard({ xpos,ypos });

        if (!game.accessGameboardAPI()[xpos][ypos].has_value())
        {
            break;
        }
    }

    while (!saveStack.empty())
    {
        game.accessBoard().insertCard({ saveStack.top().getValue(),saveStack.top().getPlayerID() }, { xpos2,ypos2 });
        saveStack.pop();
    }

    int readValue;
    std::cout << "\nEnter value for insert: ";
    std::cin >> readValue;
    uint8_t value = static_cast<uint8_t>(readValue);

    if (!game.accessBoard().validateValue(value))
    {
        std::cout << "value wrong!\n";
        return false;
    }

    if (!game.getCurrentPlayer().removeCard(value))
    {
        std::cout << "value available wrong!\n"; /*if card exists also delete it*/
        return false; /*restart loop*/
    }

    game.accessBoard().insertCard(Card{ value,game.getCurrentPlayerEnum() }, {xpos,ypos});
    game.accessBoard().addPositionToValid({ xpos2,ypos2 });
    game.accessBoard().addPositionToValid({ xpos,ypos});

    markUsed();
    game.getCurrentPlayer().markMagicUsed();
    return true;
}
