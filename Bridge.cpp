#include "Bridge.h"

Bridge::Bridge(const std::reference_wrapper<const Gamemode>& gamemode)
    : m_gamemode(gamemode),
    m_columnPocket(3 + static_cast<size_t>(m_gamemode.get().getIs4x4()), { 0,0 }),
    m_rowPocket(3 + static_cast<size_t>(m_gamemode.get().getIs4x4()), { 0,0 })
{
}

const std::deque<std::pair<uint8_t, uint8_t>>& Bridge::getColumnPocket() const
{
    return m_columnPocket;
}

const std::deque<std::pair<uint8_t, uint8_t>>& Bridge::getRowPocket() const
{
    return m_rowPocket;
}

void Bridge::popColumnBack()
{
    if (!m_columnPocket.empty())
    {
        m_columnPocket.pop_back();
    }
}

void Bridge::popRowBack()
{
    if (!m_rowPocket.empty())
    {
        m_rowPocket.pop_back();
    }
}

void Bridge::popColumnFront()
{
    if (!m_columnPocket.empty())
    {
        m_columnPocket.pop_front();
    }
}

void Bridge::popRowFront()
{
    if (!m_rowPocket.empty())
    {
        m_rowPocket.pop_front();
    }
}

void Bridge::pushColumnBack(const std::pair<size_t, size_t>& position)
{
    m_columnPocket.push_back(position);
}

void Bridge::pushRowBack(const std::pair<size_t, size_t>& position)
{
    m_rowPocket.push_back(position);
}

void Bridge::pushColumnFront(const std::pair<size_t, size_t>& position)
{
    m_columnPocket.push_front(position);
}

void Bridge::pushRowFront(const std::pair<size_t, size_t>& position)
{
    m_rowPocket.push_front(position);
}

void Bridge::incrementColumnPocket(size_t position, PlayerEnum player)
{
    if (player == PlayerEnum::Player1)
    {
        m_columnPocket[position].first++;
    }
    else
    {
        m_columnPocket[position].second++;
    }
}

void Bridge::incrementRowPocket(size_t position, PlayerEnum player)
{
    if (player == PlayerEnum::Player1)
    {
        m_rowPocket[position].first++;
    }
    else
    {
        m_rowPocket[position].second++;
    }
}

void Bridge::decrementColumnPocket(size_t position, PlayerEnum player)
{
    if (player == PlayerEnum::Player1)
    {
        m_columnPocket[position].first--;
    }
    else
    {
        m_columnPocket[position].second--;
    }
}

void Bridge::decrementRowPocket(size_t position, PlayerEnum player)
{
    if (player == PlayerEnum::Player1)
    {
        m_rowPocket[position].first--;
    }
    else
    {
        m_rowPocket[position].second--;
    }
}

uint8_t Bridge::getColumnPocket(size_t position, PlayerEnum player)
{
    if (player == PlayerEnum::Player1)
    {
        return m_columnPocket[position].first;
    }
    else
    {
        return m_columnPocket[position].second;
    }
}

uint8_t Bridge::getRowPocket(size_t position, PlayerEnum player)
{
    if (player == PlayerEnum::Player1)
    {
        return m_rowPocket[position].first;
    }
    else
    {
        return m_rowPocket[position].second;
    }
}

void Bridge::resetBridge()
{
    size_t pocketSize = 3 + static_cast<size_t>(m_gamemode.get().getIs4x4());

    m_columnPocket.clear();
    for (size_t i = 0; i < pocketSize; ++i)
    {
        m_columnPocket.push_back({ 0, 0 });
    }

    m_rowPocket.clear();
    for (size_t i = 0; i < pocketSize; ++i)
    {
        m_rowPocket.push_back({ 0, 0 });
    }
}

void Bridge::printPockets() const
{
    // Print column pocket
    std::cout << "Column Pocket: ";
    for (const auto& pair : m_columnPocket) {
        std::cout << "(" << static_cast<int>(pair.first) << ", " << static_cast<int>(pair.second) << ") ";
    }
    std::cout << std::endl;

    // Print row pocket
    std::cout << "Row Pocket: ";
    for (const auto& pair : m_rowPocket) {
        std::cout << "(" << static_cast<int>(pair.first) << ", " << static_cast<int>(pair.second) << ") ";
    }
    std::cout << std::endl;
}

bool Bridge::verifyColumnExplosionCriteria() const
{
    bool is4x4 = m_gamemode.get().getIs4x4();
    int requiredSum = is4x4 ? 4 : 3;

    int count = 0;

    for (const auto& pair : m_columnPocket)
    {
        if (pair.first + pair.second == requiredSum)
        {
            count++;
            if (count >= 2)
            {
                return true;
            }
        }
    }

    return false;
}

bool Bridge::verifyRowExplosionCriteria() const
{
    bool is4x4 = m_gamemode.get().getIs4x4();
    int requiredSum = is4x4 ? 4 : 3;

    int count = 0;

    for (const auto& pair : m_rowPocket)
    {
        if (pair.first + pair.second == requiredSum)
        {
            std::cout << "\n\n\nRow pair: " << pair.first + pair.second << '\n';
            count++;
            if (count >= 2)
            {
                return true;
            }
        }
    }
    std::cout << "\n\n\n\nRow: " << count << "\n";

    return false;
}
