#pragma once
#include <array>
#include "Gamemode.h"
#include "Bridge.h"
#include "Player.h"
#include "Board.h"
#include "Game.h"

class Match
{
private:
    const Gamemode                          m_gamemode;
    Bridge                                  m_bridge;

    Board                                   m_board;
    std::array<Player, 2>                   m_players;

    uint8_t                                 m_matchNumber{ 1 };

    std::reference_wrapper<const Gamemode>  getGamemode() const;
    bool                                    matchWon(PlayerEnum winner);
    void                                    reset();
    PlayerEnum                              startGame();
public:
    explicit Match(Gamemode gamemode);

    void                                    startMatch();
};