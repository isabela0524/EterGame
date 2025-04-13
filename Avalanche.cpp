#include "Avalanche.h"
#include "Game.h"

Avalanche::Avalanche()
{
    m_description = "Avalanche: Choose two adjacent stacks horizontally/vertically and shift them by one position towards an empty space.";
}

void Avalanche::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool Avalanche::usePower(Game& game)
{
    std::cout << "Avalanche power activated: Shift two adjacent stacks by one position towards an empty space." << std::endl;
    size_t startRow, startCol, direction;
    std::cout << "\nEnter the row and column of the first stack: ";
    std::cin >> startRow >> startCol;

    std::cout << "\nChoose direction for shifting (1 for left/up, 2 for right/down): ";
    std::cin >> direction;

    bool isHorizontal = true; 

    if (direction < 1 || direction > 2) {
        std::cout << "\nInvalid direction!";
        return false;
    }

    // Check for adjacency
    auto& board = game.accessBoard();
    if ((board.getBoard()[startRow][startCol + 1].has_value() && isHorizontal) ||
        (board.getBoard()[startRow + 1][startCol].has_value() && !isHorizontal)) {
        std::stack<Card> tempStack;

        // Collect cards from the two stacks
        for (size_t i = 0; i < 2; ++i) {
            size_t row = startRow + (isHorizontal ? 0 : i);
            size_t col = startCol + (isHorizontal ? i : 0);

            if (board.getBoard()[row][col].has_value()) {
                auto& stack = board.getBoard()[row][col];
                while (!stack->empty()) {
                    tempStack.push(stack->top());
                    board.removeCard({ row, col });
                    if (!stack.has_value())
                    {
                        break;
                    }

                }
            }
        }


        // Determine target position
        size_t targetRow = startRow, targetCol = startCol;
        if (isHorizontal) {
            targetCol = direction == 1 ? startCol - 1 : startCol + 2;
            game.accessBoard().addPositionToValid({ targetRow,targetCol });
        }
        else {
            targetRow = direction == 1 ? startRow - 1 : startRow + 2;
             game.accessBoard().addPositionToValid({ targetRow,targetCol });
        }

        // Validate target position
        if (!board.validateInsertPosition({ targetRow, targetCol })) {
            std::cout << "\nInvalid target position!";
            return false;
        }

        // Insert collected cards into the target stack
        while (!tempStack.empty()) {
            board.insertCard({ tempStack.top().getValue(), tempStack.top().getPlayerID() }, { targetRow, targetCol });
            tempStack.pop();
        }
        game.accessBoard().addPositionToValid({ targetRow,targetCol });

    }
    else {
        std::cout << "\nSelected stacks are not adjacent!";
        return false;
    }


    markUsed();
    game.getCurrentPlayer().markMageUsed();
    return true;
}
