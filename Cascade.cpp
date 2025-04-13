#include "Cascade.h"
#include "Game.h"

Cascade::Cascade()
{
    m_description = "Cascade: Choose a row or column with at least 3 occupied positions. The stacks on that row move towards an edge, forming a new stack.";
}

void Cascade::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool Cascade::usePower(Game& game) 
{
    std::cout << "Cascade power activated: Move stacks on a chosen row towards an edge, forming a new stack." << std::endl;

    size_t choice, condition;
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
            if (board.getBoard()[condition][j].has_value())
            {
                noOfFound++;
            }
        }

        if (noOfFound < 3)
        {
            std::cout << "\nThe row must have at least 3 occupied positions to cascade!\n";
            return false;
        }

        int cascadeDirection{ 0 };
        std::cout << "\nChoose cascade direction: 1 for left, 2 for right: ";
        std::cin >> cascadeDirection;

        if (cascadeDirection == 1) // Cascade left
        {
            std::stack<Card> newStack;

            for (size_t j = gMiddle.second - 1 - 0.5 * is4x4; j <= gMiddle.second + 1 + 0.5 * is4x4; j++)
            {
                if (board.getBoard()[condition][j].has_value())
                {
                    auto& stack = board.getBoard()[condition][j];
                    while (!stack->empty())
                    {
                        newStack.push(stack->top());
                        board.removeCard({ condition,j });
                        if (!stack.has_value())
                            break;
                    }
                }
            }

            while (!newStack.empty())
            {
                board.insertCard({ newStack.top().getValue(), newStack.top().getPlayerID() }, { condition, gMiddle.second - 1 - 0.5 * is4x4 });
                newStack.pop();
            }
            board.addPositionToValid({ condition, gMiddle.second - 1 - 0.5 * is4x4 });

        }
        else if (cascadeDirection == 2) // Cascade right
        {
            std::stack<Card> newStack;

            for (size_t j = gMiddle.second + 1 + 0.5 * is4x4; j >= gMiddle.second - 1 - 0.5 * is4x4; j--)
            {
                if (board.getBoard()[condition][j].has_value())
                {
                    auto& stack = board.getBoard()[condition][j];
                    while (!stack->empty())
                    {
                        newStack.push(stack->top());
                        board.removeCard({ condition,j });
                        if (!stack.has_value())
                            break;
                    }
                }
            }

            while (!newStack.empty())
            {
                board.insertCard({ newStack.top().getValue(), newStack.top().getPlayerID() }, { condition, gMiddle.second + 1 + 0.5 * is4x4 });
                newStack.pop();
            }
            board.addPositionToValid({ condition, gMiddle.second + 1 + 0.5 * is4x4 });
        }
        else
        {
            std::cout << "\nInvalid cascade direction!\n";
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
            if (board.getBoard()[i][condition].has_value())
            {
                noOfFound++;
            }
        }

        if (noOfFound < 3)
        {
            std::cout << "\nThe column must have at least 3 occupied positions to cascade!\n";
            return false;
        }

        int cascadeDirection{ 0 };
        std::cout << "\nChoose cascade direction: 1 for up, 2 for down: ";
        std::cin >> cascadeDirection;

        if (cascadeDirection == 1) // Cascade up
        {
            std::stack<Card> newStack;

            for (size_t i = gMiddle.first - 1 - 0.5 * is4x4; i <= gMiddle.first + 1 + 0.5 * is4x4; i++)
            {
                if (board.getBoard()[i][condition].has_value())
                {
                    auto& stack = board.getBoard()[i][condition];
                    while (!stack->empty())
                    {
                        newStack.push(stack->top());
                        board.removeCard({ i,condition });
                        if (!stack.has_value())
                            break;
                    }
                }
            }

            while (!newStack.empty())
            {
                board.insertCard({ newStack.top().getValue(), newStack.top().getPlayerID() }, { gMiddle.first - 1 - 0.5 * is4x4, condition });
                newStack.pop();
            }
            board.addPositionToValid({ gMiddle.first - 1 - 0.5 * is4x4, condition });
        }
        else if (cascadeDirection == 2) // Cascade down
        {
            std::stack<Card> newStack;

            for (size_t i = gMiddle.first + 1 + 0.5 * is4x4; i >= gMiddle.first - 1 - 0.5 * is4x4; i--)
            {
                if (board.getBoard()[i][condition].has_value())
                {
                    auto& stack = board.getBoard()[i][condition];
                    while (!stack->empty())
                    {
                        newStack.push(stack->top());
                        board.removeCard({ i,condition });
                        if (!stack.has_value())
                            break;
                    }
                }
            }

            while (!newStack.empty())
            {
                board.insertCard({ newStack.top().getValue(), newStack.top().getPlayerID() }, { gMiddle.first + 1 + 0.5 * is4x4, condition });
                newStack.pop();
            }
            board.addPositionToValid({ gMiddle.first + 1 + 0.5 * is4x4, condition });
        }
        else
        {
            std::cout << "\nInvalid cascade direction!\n";
            return false;
        }
    }

    markUsed();
    game.getCurrentPlayer().markMagicUsed();
    return true;
}
