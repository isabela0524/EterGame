#include "Hurricane.h"
#include "Game.h"

Hurricane::Hurricane()
{
    m_description = "Hurricane: Shifts a fully occupied row or column by one position in the desired direction.";
}

void Hurricane::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool Hurricane::usePower(Game& game)
{
    int choice, condition;
    std::cout << "\nPress 1 for row, 2 for column: ";
    std::cin >> choice;
    if (choice <= 0 || choice > 2)
    {
        return false;
    }

    const auto& gMiddle = game.accessBoard().getGridMiddle();
    const auto& is4x4 = game.accessBoard().getis4x4();
    auto& board = game.accessBoard();

    if (choice == 1)
    {
        std::cout << "\nEnter row number: ";
        std::cin >> condition;

        int noOfFound{ 0 };

        for (size_t j = gMiddle.second - 1 - 0.5 * is4x4; j <= gMiddle.second + 1 + 0.5 * is4x4; j++)
        {
            if (!board.validateInsertPosition({ condition, j }))
            {
                std::cout << "\nInvalid row!\n";
                return false;
            }

            if (board.getBoard()[condition][j].has_value())
            {
                noOfFound++;
            }
        }

        if (noOfFound != 4)
        {
            return false;
        }

        int shift{ 0 };
        std::cout << "\nDo you want to shift to the left, or to the right? 1 for left, 2 for right: ";
        std::cin >> shift;

        if (shift == 1) // Shift left
        {
            for (size_t j = gMiddle.second - 1 - 0.5 * is4x4; j < gMiddle.second + 1 + 0.5 * is4x4; j++)
            {
                board.getBoard()[condition][j] = std::move(board.getBoard()[condition][j + 1]);
            }
            board.getBoard()[condition][gMiddle.second + 1 + 0.5 * is4x4].reset();
        }
        else if (shift == 2) // Shift right
        {
            for (size_t j = gMiddle.second + 1 + 0.5 * is4x4; j > gMiddle.second - 1 - 0.5 * is4x4; j--)
            {
                board.getBoard()[condition][j] = std::move(board.getBoard()[condition][j - 1]);
            }
            board.getBoard()[condition][gMiddle.second - 1 - 0.5 * is4x4].reset();
        }
        else
        {
            std::cout << "\nInvalid shift direction!\n";
            return false;
        }
    }
    else if (choice == 2)
    {
        std::cout << "\nEnter column number: ";
        std::cin >> condition;

        int noOfFound{ 0 };

        for (size_t i = gMiddle.first - 1 - 0.5 * is4x4; i <= gMiddle.first + 1 + 0.5 * is4x4; i++)
        {
            if (!board.validateInsertPosition({ i, condition }))
            {
                std::cout << "\nInvalid column!\n";
                return false;
            }

            if (board.getBoard()[i][condition].has_value())
            {
                noOfFound++;
            }
        }

        if (noOfFound != 4)
        {
            return false;
        }

        int shift{ 0 };
        std::cout << "\nDo you want to shift up or down? 1 for up, 2 for down: ";
        std::cin >> shift;

        if (shift == 1) // Shift up
        {
            for (size_t i = gMiddle.first - 1 - 0.5 * is4x4; i < gMiddle.first + 1 + 0.5 * is4x4; i++)
            {
                board.getBoard()[i][condition] = std::move(board.getBoard()[i + 1][condition]);
            }
            board.getBoard()[gMiddle.first + 1 + 0.5 * is4x4][condition].reset();
        }
        else if (shift == 2) // Shift down
        {
            for (size_t i = gMiddle.first + 1 + 0.5 * is4x4; i > gMiddle.first - 1 - 0.5 * is4x4; i--)
            {
                board.getBoard()[i][condition] = std::move(board.getBoard()[i - 1][condition]);
            }
            board.getBoard()[gMiddle.first - 1 - 0.5 * is4x4][condition].reset();
        }
        else
        {
            std::cout << "\nInvalid shift direction!\n";
            return false;
        }
    }

    markUsed();
    game.getCurrentPlayer().markMagicUsed();
    return true;
}
