#include "Flames.h"
#include "Game.h"

Flames::Flames()
{
    m_description = "Flames: Turns the opponent's illusion face up. Then play a card on any position on the board.";
}

void Flames::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool Flames::usePower(Game& game)
{
    std::cout << "Flames power activated: Turns the opponent's illusion face up and play a card." << std::endl;
    if (!game.getCurrentOpponent().hasUsedIllusion())
    {
        std::cout << "\nThe opponent has not used an illusion!\n";
        return false;
    }
    const auto& gMiddle = game.accessBoard().getGridMiddle();
    const auto& is4x4 = game.accessBoard().getis4x4();
    auto& board = game.accessBoard();

    for (size_t i = gMiddle.first - 1 - 0.5 * is4x4; i <= gMiddle.first + 1 + 0.5 * is4x4; i++)
    {
        for (size_t j = gMiddle.second - 1 - 0.5 * is4x4; j <= gMiddle.second + 1 + 0.5 * is4x4; j++)
        {
            if (board.getValueAt({ i,j }) != 0 && board.getCardOnTopAt({i,j}).isIllusion())
            {
                board.getCardOnTopAt({ i,j }).discoverIllusion();

                if (board.getCardOnTopAt({ i,j }).isIllusion())
                {
                    std::cout << "hehehrere";
                }


                markUsed();
                game.getCurrentPlayer().markMagicUsed();
                game.placeCard();
                return true;
            }
        }
    }
    

    return false;
}
