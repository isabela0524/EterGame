#include "Earthquake.h"
#include "Game.h"

Earthquake::Earthquake()
{
    m_description = "Earthquake: Removes all visible cards with the number 1 from the board.";
}

void Earthquake::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool Earthquake::usePower(Game& game)
{
    std::cout << "Earthquake power activated: Removes all visible cards with the number 1 from the board." << std::endl;

    const auto& gMiddle = game.accessBoard().getGridMiddle();
    const auto& is4x4 = game.accessBoard().getis4x4();
    auto& board = game.accessBoard();

    for (size_t i = gMiddle.first - 1 - 0.5 * is4x4; i <= gMiddle.first + 1 + 0.5 * is4x4; i++)
    {
        for (size_t j = gMiddle.second - 1 - 0.5 * is4x4; j <= gMiddle.second + 1 + 0.5 * is4x4; j++)
        {
            if (game.accessGameboardAPI()[i][j].has_value())
            {
                if (game.accessGameboardAPI()[i][j]->top().getValue() == 1)
                    game.accessBoard().removeCard({ i,j });
            }
        }
    }

    markUsed();
    game.getCurrentPlayer().markMagicUsed();
    return true;
}
