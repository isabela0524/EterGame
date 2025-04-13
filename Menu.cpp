#include "Menu.h"
#include <cstdlib>

bool Menu::askBooleanOption(const std::string& question)
{
    char choice;
    std::cout << question;
    std::cin >> choice;
    return (choice == 'y' || choice == 'Y');
}

Gamemode Menu::showMenu()
{
    std::string selectedMode;
    int seconds{ -1 };

    std::cout << "\nChoose a gamemode (Training, Mage, Magic, MageAndMagic, Timer): ";
    std::cin >> selectedMode;

    if (selectedMode == "Timer" || selectedMode == "timer")
    {
        std::cout << "\nEnter the gamemode to apply Speed to (Training, Mage, Magic, MageAndMagic): ";
        std::cin >> selectedMode;
        std::cout << "\nEnter the number of secconds per game (30,60,120): ";
        std::cin >> seconds;
        if (seconds != 30 && seconds != 60 && seconds != 120)
        {
            std::cout << "\nTimer unavailable!";
            seconds = -1;
        }
    }
 
    Gamemode selectedGamemode = GamemodeDistributor::generateGamemode(selectedMode,
            askBooleanOption("\nDo you want illusions in the game? (y/n): "),
            askBooleanOption("\nDo you want explosions in the game? (y/n): "),
            seconds);
  

    std::system("cls");

    return selectedGamemode;
}

void Menu::handleMatch()
{
    Game_UI* game = new Game_UI();
    game->handleEvents();
    game->update();
    game->render();

    Match eterMatch(showMenu());
    eterMatch.startMatch();
}
