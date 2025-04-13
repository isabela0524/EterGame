#include "Game.h"
#include <numeric>
#include <algorithm>
#include <random>
#include <functional>


Game::Game(Bridge& bridge, const Board& board,const std::array<Player, 2>& players)
	:m_bridge{bridge},
	m_board{board},
	m_players{players}
{
}

void Game::Input::readInput()
{
	int value;
	std::cout << "\nEnter xPos yPos Value for insert: ";
	std::cin >> this->position.first >> this->position.second >> value;
	this->value = static_cast<uint8_t>(value);
}

bool Game::validateIllusionCover(const std::pair<size_t, size_t>& position, uint8_t value)
{
	auto& illusionCard = m_board.getBoard()[position.first][position.second]->top();

	illusionCard.discoverIllusion();

	if (value <= illusionCard.getValue())
	{
		std::cout << "\nIllusion cannot be covered!";
		return false;
	}

	return true;
}

bool Game::getResponse() const
{
	char input;
	std::cin >> input;

	return (std::tolower(input) == 'y');
}

void Game::simulateLastMove()
{
	switchPlayer();
	placeCard();
}

void Game::resetGame()
{
	m_players[0].initialisePlayerCards();
	m_players[1].initialisePlayerCards();
	m_board.resetBoard();
	m_timerPlayer1.resetTimer();
	m_timerPlayer2.resetTimer();
}

void Game::startTimer()
{
	m_currentTimer.get().start();
}

void Game::stopTimer()
{
	m_currentTimer.get().stop();
}

bool Game::timerRanOutOfTime() const
{
	return !m_currentTimer.get().hasTimeLeft() && m_currentTimer.get().isTimer();
}

void Game::printLogic() const
{
	std::cout << "\n> Currently playing as Player " << static_cast<int>(m_currentPlayer) + 1;
	m_board.printGameboard();
	std::cout << "\n> Valid positions for insert: ";
	m_board.printValidPositions();
	std::cout << "\n> Current cards held by player: ";
	m_players[static_cast<size_t>(m_currentPlayer)].printPlayerCards();

	auto mage = getMage(m_currentPlayer);
	if (mage != nullptr)
	{
		std::cout << "\n> Mage: \n";
		mage.get()->getDescription();
	}

	auto magicPowers = getMagicPowers(m_currentPlayer);
	if (!magicPowers.empty() && !m_players[static_cast<size_t>(m_currentPlayer)].hasUsedMagic())
	{
		std::cout << "\n> Magic Powers: \n";
		for (const auto& magicPower : magicPowers)
		{
			magicPower.get()->getDescription();
			std::cout << "\n";
		}
	}
}

Player& Game::getCurrentPlayer()
{
	return m_players[static_cast<size_t>(m_currentPlayer)];
}

Player& Game::getCurrentOpponent()
{
	auto currentOpponent = (m_currentPlayer == PlayerEnum::Player1) ? PlayerEnum::Player2 : PlayerEnum::Player1;
	return m_players[static_cast<size_t>(currentOpponent)];
}

Player& Game::getPlayerBasedOnEnum(PlayerEnum playerEnum)
{
	return m_players[static_cast<size_t>(playerEnum)];
}

bool Game::checkWinCase1(PlayerEnum currentPlayer) const
{
	if (verifySum(currentPlayer) && verifyNullPresence(currentPlayer))
	{
		if (verifyRowOrColumnWin(currentPlayer) || verifyDiagonalWin(currentPlayer))
		{
			return true;
		}
		return false;
	}
	return false;
}

bool Game::checkWinCase2(PlayerEnum currentPlayer) const
{
	return m_board.checkWinCase2(currentPlayer);
}

bool Game::checkWinCase3(PlayerEnum currentPlayer) const
{
	return m_players[static_cast<size_t>(m_currentPlayer)].hasUsedAllCards();
}

PlayerEnum Game::checkWinCase4() const
{
	uint8_t sum{ 0u };
	static const uint8_t baseValue{ 5u };//this will make it so that pit's are ignored and eter card has value 1
	auto gm{ m_board.getGridMiddle() };
	auto is4x4{ m_players[0].getGamemode().getIs4x4() };

	for (size_t i = gm.first - 1 - 0.5*is4x4 * is4x4; i <= gm.first + 1 + 0.5*is4x4 * is4x4; i++)
	{
		for (size_t j = gm.second - 1 - 0.5 *is4x4; j <= gm.second + 1 + 0.5* is4x4; j++)
		{
			sum += m_board.getValueAt({ i,j });
		}
	}


	return sum%baseValue > 0u ? PlayerEnum::Player1 : PlayerEnum::Player2;
}

PlayerEnum Game::onTimerTimeout() const
{
	if (m_currentPlayer == PlayerEnum::Player1)
		return PlayerEnum::Player2;
	else
		return PlayerEnum::Player1;
}

const std::shared_ptr<AbstractMage>& Game::getMage(PlayerEnum currentPlayer) const
{
	if (!m_players[static_cast<size_t>(currentPlayer)].hasUsedMage())
	{
		size_t playerIndex = (currentPlayer == PlayerEnum::Player1) ? 0 : 1;

		const Gamemode& gamemode = m_players[playerIndex].getGamemode();

		return gamemode.getMages().at(playerIndex);
	}

	return nullptr;
}

const std::vector<std::shared_ptr<AbstractMagic>> Game::getMagicPowers(PlayerEnum currentPlayer) const
{
	std::vector<std::shared_ptr<AbstractMagic>> unusedMagicPowers;
	if (!m_players[static_cast<size_t>(currentPlayer)].hasUsedMagic())
	{
		size_t playerIndex = (currentPlayer == PlayerEnum::Player1) ? 0 : 1;

		const Gamemode& gamemode = m_players[playerIndex].getGamemode();

		for (const auto& magicPower : gamemode.getMagicPowers())
		{
			if (!magicPower->hasBeenUsed())
			{
				unusedMagicPowers.push_back(magicPower);
			}
		}

		return unusedMagicPowers;
	}

	return unusedMagicPowers;
}

bool Game::verifySum(PlayerEnum currentPlayer) const
{
	auto& gamemode = m_players[static_cast<size_t>(m_currentPlayer)].getGamemode();
	uint8_t columnSum = std::accumulate(m_bridge.getColumnPocket().begin(), m_bridge.getColumnPocket().end(), 0,
		[currentPlayer](uint8_t sum, const std::pair<uint8_t, uint8_t>& pocket) {
			return sum + (currentPlayer == PlayerEnum::Player1 ? pocket.first : pocket.second);
		});

	return columnSum >= 3 + gamemode.getIs4x4();
}

bool Game::verifyNullPresence(PlayerEnum currentPlayer) const
{
	bool columnHasNoZeros = !std::any_of(m_bridge.getColumnPocket().begin(), m_bridge.getColumnPocket().end(),
		[currentPlayer](const std::pair<uint8_t, uint8_t>& pocket) {
			return (currentPlayer == PlayerEnum::Player1 ? pocket.first : pocket.second) == 0;
		});

	bool rowHasNoZeros = !std::any_of(m_bridge.getRowPocket().begin(), m_bridge.getRowPocket().end(),
		[currentPlayer](const std::pair<uint8_t, uint8_t>& pocket) {
			return (currentPlayer == PlayerEnum::Player1 ? pocket.first : pocket.second) == 0;
		});

	return columnHasNoZeros || rowHasNoZeros;
}

bool Game::verifyRowOrColumnWin(PlayerEnum currentPlayer) const
{
	auto& gamemode = m_players[static_cast<size_t>(m_currentPlayer)].getGamemode();
	uint8_t targetValue = 3 + gamemode.getIs4x4();

	bool columnWin = std::any_of(m_bridge.getColumnPocket().begin(), m_bridge.getColumnPocket().end(),
		[targetValue, currentPlayer](const std::pair<uint8_t, uint8_t>& pocket) {
			return (currentPlayer == PlayerEnum::Player1 ? pocket.first : pocket.second) == targetValue;
		});

	bool rowWin = std::any_of(m_bridge.getRowPocket().begin(), m_bridge.getRowPocket().end(),
		[targetValue, currentPlayer](const std::pair<uint8_t, uint8_t>& pocket) {
			return (currentPlayer == PlayerEnum::Player1 ? pocket.first : pocket.second) == targetValue;
		});

	return columnWin || rowWin;
}

bool Game::verifyDiagonalWin(PlayerEnum currentPlayer) const
{
	return std::equal(m_bridge.getColumnPocket().begin(), m_bridge.getColumnPocket().end(), m_bridge.getRowPocket().begin(),
		[currentPlayer](const std::pair<uint8_t, uint8_t>& col, const std::pair<uint8_t, uint8_t>& row) {
			return (currentPlayer == PlayerEnum::Player1 ? col.first : col.second) == (currentPlayer == PlayerEnum::Player1 ? row.first : row.second);
		});
}

void Game::switchPlayer()
{
	m_currentPlayer = (m_currentPlayer == PlayerEnum::Player1 ? PlayerEnum::Player2 : PlayerEnum::Player1);
	m_currentTimer = (m_currentTimer.get() == m_timerPlayer1 ? std::ref(m_timerPlayer2) : std::ref(m_timerPlayer1));
	//std::cout << "\nSwitch player to Player " << static_cast<int>(m_currentPlayer) + 1;
}

std::string_view Game::getPlayerChoice() const
{
	auto& player = m_players[static_cast<size_t>(m_currentPlayer)];
	auto& gamemode = player.getGamemode();

	auto hasUsedMage = player.hasUsedMage();
	auto hasUsedMagic = player.hasUsedMagic();

	std::cout << "\n> Possible available listed below. Please type what you choose:\n";
	std::cout << " > Card\n";

	if (!gamemode.getMages().empty())
	{
		std::cout << " > Mage\n";
	}
	if (!gamemode.getMagicPowers().empty() && !player.hasUsedMagic())
	{
		std::cout << " > Magic\n";
	}
	if (gamemode.getHasIlusions() && !player.hasUsedIllusion())
	{
		std::cout << " > Illusion\n";
	}
	if (gamemode.getHasExplosions() && player.canUseExplosion())
	{
		std::cout << " > Explosion\n";
	}
	std::cout << " > Reset\n";

	static std::string choice;
	std::cout << "Your choice: ";
	std::cin >> choice;
	std::cout << '\n';


	std::transform(choice.begin(), choice.end(), choice.begin(), ::tolower);

	if (choice == "card" ||
		(choice == "mage" && !gamemode.getMages().empty() && !hasUsedMage) ||
		(choice == "magic" && !gamemode.getMagicPowers().empty() && !hasUsedMagic) ||
		(choice == "illusion" && gamemode.getHasIlusions()) && !player.hasUsedIllusion()|| 
		(choice == "explosion" && gamemode.getHasExplosions() && player.canUseExplosion()) ||
		choice == "reset")
	{
		return choice;
	}
	else
	{
		std::cout << "Invalid choice. Please try again.\n";
		return getPlayerChoice();
	}
}

void Game::handleChoice(std::string_view choice)
{
	if (choice == "card")
	{
		placeCard();
	}
	else if (choice == "mage")
	{
		useMage();
	}
	else if (choice == "magic")
	{
		useMagicPowers();
	}
	else if (choice == "illusion")
	{
		placeIllusion();
	}
	else if (choice == "explosion")
	{
		useExplosion();
	}
	else if (choice == "reset")
	{
		std::cout << "Game reset.\n";
		resetGame();
	}
}

void Game::returnToPlayer()
{
	if (m_board.isBoardEmpty())
	{
		return;
	}

	size_t posX, posY;
	std::cout << "Return to player the card on position: ";
	std::cin >> posX>> posY;
	std::cout << "\n";

	if (!m_board.validateInsertPosition({posX,posY}))
	{
		std::cout << "Possition not available!";
		return returnToPlayer();
	}

	if (!m_board.getBoard()[posX][posY].has_value())/*nu are valoare*/
	{
		std::cout << "\nEmpty cell!\n";
		return returnToPlayer();
	}

	auto& cell = m_board.getBoard()[posX][posY]->top();
	m_players[static_cast<size_t>(cell.getPlayerID())].addCard(cell.getValue());
	m_board.removeCard({ posX,posY });
	std::cout << "\nCard returned to player!\n";
}

void Game::placeRandomPit()
{
	if (!m_board.canHandlePit())
	{
		return;
	}

	auto gMiddle = m_board.getGridMiddle();
	auto is4x4 = m_players[0].getGamemode().getIs4x4();

	int minRow = gMiddle.first - 1 - 0.5 * is4x4;
	int maxRow = gMiddle.first + 1 + 0.5 * is4x4;
	int minCol = gMiddle.second - 1 - 0.5 * is4x4;
	int maxCol = gMiddle.second + 1 + 0.5 * is4x4;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> rowDist(minRow, maxRow);
	std::uniform_int_distribution<> colDist(minCol, maxCol);

	int randomRow = rowDist(gen);
	int randomCol = colDist(gen);


	while (m_board.getBoard()[randomRow][randomCol].has_value())
	{
		m_board.removeCard({ randomRow,randomCol });
	}


	static const uint8_t pitValue{ 5 };
	m_board.insertCard({ pitValue,m_currentPlayer }, { randomRow,randomCol });
}

void Game::removeCard()
{
	if (m_board.isBoardEmpty())
	{
		return;
	}

	size_t posX, posY;
	std::cout << "Remove the card on position: ";
	std::cin >> posX >> posY;
	std::cout << "\n";

	if (!m_board.validateInsertPosition({ posX,posY }))
	{
		std::cout << "Possition not available!";
		return returnToPlayer();
	}

	if (!m_board.getBoard()[posX][posY].has_value())/*nu are valoare*/
	{
		std::cout << "\nEmpty cell!\n";
		return returnToPlayer();
	}

	m_board.removeCard({ posX,posY });
}

bool Game::verifyExplosionCriteria() const
{
	if (m_players[0].getGamemode().getHasExplosions())
	{
		if (m_bridge.verifyRowExplosionCriteria() || m_bridge.verifyColumnExplosionCriteria())
		{
			return true;
		}
	}
	return false;
}

void Game::useMage()
{
	if (!m_players[static_cast<size_t>(m_currentPlayer)].hasUsedMage())
	{
		m_players[static_cast<size_t>(m_currentPlayer)].getGamemode().getMages()[static_cast<size_t>(m_currentPlayer)]->usePower(*this);
		m_board.revalidateInsertPosition();
		std::cout << "\n==============================================================\n";
	}
	else
	{
		std::cout << "here mage";
		return;
	}
}

void Game::useMagicPowers()
{
	if (!m_players[static_cast<size_t>(m_currentPlayer)].hasUsedMagic())
	{
		if (m_players[static_cast<size_t>(m_currentPlayer)].getGamemode().getMagicPowers()[0]->hasBeenUsed())
		{
			m_players[static_cast<size_t>(m_currentPlayer)].getGamemode().getMagicPowers()[1]->usePower(*this);
			m_board.revalidateInsertPosition();
			return;
		}
		else if (m_players[static_cast<size_t>(m_currentPlayer)].getGamemode().getMagicPowers()[1]->hasBeenUsed())
		{
			m_players[static_cast<size_t>(m_currentPlayer)].getGamemode().getMagicPowers()[0]->usePower(*this);
			m_board.revalidateInsertPosition();
			return;
		}

		int choice;
		std::cout << "\nWhich power do you choose: 1 or 2: ";
		std::cin >> choice;

		if (0 >= choice || choice > 2)
		{
			std::cout << "\nInvalid choice!\n";
			return;
		}

		m_players[static_cast<size_t>(m_currentPlayer)].getGamemode().getMagicPowers()[choice-1]->usePower(*this);
		m_board.revalidateInsertPosition();
		std::cout << "\n==============================================================\n";
	}
	else
	{
		std::cout << "here magic";
		return;
	}
}

void Game::handleEterCard(const std::pair<size_t,size_t>& position)
{
	const uint8_t ETER{ 6 };

	if (!getCurrentPlayer().removeCard(ETER))
	{
		std::cout << "value available wrong!\n"; /*if card exists also delete it*/
		return placeCard(); /*restart loop*/
	}

	if (!m_board.validateInsertPosition({position.first,position.second}))
	{
		std::cout << "wrong!\n";
		return placeCard();
	}


	if (m_board.getBoard()[position.first][position.second].has_value())
	{
		std::cout << "\nEter card must be placed on an empty slot!";
		return handleChoice(getPlayerChoice());
	}


	if (!m_board.isFirstMove() && !m_board.tryGridShiftForInsertPosition(m_input.position))
	{
		std::cout << "grid wrong\n";
		return placeCard();
	}

	m_board.insertCard(Card{ m_input.value,m_currentPlayer }, m_input.position);

	if (m_areExplosionsEnabled && verifyExplosionCriteria())
	{
		m_players[static_cast<size_t>(m_currentPlayer)].enableExplosion();
		m_areExplosionsEnabled = false;
	}

	if (m_board.getLockcase() < m_board.getMinLockcaseValue())
	{
		m_board.addPositionToValid(m_input.position);
		m_board.setLockcase();
	}

	std::cout << "\n==============================================================\n";
}


PlayerEnum Game::playGame()
{
	while (true)
	{
		printLogic();
		startTimer();
		handleChoice(getPlayerChoice());
		

		if (checkWinCase1(m_currentPlayer))
		{
			auto winner = (m_currentPlayer == PlayerEnum::Player1 ? "\nWon Player 1" : "\nWon Player 2");
			std::cout << winner;
			break;
		}
		else if (checkWinCase2(m_currentPlayer) || checkWinCase3(m_currentPlayer))
		{
			auto winner = (m_currentPlayer == PlayerEnum::Player1 ? "Player 1" : "Player 2");
			auto loser = (m_currentPlayer == PlayerEnum::Player1 ? "Player 2" : "Player 1");

			std::cout << winner << " won. " << loser << " can still place one last card. Do you want to? y/n ";
			std::cout << '\n';
			if (getResponse())
			{
				simulateLastMove();
			}
			return checkWinCase4();
		}

		if (timerRanOutOfTime())
		{
			std::cout << "\nPlayer " << static_cast<int>(m_currentPlayer) + 1 << " has ran out of time!\n"
					  <<"\n==============================================================\n";
			return onTimerTimeout();
		}


		stopTimer();
		switchPlayer();
	}
	return m_currentPlayer;
}

Game::Gamematrix& Game::accessGameboardAPI()
{
	return m_board.getBoard();
}

Board& Game::accessBoard()
{
	return m_board;
}

PlayerEnum Game::getCurrentPlayerEnum()
{
	return m_currentPlayer;
}

void Game::placeIllusion()
{
	const uint8_t ETER{ 6 };
	m_input.readInput();

	if (m_input.value == ETER)
	{
		std::cout << "\nCannot make Eter card an Illusion!";
		switchPlayer();
		return;
	}

	auto& cellx = m_board.getBoard()[m_input.position.first][m_input.position.second];
	if (cellx.has_value() && !cellx->empty() && cellx->top().isIllusion())
	{
		std::cout << "\nCannot place illusion on another illusion!";
		switchPlayer();
		return;
	}

	if (!m_board.validateInsertPosition(m_input.position))
	{
		std::cout << "wrong!\n";
		return handleChoice(getPlayerChoice()); /*restart loop*/
	}
	if (!m_board.validateValue(m_input.value))
	{
		std::cout << "value wrong!\n";
		return handleChoice(getPlayerChoice()); /*restart loop*/
	}

	if (m_input.position.first < 0 || m_input.position.first >= m_board.getBoard().size() ||
		m_input.position.second < 0 || m_input.position.second >= m_board.getBoard()[m_input.position.first].size()) {
		std::cout << "\nInvalid position!";
		return handleChoice(getPlayerChoice());
	}

	const auto& cell = m_board.getBoard()[m_input.position.first][m_input.position.second];
	if (cell.has_value() && !cell->empty()) {
		std::cout << "\nIllusion card must be placed on an empty slot!";
		return handleChoice(getPlayerChoice());
	}

	if (!getCurrentPlayer().removeCard(m_input.value))
	{
		std::cout << "value available wrong!\n"; /*if card exists also delete it*/
		return handleChoice(getPlayerChoice());
	}


	if (!m_board.isFirstMove() && !m_board.tryGridShiftForInsertPosition(m_input.position))
	{
		std::cout << "grid wrong\n";
		return handleChoice(getPlayerChoice());
	}


	m_board.insertCard(Card{ m_input.value,m_currentPlayer,true }, m_input.position);

	if (verifyExplosionCriteria())
	{
		m_players[static_cast<size_t>(m_currentPlayer)].enableExplosion();
	}

	if (m_board.getLockcase() < m_board.getMinLockcaseValue())
	{
		m_board.addPositionToValid(m_input.position);
		m_board.setLockcase();
	}

	getCurrentPlayer().markIllusionUsed();
	std::cout << "\n==============================================================\n";
}

void Game::useExplosion()
{
	auto is4x4 = m_players[0].getGamemode().getIs4x4();

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> timesDist(is4x4 ? 3 : 2, is4x4 ? 6 : 4);
	int timesToCall = timesDist(gen);

	std::discrete_distribution<> functionDist({ 10, 10, 1 });

	std::function<void()> functions[] = 
	{
		[&]() { returnToPlayer(); },
		[&]() { removeCard(); },
		[&]() { placeRandomPit(); }
	};

	for (int i = 0; i < timesToCall; ++i)
	{
		int chosenFunction = functionDist(gen);
		functions[chosenFunction]();
	}

}

void Game::placeCard()
{
	const uint8_t ETER{ 6 };
	m_input.readInput();

	//handle eter card placement
	if (m_input.value == ETER)
	{
		return handleEterCard(m_input.position);
	}

	//first ring of verification
	if (!m_board.validateInsertPosition(m_input.position))
	{
		std::cout << "wrong!\n";
		return placeCard(); /*restart loop*/
	}
	if (!m_board.validateValue(m_input.value))
	{
		std::cout << "value wrong!\n";
		return placeCard(); /*restart loop*/
	}


	auto& cell = m_board.getBoard()[m_input.position.first][m_input.position.second];
	if (cell.has_value() && !cell->empty() && cell->top().isIllusion())
	{
		if (!validateIllusionCover(m_input.position, m_input.value))
		{
			getCurrentPlayer().removeCard(m_input.value);
			std::cout << "\nCould not place over Illusion!";
			std::cout << "\n==============================================================\n";
			return;
		}
	}
	else
	{
		if (!m_board.validateStackRule(m_input.position, m_input.value)) {
			std::cout << "Value stack wrong!\n";
			return placeCard(); // Restart loop
		}
	}


	if (!getCurrentPlayer().removeCard(m_input.value))
	{
		std::cout << "value available wrong!\n"; /*if card exists also delete it*/
		return placeCard(); /*restart loop*/
	}

	//second ring of verification
	if (!m_board.isFirstMove() && !m_board.tryGridShiftForInsertPosition(m_input.position))
	{
		std::cout << "grid wrong\n";
		return placeCard();
	}

	m_board.insertCard(Card{ m_input.value,m_currentPlayer }, m_input.position);

	getCurrentPlayer().updateLastCardPlaced(m_input.position);

	if (m_areExplosionsEnabled && verifyExplosionCriteria())
	{
		m_players[static_cast<size_t>(m_currentPlayer)].enableExplosion();
		m_areExplosionsEnabled = false;
	}

	if (m_board.getLockcase() < m_board.getMinLockcaseValue())
	{
		m_board.addPositionToValid(m_input.position);
		m_board.setLockcase();
	}

	std::cout << "\n==============================================================\n";
}