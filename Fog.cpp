#include "Fog.h"
#include "Game.h"

Fog::Fog()
{
    m_description = "Fog: Play another illusion.";
}

void Fog::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool Fog::usePower(Game& game)
{
    std::cout << "Fog power activated: Play another illusion." << std::endl;

    const auto& gMiddle = game.accessBoard().getGridMiddle();
    const auto& is4x4 = game.accessBoard().getis4x4();
    auto& board = game.accessBoard();


    if (game.getCurrentPlayer().hasUsedIllusion())
    {
        for (size_t i = gMiddle.first - 1 - 0.5 * is4x4; i <= gMiddle.first + 1 + 0.5 * is4x4; i++)
        {
            for (size_t j = gMiddle.second - 1 - 0.5 * is4x4; j <= gMiddle.second + 1 + 0.5 * is4x4; j++)
            {
                auto& cell = board.getBoard()[i][j];
                if (cell)
                {
                    if (cell->top().isIllusion())
                    {
                        return false;
                    }
                }
            }
        }

        markUsed();
        game.getCurrentPlayer().markMagicUsed();
        game.placeIllusion();

    }


    return true;
}
