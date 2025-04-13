#include "FireMage.h"
#include "Game.h"

FireMage::FireMage()
{
    m_description = "FireMage: Removes a row or column of cards from the board containing at least 3 positions and one own card.";
}

void FireMage::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool FireMage::usePower(Game& game) const
{
    std::cout << "FireMage uses power to remove a row of cards!" << std::endl;
    auto gb = game.accessBoard();  

    int choice;
    std::cout << "\nPlease input if you'd like to remove a column or a row: \n1.Row\n2.Column\n";
    std::cin >> choice;


    if (choice != 1 && choice != 2)
    {
        std::cout << "\nInvalid choice!";
        return false;
    }

    
    if (choice == 1)
    {
        size_t row;
        std::cout << "\nPlease enter the row you want to erase: ";
        std::cin >> row;

        bool conditionMet = false;
        int noOfPositions = 0;


        int minRow = gb.getGridMiddle().second - 1 - 0.5 * gb.getis4x4();
        int maxRow = gb.getGridMiddle().second + 1 + 0.5 * gb.getis4x4();
        std::cout << "From " << minRow << " to " << maxRow << "\n";
        
        for (int j = minRow; j <= maxRow; ++j)
        {
  
            if (game.accessGameboardAPI()[row][j].has_value())
                noOfPositions++;


            if (gb.containsOwnCard({ row, j }, game.getCurrentPlayerEnum()))
                conditionMet = true;
        }

        if (conditionMet && noOfPositions >= 3)
        {
            for (int j = minRow; j <= maxRow; ++j)
            {
                auto& stack = game.accessGameboardAPI()[row][j];
                while (stack.has_value())
                {
                    game.accessBoard().removeCard({ row,j });
                }
            }
            game.getCurrentPlayer().markMageUsed();
            return true;
        }
        else
        {
            std::cout << "\nCannot remove row!\n";
            return false;
        }
    }
    else if (choice == 2)
    {
        size_t col;
        std::cout << "\nPlease enter the column you want to erase: ";
        std::cin >> col;

        bool conditionMet = false;
        int noOfPositions = 0;

        
        int minCol = static_cast<int>(gb.getGridMiddle().first) - 0.5 * gb.getis4x4();
        int maxCol = static_cast<int>(gb.getGridMiddle().first) + 0.5 * gb.getis4x4();

       
        for (int i = minCol; i <= maxCol; ++i)
        {
           
            if (game.accessGameboardAPI()[i][col].has_value())
                noOfPositions++;

           
            if (gb.containsOwnCard({ i, col }, game.getCurrentPlayerEnum()))
                conditionMet = true;
        }

        if (conditionMet && noOfPositions >= 3)
        {
            for (int i = minCol; i <= maxCol; ++i)
            {
                auto& stack = game.accessGameboardAPI()[i][col];
                if (stack.has_value()) 
                {
                    while (!stack->empty())
                    {
                        game.accessBoard().removeCard({ i, col });
                    }
                }
            }
            game.getCurrentPlayer().markMageUsed();
            return true;
        }
        else
        {
            std::cout << "\nCannot remove column!\n";
            return false;
        }
    }

    return false;
}
