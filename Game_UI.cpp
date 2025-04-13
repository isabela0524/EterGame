#include <SDL.h>
#include <SDL_Image.h>
module Game_UI;

SDL_Texture* texture;


Game_UI::Game_UI()
{
	this->initUI("Eter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, false);

}


Game_UI::~Game_UI()
{

}


void Game_UI::initUI(const char* title, int x_pos, int y_pos, int width, int height, bool fullscreen) {

	int flags = 0;

	if (fullscreen == true)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{

		std::cout << "Subsistems Initialized" << std::endl;
		m_window = SDL_CreateWindow(title, x_pos, y_pos, width, height, flags);
		if (m_window)
		{
			std::cout << "Window created" << std::endl;
		}

		m_renderer = SDL_CreateRenderer(m_window, -1, 0);
		if (m_renderer)
		{
			SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
			std::cout << "Renderer created" << std::endl;
		}

		m_isRunning = true;
	}
	else
	{
		m_isRunning = false;
	}

	SDL_Surface* temporarySurface = IMG_Load("Eter.png");
	texture = SDL_CreateTextureFromSurface(m_renderer, temporarySurface);
	SDL_FreeSurface(temporarySurface);
}

void Game_UI::handleEvents()
{

	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		m_isRunning = false;
		break;
	default:
		break;
	}

	std::cout << "Hi!";
}

void Game_UI::update()
{

}

void Game_UI::render()
{
	SDL_RenderClear(m_renderer);
	//Render Stuff
	SDL_RenderCopy(m_renderer, texture, NULL, NULL);
	SDL_RenderPresent(m_renderer);
}

void Game_UI::clean()
{
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	SDL_Quit();
	std::cout << "Game cleaned";
}


bool Game_UI::running() { return m_isRunning; };
