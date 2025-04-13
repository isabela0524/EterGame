#include "GameApp.h"

// --------------------------------------------------------------------
// CONSTRUCTOR
// --------------------------------------------------------------------
GameApp::GameApp()
    : m_window(nullptr),
    m_renderer(nullptr),
    m_running(true),
    m_currentState(SPLASH),
    m_splashAlpha(0),
    m_splashTimer(0),
    m_fadeDuration(4500),
    m_fadeIn(true),
    m_currentPlayerIndex(0),
    m_selectedGameMode(GameMode::SEVEN_CARDS)
{
}

// --------------------------------------------------------------------
// INITIALIZATION
// --------------------------------------------------------------------
bool GameApp::init()
{
    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf could not initialize! Error: " << TTF_GetError() << std::endl;
        return false;
    }
    m_font = TTF_OpenFont("../assets/Arial.ttf", 24); // Increased size for better visibility
    if (!m_font) {
        std::cerr << "Failed to load font! Error: " << TTF_GetError() << std::endl;
        return false;
    }

    // 1) SDL init
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        std::cerr << "SDL could not initialize! Error: "
            << SDL_GetError() << std::endl;
        return false;
    }

    // 2) SDL_image init (PNG)
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        std::cerr << "SDL_image could not initialize! Error: "
            << IMG_GetError() << std::endl;
        return false;
    }

    // 3) Create window
    m_window = SDL_CreateWindow("Eter Game",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (!m_window)
    {
        std::cerr << "Window could not be created! Error: "
            << SDL_GetError() << std::endl;
        return false;
    }

    // 4) Create renderer
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer)
    {
        std::cerr << "Renderer could not be created! Error: "
            << SDL_GetError() << std::endl;
        return false;
    }

    // 5) Load splash
    {
        SDL_Surface* surf = IMG_Load("Intro.png");
        if (!surf)
        {
            std::cerr << "Failed to load splash image: "
                << IMG_GetError() << std::endl;
            return false;
        }
        m_splashTexture = SDL_CreateTextureFromSurface(m_renderer, surf);
        SDL_FreeSurface(surf);
    }

    // 6) Load backgrounds
    {
        SDL_Surface* surf = IMG_Load("Menu.png");
        if (!surf)
        {
            std::cerr << "Failed to load main_menu_bg: "
                << IMG_GetError() << std::endl;
            return false;
        }
        m_mainMenuBg = SDL_CreateTextureFromSurface(m_renderer, surf);
        SDL_FreeSurface(surf);
    }
    {
        SDL_Surface* surf = IMG_Load("Menu.png");
        if (!surf)
        {
            std::cerr << "Failed to load settings_bg: "
                << IMG_GetError() << std::endl;
            return false;
        }
        m_settingsBg = SDL_CreateTextureFromSurface(m_renderer, surf);
        SDL_FreeSurface(surf);
    }
    {
        SDL_Surface* surf = IMG_Load("Menu.png");
        if (!surf)
        {
            std::cerr << "Failed to load game_board_bg: "
                << IMG_GetError() << std::endl;
            return false;
        }
        m_gameBoardBg = SDL_CreateTextureFromSurface(m_renderer, surf);
        SDL_FreeSurface(surf);
    }

    // 7) Prepare splash fade
    SDL_SetTextureBlendMode(m_splashTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(m_splashTexture, m_splashAlpha);

    // 8) Init buttons & board
    initButtons();
    initBoard(); // Board cells, position, etc.

    initCardTextures();
    initPlayerHands();

    return true;
}

void GameApp::initCardTextures()
{
    // Clear vectors in case this is called more than once
    m_blueCardTextures.clear();
    m_redCardTextures.clear();

    // --- BLUE CARDS ---
    std::vector<std::string> bluePaths = {
        "../assets/blue_card_1.png",
        "../assets/blue_card_2.png",
        "../assets/blue_card_3.png",
        "../assets/blue_card_4.png"
    };

    for (const auto& path : bluePaths) {
        SDL_Texture* texture = IMG_LoadTexture(m_renderer, path.c_str());
        if (!texture) {
            std::cerr << "Failed to load texture: " << path
                << " - " << IMG_GetError() << std::endl;
        }
        else {
            m_blueCardTextures.push_back(texture);
        }
    }

    // --- RED CARDS ---
    std::vector<std::string> redPaths = {
        "../assets/red_card_1.png",
        "../assets/red_card_2.png",
        "../assets/red_card_3.png",
        "../assets/red_card_4.png"
    };

    for (const auto& path : redPaths) {
        SDL_Texture* texture = IMG_LoadTexture(m_renderer, path.c_str());
        if (!texture) {
            std::cerr << "Failed to load texture: " << path
                << " - " << IMG_GetError() << std::endl;
        }
        else {
            m_redCardTextures.push_back(texture);
        }
    }
}


void GameApp::initPlayerHands()
{

    // Initialize Player 1 cards 
    for (int i = 0; i < 7; ++i) {
        Cardx card;
        card.value = (i < 2) ? 1 : (i < 4) ? 2 : (i < 6) ? 3 : 4;
        card.rect = { 100 + i * 120, 800, 100, 150 };
        card.faceUp = true;
        card.texture = m_blueCardTextures[card.value - 1];
        m_player1.hand.push_back(card);
    }

    // Initialize Player 2 cards
    for (int i = 0; i < 7; ++i) {
        Cardx card;
        card.value = (i < 2) ? 1 : (i < 4) ? 2 : (i < 6) ? 3 : 4;
        card.rect = { 100 + i * 120, 100, 100, 150 };
        card.faceUp = false;
        card.texture = m_redCardTextures[card.value - 1];
        m_player2.hand.push_back(card);
    }
}

void GameApp::cleanCardTextures()
{
    for (auto texture : m_blueCardTextures)
    {
        if (texture) SDL_DestroyTexture(texture);
    }
    m_blueCardTextures.clear();

    for (auto texture : m_redCardTextures)
    {
        if (texture) SDL_DestroyTexture(texture);
    }
    m_redCardTextures.clear();

}

// --------------------------------------------------------------------
// MAIN LOOP
// --------------------------------------------------------------------
void GameApp::run()
{
    Uint32 lastTick = getTick();

    while (m_running)
    {
        Uint32 currentTick = getTick();
        Uint32 deltaTime = currentTick - lastTick;
        lastTick = currentTick;

        handleEvents();
        update(deltaTime);
        render();

        // Frame capping
        Uint32 frameTime = getTick() - currentTick;
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
}

// --------------------------------------------------------------------
// CLEAN-UP
// --------------------------------------------------------------------
void GameApp::clean()
{
    if (m_font) {
        TTF_CloseFont(m_font);
    }
    TTF_Quit();
    if (m_splashTexture)   SDL_DestroyTexture(m_splashTexture);
    if (m_mainMenuBg)      SDL_DestroyTexture(m_mainMenuBg);
    if (m_settingsBg)      SDL_DestroyTexture(m_settingsBg);
    if (m_gameBoardBg)     SDL_DestroyTexture(m_gameBoardBg);

    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);

    IMG_Quit();
    SDL_Quit();
}

// --------------------------------------------------------------------
// GET TICKS (HELPER)
// --------------------------------------------------------------------
Uint32 GameApp::getTick()
{
    return SDL_GetTicks();
}

// --------------------------------------------------------------------
// BUTTONS INIT
// --------------------------------------------------------------------
void GameApp::initButtons()
{
    int buttonWidth = 400;
    int buttonHeight = 100;
    int gap = 10;

    SDL_Texture* texture = NULL;
    

    // Main menu buttons (bottom-middle)
    int xPos = (SCREEN_WIDTH - buttonWidth) / 2;
    int yPos = SCREEN_HEIGHT - 350; // offset from bottom

    m_playButton.rect = { xPos, yPos, buttonWidth, buttonHeight };
    m_playButton.text = "Play";
    m_playButton.hovered = false;
    SDL_Surface* textSurface = TTF_RenderText_Shaded(m_font, m_playButton.text.c_str(), SDL_Color(255.f, 255.f, 255.f, 255.f), SDL_Color(0.f, 0.f, 0.f, 0.f));
    texture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
    SDL_RenderCopy(m_renderer, texture, NULL, &m_playButton.rect);
    SDL_FreeSurface(textSurface);

    m_settingsButton.rect = { xPos, yPos + (buttonHeight + gap),
                             buttonWidth, buttonHeight };
    m_settingsButton.text = "Settings";
    m_settingsButton.hovered = false;

    m_quitButton.rect = { xPos, yPos + 2 * (buttonHeight + gap),
                             buttonWidth, buttonHeight };
    m_quitButton.text = "Quit";
    m_quitButton.hovered = false;

    // Settings menu buttons
    m_settingsButton1.rect = { 200, 200, buttonWidth, buttonHeight };
    m_settingsButton1.text = "Option A";   // e.g. 7 Cards
    m_settingsButton1.hovered = false;

    m_settingsButton2.rect = { 200, 310, buttonWidth, buttonHeight };
    m_settingsButton2.text = "Option B";   // e.g. 10 Cards
    m_settingsButton2.hovered = false;

    m_settingsButton3.rect = { 200, 420, buttonWidth, buttonHeight };
    m_settingsButton3.text = "Training";
    m_settingsButton3.hovered = false;

    m_settingsBackButton.rect = { 200, 530, buttonWidth, buttonHeight };
    m_settingsBackButton.text = "Back";
    m_settingsBackButton.hovered = false;
}

// --------------------------------------------------------------------
// BOARD INIT
// --------------------------------------------------------------------
void GameApp::initBoard()
{
    // Initialize all cells to empty
    for (int r = 0; r < BOARD_ROWS; ++r) {
        for (int c = 0; c < BOARD_COLS; ++c) {
            m_gameBoard.cells[r][c].cardOwner = -1;
            m_gameBoard.cells[r][c].cardIndex = -1;
        }
    }

    m_gameBoard.cellWidth = 80;
    m_gameBoard.cellHeight = 120;

    int totalBoardWidth = BOARD_COLS * m_gameBoard.cellWidth;
    int totalBoardHeight = BOARD_ROWS * m_gameBoard.cellHeight;

    // Center the 5×5 board in the screen
    m_gameBoard.boardX = (SCREEN_WIDTH - totalBoardWidth) / 2;
    m_gameBoard.boardY = (SCREEN_HEIGHT - totalBoardHeight) / 2;
}

// --------------------------------------------------------------------
// HANDLE EVENTS
// --------------------------------------------------------------------
void GameApp::handleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT) {
            m_running = false;
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            int mx = e.button.x;
            int my = e.button.y;

            // -------------------------
            // MAIN MENU
            // -------------------------
            if (m_currentState == MAIN_MENU)
            {
                if (isMouseOverButton(mx, my, m_playButton)) {
                    m_currentState = PLAY;
                    startGame();
                }
                else if (isMouseOverButton(mx, my, m_settingsButton)) {
                    m_currentState = SETTINGS;
                }
                else if (isMouseOverButton(mx, my, m_quitButton)) {
                    m_currentState = QUIT;
                    m_running = false;
                }
            }
            // -------------------------
            // SETTINGS
            // -------------------------
            else if (m_currentState == SETTINGS)
            {
                if (isMouseOverButton(mx, my, m_settingsBackButton)) {
                    m_currentState = MAIN_MENU;
                }
                // e.g., if Option A means 7 cards, Option B means 10 cards
                else if (isMouseOverButton(mx, my, m_settingsButton1)) {
                    m_selectedGameMode = GameMode::SEVEN_CARDS;
                }
                else if (isMouseOverButton(mx, my, m_settingsButton2)) {
                    m_selectedGameMode = GameMode::TEN_CARDS;
                }
                else if (isMouseOverButton(mx, my, m_settingsButton3)) {
                    // This sets the game to TRAINING mode
                    m_selectedGameMode = GameMode::TRAINING;
                }
                // etc.
            }
            // -------------------------
            // PLAY
            // -------------------------
            else if (m_currentState == PLAY)
            {
                // Drag detection on the *active player's* hand
                Player_UI& activePlayer = (m_currentPlayerIndex == 0) ? m_player1 : m_player2;

                for (auto& card : activePlayer.hand)
                {
                    if (mx >= card.rect.x && mx <= card.rect.x + card.rect.w &&
                        my >= card.rect.y && my <= card.rect.y + card.rect.h)
                    {
                        card.beingDragged = true;
                        card.offsetX = mx - card.rect.x;
                        card.offsetY = my - card.rect.y;
                    }
                }
                // Check if back button clicked
                if (isMouseOverButton(mx, my, m_settingsBackButton)) {
                    m_currentState = MAIN_MENU;
                }
            }
        }
        else if (e.type == SDL_MOUSEBUTTONUP)
        {
            int mx = e.button.x;
            int my = e.button.y;

            if (m_currentState == PLAY)
            {
                // Release dragged cards from active player
                Player_UI& activePlayer = (m_currentPlayerIndex == 0) ? m_player1 : m_player2;

                for (int i = 0; i < (int)activePlayer.hand.size(); ++i) {
                    Cardx& card = activePlayer.hand[i];
                    if (card.beingDragged) {
                        card.beingDragged = false;

                        // Attempt to place on board
                        placeCardOnBoard(card, m_currentPlayerIndex, i);
                    }
                }
            }
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            if (m_currentState == PLAY)
            {
                int mx = e.motion.x;
                int my = e.motion.y;

                // Move any dragged card in active player's hand
                Player_UI& activePlayer = (m_currentPlayerIndex == 0) ? m_player1 : m_player2;
                for (auto& card : activePlayer.hand)
                {
                    if (card.beingDragged)
                    {
                        card.rect.x = mx - card.offsetX;
                        card.rect.y = my - card.offsetY;
                    }
                }
            }
        }
    }
}

// --------------------------------------------------------------------
// UPDATE (PER STATE)
// --------------------------------------------------------------------
void GameApp::update(Uint32 deltaTime)
{
    switch (m_currentState)
    {
    case SPLASH:
        updateSplash(deltaTime);
        break;
    case MAIN_MENU:
        updateMainMenu();
        break;
    case SETTINGS:
        updateSettingsMenu();
        break;
    case PLAY:
        updateGameBoard(deltaTime);
        break;
    case QUIT:
        m_running = false;
        break;
    }
}

// --------------------------------------------------------------------
// RENDER (PER STATE)
// --------------------------------------------------------------------
void GameApp::render()
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    switch (m_currentState)
    {
    case SPLASH:
        renderSplash();
        break;
    case MAIN_MENU:
        renderMainMenu();
        break;
    case SETTINGS:
        renderSettingsMenu();
        break;
    case PLAY:
        renderGameBoard();
        break;
    case QUIT:
        // optional goodbye screen
        break;
    }

    SDL_RenderPresent(m_renderer);
}

void GameApp::renderText(SDL_Renderer* renderer, const std::string& text, TTF_Font* font, SDL_Color color, SDL_Rect* buttonRect)
{
    // Create a surface with the text rendered on it
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!textSurface) {
        std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
        return;
    }

    // Create a texture from the surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);  // Free the surface after creating the texture

    if (!textTexture) {
        std::cerr << "Failed to create texture from text: " << SDL_GetError() << std::endl;
        return;
    }

    // Get the dimensions of the text texture
    int textWidth = 0, textHeight = 0;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    // Position the text centered within the button's rectangle
    SDL_Rect textRect = {
        buttonRect->x + (buttonRect->w - textWidth) / 2,
        buttonRect->y + (buttonRect->h - textHeight) / 2,
        textWidth,
        textHeight
    };

    // Render the text texture
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);  // Clean up the texture after rendering
}

// --------------------------------------------------------------------
// SPLASH UPDATES
// --------------------------------------------------------------------
void GameApp::updateSplash(Uint32 deltaTime)
{
    m_splashTimer += deltaTime;
    float halfDuration = (float)m_fadeDuration / 2.0f;

    if (m_fadeIn)
    {
        // fade in
        float ratio = (float)m_splashTimer / halfDuration;
        m_splashAlpha = (int)(255.0f * ratio);
        if (m_splashAlpha >= 255)
        {
            m_splashAlpha = 255;
            m_fadeIn = false;
            m_splashTimer = 0; // reset to start fade out
        }
    }
    else
    {
        // fade out
        float ratio = (float)m_splashTimer / halfDuration;
        m_splashAlpha = 255 - (int)(255.0f * ratio);
        if (m_splashAlpha <= 0)
        {
            m_splashAlpha = 0;
            m_currentState = MAIN_MENU; // go to menu
        }
    }
    SDL_SetTextureAlphaMod(m_splashTexture, m_splashAlpha);
}

// --------------------------------------------------------------------
// SPLASH RENDER
// --------------------------------------------------------------------
void GameApp::renderSplash()
{
    SDL_Rect dst = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(m_renderer, m_splashTexture, nullptr, &dst);
}

// --------------------------------------------------------------------
// MAIN MENU LOGIC
// --------------------------------------------------------------------
void GameApp::updateMainMenu()
{
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    m_playButton.hovered = isMouseOverButton(mx, my, m_playButton);
    m_settingsButton.hovered = isMouseOverButton(mx, my, m_settingsButton);
    m_quitButton.hovered = isMouseOverButton(mx, my, m_quitButton);
}

void GameApp::renderMainMenu()
{
    SDL_Rect dst = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(m_renderer, m_mainMenuBg, nullptr, &dst);

    SDL_Color buttonColor = { 70, 70, 70, 255 };  // Dark gray for button background
    SDL_Color hoverColor = { 100, 100, 100, 255 }; // Lighter gray for hover
    SDL_Color textColor = { 255, 255, 255, 255 };  // White text

    auto drawButton = [&]( Button& btn) {
        // Draw button background
        SDL_SetRenderDrawColor(m_renderer,
            btn.hovered ? hoverColor.r : buttonColor.r,
            btn.hovered ? hoverColor.g : buttonColor.g,
            btn.hovered ? hoverColor.b : buttonColor.b,
            255);
        SDL_RenderFillRect(m_renderer, &btn.rect);

        // Draw button border
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(m_renderer, &btn.rect);

        // Render button text
        renderText(m_renderer, btn.text, m_font, textColor, &btn.rect);
        };

    drawButton(m_playButton);
    drawButton(m_settingsButton);
    drawButton(m_quitButton);
}

// --------------------------------------------------------------------
// SETTINGS LOGIC
// --------------------------------------------------------------------
void GameApp::updateSettingsMenu()
{
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    m_settingsButton1.hovered = isMouseOverButton(mx, my, m_settingsButton1);
    m_settingsButton2.hovered = isMouseOverButton(mx, my, m_settingsButton2);
    m_settingsButton3.hovered = isMouseOverButton(mx, my, m_settingsButton3);
    m_settingsBackButton.hovered = isMouseOverButton(mx, my, m_settingsBackButton);
}

void GameApp::renderSettingsMenu()
{
    SDL_Rect dst = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(m_renderer, m_settingsBg, nullptr, &dst);

    // Draw buttons
    SDL_Color buttonColor = { 70, 70, 70, 255 };
    SDL_Color hoverColor = { 100, 100, 100, 255 };
    SDL_Color textColor = { 255, 255, 255, 255 };

    auto drawButton = [&]( Button& btn) {
        // Draw button background
        SDL_SetRenderDrawColor(m_renderer,
            btn.hovered ? hoverColor.r : buttonColor.r,
            btn.hovered ? hoverColor.g : buttonColor.g,
            btn.hovered ? hoverColor.b : buttonColor.b,
            255);
        SDL_RenderFillRect(m_renderer, &btn.rect);

        // Draw button border
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(m_renderer, &btn.rect);

        // Render button text
        renderText(m_renderer, btn.text, m_font, textColor, &btn.rect);
        };

    drawButton(m_settingsButton1);
    drawButton(m_settingsButton2);
    drawButton(m_settingsButton3);
    drawButton(m_settingsBackButton);
}

// --------------------------------------------------------------------
// GAME BOARD LOGIC
// --------------------------------------------------------------------
void GameApp::updateGameBoard(Uint32 deltaTime)
{
    (void)deltaTime;
    // Possibly do animations, timers, etc.

    // Also check the "Back" button hover
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    m_settingsBackButton.hovered = isMouseOverButton(mx, my, m_settingsBackButton);
}

void GameApp::renderGameBoard()
{
    // 1) Draw the game board background
    SDL_Rect dst = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(m_renderer, m_gameBoardBg, nullptr, &dst);

    // 2) (Optional) Draw the board's grid lines
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    for (int r = 0; r <= BOARD_ROWS; ++r) {
        int y = m_gameBoard.boardY + r * m_gameBoard.cellHeight;
        SDL_RenderDrawLine(m_renderer,
            m_gameBoard.boardX, y,
            m_gameBoard.boardX + BOARD_COLS * m_gameBoard.cellWidth,
            y);
    }
    for (int c = 0; c <= BOARD_COLS; ++c) {
        int x = m_gameBoard.boardX + c * m_gameBoard.cellWidth;
        SDL_RenderDrawLine(m_renderer,
            x, m_gameBoard.boardY,
            x, m_gameBoard.boardY + BOARD_ROWS * m_gameBoard.cellHeight);
    }

    // 3) Render the *active* player's cards
    Player_UI& activePlayer = (m_currentPlayerIndex == 0) ? m_player1 : m_player2;
    for (auto& card : activePlayer.hand)
    {
        if (card.faceUp) {
            // Show the card’s texture if face-up
            SDL_RenderCopy(m_renderer, card.texture, nullptr, &card.rect);
        }
        else {
            // Draw a gray rectangle for face-down
            SDL_SetRenderDrawColor(m_renderer, 100, 100, 100, 255);
            SDL_RenderFillRect(m_renderer, &card.rect);
        }
    }

    // 4) Render the *inactive* player's cards
    Player_UI& inactivePlayer = (m_currentPlayerIndex == 0) ? m_player2 : m_player1;
    for (auto& card : inactivePlayer.hand)
    {
        if (card.faceUp) {
            // Show the card’s texture if face-up
            SDL_RenderCopy(m_renderer, card.texture, nullptr, &card.rect);
        }
        else {
            // Draw a gray rectangle for face-down
            SDL_SetRenderDrawColor(m_renderer, 100, 100, 100, 255);
            SDL_RenderFillRect(m_renderer, &card.rect);
        }
    }

    // 5) Render the Back button
    auto drawButton = [&](Button& btn) {
        SDL_Color buttonColor = { 70, 70, 70, 255 };
        SDL_Color hoverColor = { 100, 100, 100, 255 };
        SDL_Color textColor = { 255, 255, 255, 255 };

        // Button background
        SDL_SetRenderDrawColor(m_renderer,
            btn.hovered ? hoverColor.r : buttonColor.r,
            btn.hovered ? hoverColor.g : buttonColor.g,
            btn.hovered ? hoverColor.b : buttonColor.b,
            255);
        SDL_RenderFillRect(m_renderer, &btn.rect);

        // Border
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(m_renderer, &btn.rect);

        // Text label
        renderText(m_renderer, btn.text, m_font, textColor, &btn.rect);
        };
    drawButton(m_settingsBackButton);
}


// --------------------------------------------------------------------
// START GAME (Give each player cards, etc.)
// --------------------------------------------------------------------
void GameApp::startGame()
{
    // Clear old hands
    m_player1.hand.clear();
    m_player2.hand.clear();

    if (m_selectedGameMode == GameMode::TRAINING)
    {
        // 1) Hard-code the 7 training card values
        std::vector<int> trainingValues = { 1,1,2,2,3,3,4 };

        // 2) For each player, create 7 cards with these values
        for (int i = 0; i < 7; ++i) {
            int v = trainingValues[i];

            Cardx cardP1;
            cardP1.value = v;
            cardP1.rect = { 100 + i * 70, SCREEN_HEIGHT - 200, 60, 90 };
            cardP1.beingDragged = false;
            cardP1.faceUp = true;  // player1 is active at start
            cardP1.texture = m_blueCardTextures[v - 1];
            m_player1.hand.push_back(cardP1);

            Cardx cardP2;
            cardP2.value = v;
            cardP2.rect = { 100 + i * 70, 100, 60, 90 };
            cardP2.beingDragged = false;
            cardP2.faceUp = false; // hidden if not active
            cardP2.texture = m_redCardTextures[v - 1];
            m_player2.hand.push_back(cardP2);
        }

        // 3) Force a 3×3 board if training
        initBoard();

    }
    else
    {
        // Decide how many cards per player
        int numCards = (m_selectedGameMode == GameMode::SEVEN_CARDS) ? 7 : 10;

        // Use any pattern you want for these cards.
        // Example: i=0..6 or i=0..9 cycling ranks 1..4
        for (int i = 0; i < numCards; ++i)
        {
            int v = (i % 4) + 1; // cycles through 1,2,3,4,1,2,3,4,...

            // Player 1's card
            Cardx cardP1;
            cardP1.value = v;
            cardP1.rect = { 100 + i * 70, SCREEN_HEIGHT - 200, 60, 90 };
            cardP1.beingDragged = false;
            cardP1.offsetX = 0;
            cardP1.offsetY = 0;
            cardP1.faceUp = true; // player1 is active initially
            cardP1.texture = m_blueCardTextures[v - 1];
            m_player1.hand.push_back(cardP1);

            // Player 2's card
            Cardx cardP2;
            cardP2.value = v;
            cardP2.rect = { 100 + i * 70, 100, 60, 90 };
            cardP2.beingDragged = false;
            cardP2.offsetX = 0;
            cardP2.offsetY = 0;
            cardP2.faceUp = false; // hidden if not their turn
            cardP2.texture = m_redCardTextures[v - 1];
            m_player2.hand.push_back(cardP2);
        }
    }

    // Active player = player1
    m_currentPlayerIndex = 0;

    // Reset the board to empty
    initBoard();

    // Go to play state
    m_currentState = PLAY;
}


// --------------------------------------------------------------------
// SWITCH TURNS (Flip faceUp states, etc.)
// --------------------------------------------------------------------
void GameApp::switchTurns()
{
    // Toggle 0 <-> 1
    m_currentPlayerIndex = (m_currentPlayerIndex == 0) ? 1 : 0;

    // Face-up for active player's cards, face-down for inactive
    for (auto& c : m_player1.hand) {
        c.faceUp = (m_currentPlayerIndex == 0);
    }
    for (auto& c : m_player2.hand) {
        c.faceUp = (m_currentPlayerIndex == 1);
    }
}

// --------------------------------------------------------------------
// PLACE CARD ON BOARD
// --------------------------------------------------------------------
void GameApp::placeCardOnBoard(Cardx& card, int ownerID, int cardIndex)
{
    // Check all cells to see if we can place it
    for (int r = 0; r < BOARD_ROWS; ++r) {
        for (int c = 0; c < BOARD_COLS; ++c) {
            if (m_gameBoard.cells[r][c].cardOwner == -1) {
                // bounding box of this cell
                int cellX = m_gameBoard.boardX + c * m_gameBoard.cellWidth;
                int cellY = m_gameBoard.boardY + r * m_gameBoard.cellHeight;
                SDL_Rect cellRect = { cellX, cellY, m_gameBoard.cellWidth, m_gameBoard.cellHeight };

                // Check if card center is in cell
                int cardCenterX = card.rect.x + card.rect.w / 2;
                int cardCenterY = card.rect.y + card.rect.h / 2;

                if (cardCenterX >= cellRect.x && cardCenterX <= cellRect.x + cellRect.w &&
                    cardCenterY >= cellRect.y && cardCenterY <= cellRect.y + cellRect.h)
                {
                    // Snap the card to the center
                    card.rect.x = cellX + (m_gameBoard.cellWidth - card.rect.w) / 2;
                    card.rect.y = cellY + (m_gameBoard.cellHeight - card.rect.h) / 2;

                    // Mark board cell as occupied
                    m_gameBoard.cells[r][c].cardOwner = ownerID;
                    m_gameBoard.cells[r][c].cardIndex = cardIndex;

                    // Switch turns if your rule is "one card per turn"
                    switchTurns();
                    return;
                }
            }
        }
    }
}

// --------------------------------------------------------------------
// UTILITY: CHECK MOUSE OVER BUTTON
// --------------------------------------------------------------------
bool GameApp::isMouseOverButton(int mouseX, int mouseY, const Button& btn)
{
    return (mouseX >= btn.rect.x && mouseX <= btn.rect.x + btn.rect.w &&
        mouseY >= btn.rect.y && mouseY <= btn.rect.y + btn.rect.h);
}
