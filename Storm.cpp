#include "Storm.h"
#include "Game.h"

Storm::Storm()
{
    m_description = "Storm: All cards covered return to their owners' hands.";
}

void Storm::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool Storm::usePower(Game& game)
{
    std::cout << "Storm power activated: All covered cards return to their owners' hands." << std::endl;

    const auto& gMiddle = game.accessBoard().getGridMiddle();
    const auto& is4x4 = game.accessBoard().getis4x4();
    auto& board = game.accessBoard();

    for (size_t i = gMiddle.first - 1 - 0.5 * is4x4; i <= gMiddle.first + 1 + 0.5 * is4x4; i++)
    {
        for (size_t j = gMiddle.second - 1 - 0.5 * is4x4; j <= gMiddle.second + 1 + 0.5 * is4x4; j++)
        {
            
            auto& stack = board.getBoard()[i][j];
            if (stack.has_value())
            {
                if (stack->size() >= 2)
                {
                    auto cardOnTop = stack->top();
                    game.accessBoard().removeCard({ i,j });

                    while (!stack->empty())
                    {
                        auto cardBelow = stack->top();
                        game.getPlayerBasedOnEnum(cardBelow.getPlayerID()).addCard(cardBelow.getValue());
                        board.removeCard({ i,j });

                        if (!stack.has_value())
                            break;
                    }
                    game.accessBoard().insertCard({ cardOnTop.getValue(),cardOnTop.getPlayerID() }, { i,j });
                    game.accessBoard().addPositionToValid({ i,j });
                }
            }

        }
    }

    game.getCurrentPlayer().markMagicUsed();
    markUsed();
    return true;
}
