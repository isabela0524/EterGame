#include "Sparks.h"
#include "Game.h"

Sparks::Sparks()
{
    m_description = "Sparks: Choose any of your own cards covered by the opponent and play it immediately on another position.";
}

void Sparks::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool Sparks::usePower(Game& game)
{
    auto& board = game.accessBoard();

    std::cout << "Sparks power activated: Choose and play one of your own covered cards immediately on another position." << std::endl;

    size_t posX, posY;
    std::cout << "\nInput the position of own card that is covered by enemy: ";
    std::cin >> posX >> posY;

    if (!board.validateInsertPosition({ posX, posY })) {
        std::cout << "\nInvalid Position!";
        return false;
    }

    auto& cell = game.accessGameboardAPI()[posX][posY];
    if (!cell.has_value() || cell->empty()) {
        std::cout << "\nThis space is empty!\n";
        return false;
    }

    if (!board.isOwnCardCoveredByEnemy({ posX, posY })) {
        std::cout << "\nCard not covered by enemy\n";
        return false;
    }

    size_t insertX, insertY;
    std::cout << "\nInput insert position, DIFFERENT from the original position: ";
    std::cin >> insertX >> insertY;

    if (posX == insertX && posY == insertY) {
        std::cout << "\nCannot insert on the same position!";
        return false;
    }

    auto& stack = cell.value();
    auto currentTopCard = stack.top();
    auto valueOfOwnCard = currentTopCard.getValue();
    board.removeCard({ posX, posY });

    if (!board.validateInsertPosition({ insertX, insertY })) {
        std::cout << "Invalid insert position!\n";
        return false;
    }

    auto& targetCell = game.accessGameboardAPI()[insertX][insertY];
    if (targetCell.has_value() && !targetCell->empty() && targetCell->top().isIllusion()) {
        if (!game.validateIllusionCover({ insertX, insertY }, valueOfOwnCard)) {
            game.getCurrentPlayer().removeCard(valueOfOwnCard);
            std::cout << "\nCould not place over Illusion!";
            return false;
        }
    }
    else if (!board.validateStackRule({ insertX, insertY }, valueOfOwnCard)) {
        std::cout << "Invalid stack rule!\n";
        return false;
    }

    if (!board.isFirstMove() && !board.tryGridShiftForInsertPosition({ insertX, insertY })) {
        std::cout << "Grid shift failed!\n";
        return false;
    }

    board.insertCard(Card{ valueOfOwnCard, game.getCurrentPlayerEnum() }, { insertX, insertY });
    game.getCurrentPlayer().updateLastCardPlaced({ insertX, insertY });

    if (board.getLockcase() < board.getMinLockcaseValue()) {
        board.addPositionToValid({ insertX, insertY });
        board.setLockcase();
    }

    std::cout << "\n==============================================================\n";
    markUsed();
    game.getCurrentPlayer().markMagicUsed();
    return true;
}

