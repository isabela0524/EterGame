#pragma once
#include <deque>
#include <functional>
#include "Gamemode.h"
#include "PlayerEnum.h"

class Bridge
{
private:
    std::reference_wrapper<const Gamemode>          m_gamemode;
    std::deque<std::pair<uint8_t, uint8_t>>         m_columnPocket;
    std::deque<std::pair<uint8_t, uint8_t>>         m_rowPocket;

public:
    explicit Bridge(const std::reference_wrapper<const Gamemode>& gamemode);

    const std::deque<std::pair<uint8_t, uint8_t>>&  getColumnPocket() const;
    const std::deque<std::pair<uint8_t, uint8_t>>&  getRowPocket() const;

    void                                            popColumnBack();
    void                                            popRowBack();
    void                                            popColumnFront();
    void                                            popRowFront();

    void                                            pushColumnBack(const std::pair<size_t, size_t>& position);
    void                                            pushRowBack(const std::pair<size_t, size_t>& position);
    void                                            pushColumnFront(const std::pair<size_t, size_t>& position);
    void                                            pushRowFront(const std::pair<size_t, size_t>& position);

    void                                            incrementColumnPocket(size_t position, PlayerEnum player);
    void                                            incrementRowPocket(size_t position, PlayerEnum player);
    void                                            decrementColumnPocket(size_t position, PlayerEnum player);
    void                                            decrementRowPocket(size_t position, PlayerEnum player);

    uint8_t                                         getColumnPocket(size_t position, PlayerEnum player);
    uint8_t                                         getRowPocket(size_t position, PlayerEnum player);

    void                                            resetBridge();
    void                                            printPockets() const;

    bool                                            verifyColumnExplosionCriteria() const;
    bool                                            verifyRowExplosionCriteria() const;
};
