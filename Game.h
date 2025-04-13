#pragma once
#include <array>
#include "Timer.h"
#include "Player.h"
#include "Bridge.h"
#include "Board.h"

class Game
{
private:
    using Cell = std::optional<std::stack<Card>>;
    using Gamematrix = std::vector<std::vector<Cell>>;

    const	PlayerEnum k_baseFirstPlayer{ PlayerEnum::Player1 };
    struct	Input
    {
        std::pair<size_t, size_t> position;
        uint8_t value;
        void readInput();
    };
 

    Input					                            m_input;

    Bridge& m_bridge;
    Board                                               m_board;
    std::array<Player, 2> m_players;

    PlayerEnum				                            m_currentPlayer{ k_baseFirstPlayer };
    bool                                                m_areExplosionsEnabled{ m_players[0].getGamemode().getHasExplosions() };

    Timer                                               m_timerPlayer1{ m_players[0].getGamemode().getTimerSeconds(), [this]() -> PlayerEnum {return onTimerTimeout(); } };
    Timer                                               m_timerPlayer2{ m_players[0].getGamemode().getTimerSeconds(), [this]() -> PlayerEnum {return onTimerTimeout(); } };

    std::reference_wrapper<Timer>                       m_currentTimer{m_timerPlayer1};

    PlayerEnum                                          onTimerTimeout() const;
    const std::shared_ptr<AbstractMage>&                getMage(PlayerEnum currentPlayer) const;
    const std::vector<std::shared_ptr<AbstractMagic>>   getMagicPowers(PlayerEnum currentPlayer) const;

    bool                                                verifySum(PlayerEnum currentPlayer) const;
    bool                                                verifyNullPresence(PlayerEnum currentPlayer) const;
    bool                                                verifyRowOrColumnWin(PlayerEnum currentPlayer) const;
    bool                                                verifyDiagonalWin(PlayerEnum currentPlayer) const;

    bool                                                checkWinCase1(PlayerEnum currentPlayer) const;
    bool                                                checkWinCase2(PlayerEnum currentPlayer) const;
    bool                                                checkWinCase3(PlayerEnum currentPlayer) const;
    PlayerEnum                                          checkWinCase4() const;

    void                                                printLogic() const;
    void                                                switchPlayer();

    std::string_view                                    getPlayerChoice() const;
    void                                                handleChoice(std::string_view choice);

    void                                                returnToPlayer();
    void                                                placeRandomPit();
    void                                                removeCard();

    bool                                                verifyExplosionCriteria() const;

    void                                                useMagicPowers();
    void                                                useMage();
    void                                                handleEterCard(const std::pair<size_t, size_t>& position);

    bool                                                getResponse() const;
    void                                                simulateLastMove();
    void                                                resetGame();

    void                                                startTimer();
    void                                                stopTimer();
    bool                                                timerRanOutOfTime() const;
public:
    explicit Game(Bridge& bridge, const Board& board, const std::array<Player, 2>& players);

    PlayerEnum                                          playGame();

    void                                                placeCard();

    Gamematrix&                                         accessGameboardAPI();
    Board&                                              accessBoard();
    PlayerEnum                                          getCurrentPlayerEnum();
    Player&                                             getCurrentPlayer();
    Player&                                             getCurrentOpponent();
    Player&                                             getPlayerBasedOnEnum(PlayerEnum playerEnum);

    bool                                                validateIllusionCover(const std::pair<size_t, size_t>& position, uint8_t value);
    void                                                useExplosion();
    void                                                placeIllusion();
};