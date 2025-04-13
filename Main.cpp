#include <array>
#include <iostream>
#include "Menu.h"
#include "GameApp.h"

int main(int argc, char* argv[]) {

    /*Menu eterMenu;
    eterMenu.handleMatch();*/

    (void)argc; (void)argv;

    GameApp app;
    if (!app.init()) {
        std::cerr << "Failed to initialize the game.\n";
        return 1;
    }
    app.run();
    app.clean();

    return 0;
}