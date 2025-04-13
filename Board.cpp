#include "Board.h"
#include <numeric>
#include <algorithm>

Board::Board(const std::reference_wrapper<const Gamemode>& gamemode, std::reference_wrapper<Bridge> bridge)
    :m_gamemode{ gamemode },
    m_bridge{ bridge },
    m_board(k_baseMatrixSize + 2 * m_gamemode.get().getIs4x4(), std::vector<Cell>(k_baseMatrixSize + 2 * m_gamemode.get().getIs4x4())),
    m_gridMiddle{ k_baseGridMiddleCoordonate + 0.5 * gamemode.get().getIs4x4(),k_baseGridMiddleCoordonate + 0.5 * gamemode.get().getIs4x4() },
    m_lockcase{ LockCaseEnum::NoLock }
{
    m_validPositions.insert({ k_baseGridMiddleCoordonate + gamemode.get().getIs4x4(), k_baseGridMiddleCoordonate + gamemode.get().getIs4x4() });
}

Board::Gamematrix& Board::getBoard()
{
    return m_board;
}

std::size_t Board::Hash::operator()(const std::pair<int, int>& p) const
{
    std::size_t h1 = std::hash<int>{}(p.first);
    std::size_t h2 = std::hash<int>{}(p.second);
    return h1 ^ (h2 << 1);
}

const float Board::k_baseGridMiddleCoordonate = 2.0f;
const std::vector<std::pair<int, int>> Board::k_baseAdjacentPositions = { {-1, -1}, {-1, 0}, {-1, 1},
                                                                          { 0, -1}, { 0, 0}, { 0, 1},
                                                                          { 1, -1}, { 1, 0}, { 1, 1} };

//void Board::printGameboard() const
//{
//    //std::cout << "\nThe grid middle is set in: " << m_gridMiddle.first << " " << m_gridMiddle.second << '\n';
//    /*std::cout << "\n";
//    for (const auto& row : m_board)
//    {
//        for (const auto& cell : row)
//        {
//            if (cell.has_value() && !cell->empty())
//            {
//                const Card& card = cell->top();
//                std::cout << std::format("({},{}) ", card.getValue(), static_cast<int>(card.getPlayerID()) + 1);
//            }
//            else
//            {
//                std::cout << "( , ) ";
//            }
//        }
//        std::cout << std::endl;
//    }*/
//
//   /* bool is4x4 = m_gamemode.get().getIs4x4();
//
//    std::cout << "\n";
//    for (int i = m_gridMiddle.first - 1 - is4x4 * 0.5; i <= m_gridMiddle.first + 1 + is4x4 * 0.5; i++)
//    {
//        for (int j = m_gridMiddle.second - 1 - is4x4 * 0.5; j <= m_gridMiddle.second + 1 + is4x4 * 0.5; j++)
//        {
//            if (i >= 0 && i < m_board.size() && j >= 0 && j < m_board[i].size())
//            {
//                const auto& cell = m_board[i][j];
//                if (cell.has_value() && !cell->empty())
//                {
//                    const Card& card = cell->top();
//                    std::cout << std::format("({},{}) ", card.getValue(), static_cast<int>(card.getPlayerID()) + 1);
//                }
//                else
//                {
//                    std::cout << "( , ) ";
//                }
//            }
//            else
//            {
//                std::cout << "( , ) ";
//            }
//        }
//        std::cout << std::endl;
//    }*/
//
//    std::cout << "\n";
//    for (const auto& row : m_board)
//    {
//        for (const auto& cell : row)
//        {
//            if (cell.has_value() && !cell->empty())
//            {
//                const Card& card = cell->top();
//                std::cout << std::format("({},{}) ", card.getValue(), static_cast<int>(card.getPlayerID()) + 1);
//            }
//            else
//            {
//                std::cout << "( , ) ";
//            }
//        }
//        std::cout << std::endl;
//    }
//
//
//}

void Board::printGameboard() const
{
    std::cout << "\n";
    for (size_t row_index = 0; row_index < m_board.size(); ++row_index)
    {
        for (size_t col_index = 0; col_index < m_board[row_index].size(); ++col_index)
        {
            const auto& cell = m_board[row_index][col_index];
            if (validateInsertPosition({ row_index, col_index }))
            {
                if (cell.has_value() && !cell->empty())
                {
                    const Card& card = cell->top();
                    std::string value_display;

                    if (card.isIllusion())
                    {
                        value_display = "?";
                    }
                    else if (card.getValue() == 6)
                    {
                        value_display = "E";
                    }
                    else if (card.getValue() == 10)
                    {
                        value_display = "X";
                    }
                    else
                    {
                        value_display = std::to_string(card.getValue());
                    }

                    std::cout << std::format("({},{}) ", value_display, static_cast<int>(card.getPlayerID()) + 1);
                }
                else
                {
                    std::cout << "( , ) ";
                }
            }
            else
            {
                std::cout << "      ";
            }
        }
        std::cout << std::endl;
    }
}

void Board::printValidPositions() const
{
    std::cout << "\n";
    for (const auto& position : m_validPositions)
    {
        std::cout << std::format("({},{}) ", position.first, position.second);
    }
}

void Board::addPositionToValid(const std::pair<size_t, size_t>& position)
{
    size_t max_valid_index = 5 + 2 * m_gamemode.get().getIs4x4();

    for (const auto& offset : k_baseAdjacentPositions)
    {
        size_t new_i = position.first + offset.first;
        size_t new_j = position.second + offset.second;

        if (new_i >= 0 && new_i <= max_valid_index && new_j >= 0 && new_j <= max_valid_index)
        {
            m_validPositions.insert({ new_i, new_j });
        }
    }
}


void Board::removePositionFromValid(const std::pair<size_t, size_t>& position)
{
    if (m_validPositions.find(position) != m_validPositions.end())
    {
        m_validPositions.erase(position);
    }
}

const std::pair<size_t, size_t> Board::normalise(const std::pair<size_t, size_t>& position) const
{
    return std::pair<size_t, size_t>(position.first - (static_cast<size_t>(m_gridMiddle.first - 1)),
        position.second - (static_cast<size_t>(m_gridMiddle.second - 1))
    );
}

void Board::insertCard(Card&& card, const std::pair<size_t, size_t>& position)
{
    auto normalisedCoords = normalise(position);
    auto& bridge = m_bridge.get();

    if (!m_board[position.first][position.second].has_value())
    {
        m_board[position.first][position.second] = std::stack<Card>();
        m_noOfSpacesFilled++;
    }
    else
    {
        //if there is a card on position, decrement said position from said player, even if it's own card
        auto& currentTopCard = m_board[position.first][position.second]->top();
        bridge.decrementColumnPocket(normalisedCoords.first, currentTopCard.getPlayerID());
        bridge.decrementRowPocket(normalisedCoords.second, currentTopCard.getPlayerID());
    }

    bridge.incrementColumnPocket(normalisedCoords.first, card.getPlayerID());
    bridge.incrementRowPocket(normalisedCoords.second, card.getPlayerID());

    m_board[position.first][position.second]->push(std::move(card));
    addPositionToValid(position);
}

void Board::removeCard(const std::pair<size_t, size_t>& position)
{
    auto normalisedCoords = normalise(position);
    auto& bridge = m_bridge.get();

    if (!m_board[position.first][position.second].has_value())
    {
        std::cout << "\nPosition is empty!";
        return;
    }

    auto& currentTopCard = m_board[position.first][position.second]->top();
    bridge.decrementColumnPocket(normalisedCoords.first, currentTopCard.getPlayerID());
    bridge.decrementRowPocket(normalisedCoords.second, currentTopCard.getPlayerID());

    m_board[position.first][position.second]->pop();

    if (m_board[position.first][position.second]->empty())
    {
        m_board[position.first][position.second].reset();
        m_noOfSpacesFilled--;
    }
    else
    {
        auto& newTopCard = m_board[position.first][position.second]->top();
        bridge.incrementColumnPocket(normalisedCoords.first, newTopCard.getPlayerID());
        bridge.incrementRowPocket(normalisedCoords.second, newTopCard.getPlayerID());
    }

    if (m_noOfSpacesFilled == 0)
    {
        m_gridMiddle = { k_baseGridMiddleCoordonate + 0.5 * m_gamemode.get().getIs4x4(),
                         k_baseGridMiddleCoordonate + 0.5 * m_gamemode.get().getIs4x4() };
        m_validPositions.clear();
        m_validPositions.insert({ k_baseGridMiddleCoordonate + m_gamemode.get().getIs4x4(), k_baseGridMiddleCoordonate + m_gamemode.get().getIs4x4() });

        return;
    }
}

bool Board::isOwnCardCoveredByEnemy(const std::pair<size_t, size_t>& position)
{
    if (!m_board[position.first][position.second].has_value())
    {
        return false;
    }

    auto& stack = m_board[position.first][position.second].value();

    if (stack.size() == 1)
    {
        return false;
    }

    auto currentTopCard = stack.top();
    stack.pop();
    auto isCoveredByEnemy = stack.top().getPlayerID() != currentTopCard.getPlayerID();
    stack.push(currentTopCard);

    return isCoveredByEnemy;
}


bool Board::validatePositionInGrid(const std::pair<float, float>& newGridMiddle)
{
    auto& bridge = m_bridge.get();
    if (m_gridMiddle.first > newGridMiddle.first) // UP
    {
        size_t lastIndex{ bridge.getColumnPocket().size() - 1 };
        if (!(bridge.getColumnPocket(lastIndex, PlayerEnum::Player1) == 0 &&
            bridge.getColumnPocket(lastIndex, PlayerEnum::Player2) == 0))
        {
            return false;
        }
        bridge.popColumnBack();
        bridge.pushColumnFront({ 0, 0 });
    }

    if (m_gridMiddle.first < newGridMiddle.first) // DOWN
    {
        size_t firstIndex{ 0 };
        if (!(bridge.getColumnPocket(firstIndex, PlayerEnum::Player1) == 0 &&
            bridge.getColumnPocket(firstIndex, PlayerEnum::Player2) == 0))
        {
            return false;
        }
        bridge.popColumnFront();
        bridge.pushColumnBack({ 0, 0 });
    }

    if (m_gridMiddle.second > newGridMiddle.second) // LEFT
    {
        size_t lastIndex{ bridge.getRowPocket().size() - 1 };
        if (!(bridge.getRowPocket(lastIndex, PlayerEnum::Player1) == 0 &&
            bridge.getRowPocket(lastIndex, PlayerEnum::Player2) == 0))
        {
            return false;
        }
        bridge.popRowBack();
        bridge.pushRowFront({ 0, 0 });
    }

    if (m_gridMiddle.second < newGridMiddle.second) // RIGHT
    {
        size_t firstIndex{ 0 };
        if (!(bridge.getRowPocket(firstIndex, PlayerEnum::Player1) == 0 &&
            bridge.getRowPocket(firstIndex, PlayerEnum::Player2) == 0))
        {
            return false;
        }
        bridge.popRowFront();
        bridge.pushRowBack({ 0, 0 });
    }

    return true;
}

bool Board::tryGridShiftForInsertPosition(const std::pair<size_t, size_t>& position)
{
    std::pair<float, float> newGridMiddle{ m_gridMiddle };
    newGridMiddle.first += static_cast<int>((position.first - m_gridMiddle.first) / (2.f + m_gamemode.get().getIs4x4() * 0.5));
    newGridMiddle.second += static_cast<int>((position.second - m_gridMiddle.second) / (2.f + m_gamemode.get().getIs4x4() * 0.5));

    if (validatePositionInGrid(newGridMiddle))
    {
        m_gridMiddle = newGridMiddle;
        std::cout << "new grid middle in: " << m_gridMiddle.first << " " << m_gridMiddle.second << '\n';
        return true;
    }

    return false;
}

bool Board::validateInsertPosition(const std::pair<size_t, size_t>& position) const
{
    return m_validPositions.find(position) != m_validPositions.end();
}

bool Board::validateValue(uint8_t value) const
{
    return value > 0 && value <= 6;
}

bool Board::validateStackRule(const std::pair<size_t, size_t>& position, uint8_t value) const
{
    if (m_board[position.first][position.second].has_value())
    {
        if (m_board[position.first][position.second]->empty() || value > m_board[position.first][position.second]->top().getValue())
        {
            return true;
        }
        return false;
    }
    return true;
}

void Board::interpretLockcase()
{
    auto& gamemode = m_gamemode.get();
    switch (m_lockcase)
    {
    case LockCaseEnum::NoLock:
    {
        std::cout << "\nNo Lock!\n";
        break;
    }
    case LockCaseEnum::RowLock:
    {
        std::cout << "\nEliminating invalid ROW positions.\n";
        std::vector<std::pair<int, int>> positionsToRemove;

        for (const auto& position : m_validPositions)
        {
            if (position.second == m_gridMiddle.second - (2 + 0.5 * gamemode.getIs4x4()) ||
                position.second == m_gridMiddle.second + (2 + 0.5 * gamemode.getIs4x4()))
            {
                positionsToRemove.push_back(position);
            }
        }

        for (const auto& position : positionsToRemove)
        {
            m_validPositions.erase(position);
        }

        break;
    }
    case LockCaseEnum::ColumnLock:
    {
        std::cout << "\nEliminating invalid COLUMN positions.\n";
        std::vector<std::pair<int, int>> positionsToRemove;

        for (const auto& position : m_validPositions)
        {
            if (position.first == m_gridMiddle.first - (2 + 0.5 * gamemode.getIs4x4()) ||
                position.first == m_gridMiddle.first + (2 + 0.5 * gamemode.getIs4x4()))
            {
                positionsToRemove.push_back(position);
            }
        }

        for (const auto& position : positionsToRemove)
        {
            m_validPositions.erase(position);
        }
        break;
    }
    case LockCaseEnum::BothLock:
    case LockCaseEnum::GridLock:
    {
        std::cout << "\nEliminating all positions OUTSIDE of the grid.\n";
        std::vector<std::pair<int, int>> positionsToRemove;

        for (const auto& position : m_validPositions)
        {
            if (position.second == m_gridMiddle.second - (2 + 0.5 * gamemode.getIs4x4()) ||
                position.second == m_gridMiddle.second + (2 + 0.5 * gamemode.getIs4x4()))
            {
                positionsToRemove.push_back(position);
            }

            if (position.first == m_gridMiddle.first - (2 + 0.5 * gamemode.getIs4x4()) ||
                position.first == m_gridMiddle.first + (2 + 0.5 * gamemode.getIs4x4()))
            {
                positionsToRemove.push_back(position);
            }
        }

        for (const auto& position : positionsToRemove)
        {
            m_validPositions.erase(position);
        }

        break;
    }
    default:
        throw std::runtime_error("Unexpected Lockcase!");
        break;
    }
}

uint8_t Board::verifyLockcase() const
{
    uint8_t conditions = 0;
    auto& bridge = m_bridge.get();

    // Condition 1: Check if no row pocket has a total value of 0 (no players in both Player1 and Player2 positions).
    bool rowCondition = std::none_of(bridge.getRowPocket().begin(), bridge.getRowPocket().end(), [](const std::pair<uint8_t, uint8_t>& p)
        {
            return p.first + p.second == 0;
        });

    if (rowCondition)
    {
        conditions += 1;
    }

    // Condition 2: Check if no column pocket has a total value of 0 (no players in both Player1 and Player2 positions).
    bool columnCondition = std::none_of(bridge.getColumnPocket().begin(), bridge.getColumnPocket().end(), [](const std::pair<uint8_t, uint8_t>& p)
        {
            return p.first + p.second == 0;
        });

    if (columnCondition)
    {
        conditions += 2;
    }

    // Condition 3: Check if there is any row or column pocket where Player1 and Player2 have total value of 0 (both empty).
    bool combinedCondition = std::none_of(bridge.getRowPocket().begin(), bridge.getRowPocket().end(), [](const std::pair<uint8_t, uint8_t>& p)
        {
            return p.first + p.second == 0;
        }) &&
        std::none_of(bridge.getColumnPocket().begin(), bridge.getColumnPocket().end(), [](const std::pair<uint8_t, uint8_t>& p)
            {
                return p.first + p.second == 0;
            });

        if (combinedCondition)
        {
            conditions += 3;
        }

        return conditions;
}

Board::LockCaseEnum Board::getMinLockcaseValue() const
{
    return LockCaseEnum::BothLock;
}

Board::LockCaseEnum Board::getLockcase() const
{
    return m_lockcase;
}

void Board::setLockcase()
{
    uint8_t lockcaseUint = verifyLockcase();
    switch (lockcaseUint)
    {
    case 0u:
        m_lockcase = LockCaseEnum::NoLock;
        break;
    case 1u:
        m_lockcase = LockCaseEnum::RowLock;
        break;
    case 2u:
        m_lockcase = LockCaseEnum::ColumnLock;
        break;
    case 3u:
    case 6u:
        m_lockcase = LockCaseEnum::GridLock;
        break;
    default:
        throw std::runtime_error("Unexpected Lockcase value!");
        break;
    }
    interpretLockcase();
}

bool Board::isFirstMove() const
{
    return m_validPositions.size() == 1;
}

bool Board::checkWinCase2(PlayerEnum currentPlayer) const
{
    return m_noOfSpacesFilled == 9 + 7 * m_gamemode.get().getIs4x4();
}

bool Board::containsOwnCard(const std::pair<size_t, size_t>& position, PlayerEnum currentPlayer) const
{
    if (!m_board[position.first][position.second].has_value())
    {
        return false;
    }

    return m_board[position.first][position.second]->top().getPlayerID() == currentPlayer;
}

const std::pair<float, float>& Board::getGridMiddle() const
{
    return m_gridMiddle;
}

bool Board::getis4x4() const
{
    return m_gamemode.get().getIs4x4();
}

uint8_t Board::getValueAt(const std::pair<size_t, size_t>& position) const
{
    if (m_board[position.first][position.second].has_value())
        return m_board[position.first][position.second]->top().getValue();
    return 255;
}

Card& Board::getCardOnTopAt(const std::pair<size_t, size_t>& position)
{
    if (m_board[position.first][position.second].has_value())
        return m_board[position.first][position.second]->top();
    throw "Unexpected error on getCardOnTopAt";
}

bool Board::isBoardEmpty() const
{
    return m_noOfSpacesFilled == 0;
}

bool Board::canHandlePit() const
{
    return m_validPositions.size() != 1;
}

void Board::revalidateInsertPosition()
{
    m_validPositions.clear();
    for (size_t i = m_gridMiddle.first - 1 - 0.5 * m_gamemode.get().getIs4x4(); i <= m_gridMiddle.first + 1 + 0.5 * m_gamemode.get().getIs4x4(); i++)
    {
        for (size_t j = m_gridMiddle.second - 1 - 0.5 * m_gamemode.get().getIs4x4(); j <= m_gridMiddle.second + 1 + 0.5 * m_gamemode.get().getIs4x4(); j++)
        {
            if (m_board[i][j].has_value())
            {
                addPositionToValid({ i,j });
            }
        }
    }
    if (m_validPositions.size() == 0)
    {
        m_validPositions.insert({ k_baseGridMiddleCoordonate + m_gamemode.get().getIs4x4(), k_baseGridMiddleCoordonate + m_gamemode.get().getIs4x4() });
    }
}

void Board::resetBoard()
{
    for (auto position : m_validPositions)
    {
        m_board[position.first][position.second].reset();
    }
    m_bridge.get().resetBridge();
    m_validPositions.clear();
    m_validPositions.insert({ k_baseGridMiddleCoordonate + m_gamemode.get().getIs4x4(), k_baseGridMiddleCoordonate + m_gamemode.get().getIs4x4() });
}