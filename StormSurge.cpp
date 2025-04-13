#include "StormSurge.h"
#include "Game.h"

StormSurge::StormSurge()
{
    m_description = "StormSurge: Removes the stacks of cards containing 2 or more cards from the game.";
}

void StormSurge::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool StormSurge::usePower(Game& game)
{
    std::cout << "StormSurge power activated: Removes the stacks of cards containing 2 or more cards from the game." << std::endl;


    const auto& gMiddle = game.accessBoard().getGridMiddle();
    const auto& is4x4 = game.accessBoard().getis4x4();
    auto& board = game.accessBoard();

    for (size_t i = gMiddle.first - 1 - 0.5 * is4x4; i <= gMiddle.first + 1 + 0.5 * is4x4; i++)
    {
        for (size_t j = gMiddle.second - 1 - 0.5 * is4x4; j <= gMiddle.second + 1 + 0.5 * is4x4; j++)
        {
            if (!board.validateInsertPosition({ i,j }))
            {
                continue;
            }

            if (!board.getBoard()[i][j].has_value())
            {
                continue;
            }

            auto& stack = board.getBoard()[i][j].value();

            if (stack.size() >= 2)
            {
                while (!stack.empty())
                {
                    board.removeCard({ i,j });
                }
            }
        }
    }

    markUsed();
    game.getCurrentPlayer().markMagicUsed();
    return true;
}
