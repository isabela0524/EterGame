#include "Lava.h"
#include "Game.h"

Lava::Lava()
{
    m_description = "Lava: Choose a number with at least 2 cards of that number visible on the board. All visible cards with that number return to their owners' hands.";
}

void Lava::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool Lava::usePower(Game& game)
{
    std::cout << "Lava power activated: All visible cards of the chosen number return to their owners' hands." << std::endl;

    int var;
    std::cout << "\nInput the value to return: "; std::cin >> var;
    if (var == 6)
    {
        std::cout << "\nCannot chose Eter card!\n";
        return false;
    }

    if (0 >= var || var >= 5)
    {
        std::cout << "\nValue not available!\n";
        return false;
    }

    uint8_t value = static_cast<size_t>(var);
    var = 0;

    const auto& gMiddle = game.accessBoard().getGridMiddle();
    const auto& is4x4 = game.accessBoard().getis4x4();
    auto& board = game.accessBoard();

    for (size_t i = gMiddle.first - 1 - 0.5 * is4x4; i <= gMiddle.first + 1 + 0.5 * is4x4; i++)
    {
        for (size_t j = gMiddle.second - 1 - 0.5 * is4x4; j <= gMiddle.second + 1 + 0.5 * is4x4; j++)
        {
            if (board.getValueAt({ i,j }) == value) 
                var++;
        }
    }

    if (var < 2)
    {
        std::cout << "\nCondition not fufilled!\n";
        return false;
        /*Return false to punish player or to end the function if condition cannot be met*/
    }

    for (size_t i = gMiddle.first - 1 - 0.5 * is4x4; i <= gMiddle.first + 1 + 0.5 * is4x4; i++)
    {
        for (size_t j = gMiddle.second - 1 - 0.5 * is4x4; j <= gMiddle.second + 1 + 0.5 * is4x4; j++)
        {
            if (board.getValueAt({ i,j }) == value)
            {
                auto& cell = game.accessGameboardAPI()[i][j]->top();
                game.getPlayerBasedOnEnum(cell.getPlayerID()).addCard(cell.getValue());
                board.removeCard({ i,j });
            }
        }
    }

    game.getCurrentPlayer().markMagicUsed();
    markUsed();
    return true;
}
