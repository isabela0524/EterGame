#pragma once
#include <vector>
#include <stack>
#include <optional>
#include <unordered_set>
#include "Card.h"
#include "Gamemode.h"
#include "Bridge.h"
class Board
{
private:
	using Cell = std::optional<std::stack<Card>>;
	using Gamematrix = std::vector<std::vector<Cell>>;
	struct Hash
	{
		std::size_t operator()(const std::pair<int, int>& p) const;
	};
	enum LockCaseEnum
	{
		NoLock = 0U,
		RowLock = 1U,
		ColumnLock = 2U,
		BothLock = 3U,
		GridLock = 6U
	};

	static const size_t                             k_baseMatrixSize{ 5 };
	static const float                              k_baseGridMiddleCoordonate;
	static const std::vector<std::pair<int, int>>   k_baseAdjacentPositions;

	std::reference_wrapper<const Gamemode>			m_gamemode;
	std::reference_wrapper<Bridge>					m_bridge;

	Gamematrix										m_board;
	std::pair<float, float>	                        m_gridMiddle;
	LockCaseEnum                                    m_lockcase;
	std::unordered_set<std::pair<int, int>, Hash>   m_validPositions;
	uint8_t											m_noOfSpacesFilled{ 0 };
	bool											m_hasExploded{ false };


	void											interpretLockcase();
	uint8_t											verifyLockcase() const;

public:
	explicit Board(const std::reference_wrapper<const Gamemode>& gamemode, std::reference_wrapper<Bridge> bridge);
	Gamematrix&										getBoard();

	void											printGameboard() const;
	void											printValidPositions() const;

	void											addPositionToValid(const std::pair<size_t, size_t>& position);
	void											removePositionFromValid(const std::pair<size_t, size_t>& position);
	const std::pair<size_t, size_t>					normalise(const std::pair<size_t, size_t>& position) const;
	void											insertCard(Card&& card, const std::pair<size_t, size_t>& position);
	void											removeCard(const std::pair<size_t, size_t>& position);
	bool											isOwnCardCoveredByEnemy(const std::pair<size_t, size_t>& position);

	bool											validatePositionInGrid(const std::pair<float, float>& newGridMiddle);
	bool											tryGridShiftForInsertPosition(const std::pair<size_t, size_t>& position);

	bool											validateInsertPosition(const std::pair<size_t, size_t>& position) const;
	bool											validateValue(uint8_t value) const;
	bool											validateStackRule(const std::pair<size_t, size_t>& position, uint8_t value) const;

	LockCaseEnum									getMinLockcaseValue() const;
	LockCaseEnum									getLockcase() const;
	void											setLockcase();

	bool											isFirstMove() const;

	bool											checkWinCase2(PlayerEnum currentPlayer) const;

	bool											containsOwnCard(const std::pair<size_t, size_t>& position, PlayerEnum currentPlayer) const;
	const std::pair<float, float>&					getGridMiddle() const;
	bool											getis4x4() const;
	uint8_t											getValueAt(const std::pair<size_t, size_t>& position) const;
	Card&											getCardOnTopAt(const std::pair<size_t, size_t>& position);
	bool											isBoardEmpty() const;
	bool											canHandlePit() const;
	void											revalidateInsertPosition();

	void											resetBoard();
};

