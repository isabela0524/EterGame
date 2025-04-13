#include "Match.h"

Match::Match(Gamemode gamemode)
    : m_gamemode(std::move(gamemode)),
    m_bridge(Bridge(m_gamemode)),
    m_board(m_gamemode, m_bridge),
    m_players{ { Player{m_gamemode, m_bridge},
                Player{m_gamemode, m_bridge} } }
{
}

PlayerEnum Match::startGame()
{
    Game ownGame(m_bridge, m_board, m_players);
    auto winner = ownGame.playGame();

    m_players[static_cast<size_t>(winner)].incrementGamesWon();
    m_matchNumber++;
    return winner;
}

void Match::startMatch()
{
    auto winner = PlayerEnum::Player1;

    do
    {
        if (m_matchNumber > 1)
        {
            reset();
        }

        std::cout << "\n\nSTARTING GAME NUMBER " << static_cast<int>(m_matchNumber) << "!\n\n";
        winner = startGame();
    } while (!matchWon(winner));

}

std::reference_wrapper<const Gamemode> Match::getGamemode() const
{
    return std::cref(m_gamemode);
}

bool Match::matchWon(PlayerEnum winner)
{
    if ((float)m_players[static_cast<size_t>(winner)].getNumberOfGamesWon() / m_gamemode.getMaxNumberOfGames() > 0.5)
    {
        std::cout << "\n\n\nMatch Won By Player " << static_cast<int>(winner) + 1 << "\n\n\n";
        return true;
    }
    return false;
}

void Match::reset()
{
    m_bridge.resetBridge();
    m_players[static_cast<size_t>(PlayerEnum::Player1)].initialisePlayerCards();
    m_players[static_cast<size_t>(PlayerEnum::Player1)].resetMagic();

    m_players[static_cast<size_t>(PlayerEnum::Player2)].initialisePlayerCards();
    m_players[static_cast<size_t>(PlayerEnum::Player2)].resetMagic();
}
