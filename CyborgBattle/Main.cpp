#include "cleanup.h"
#include "res_path.h"
#include "drawing_functions.h"
#include "SDL_mixer.h"
#include "Globals.h"
#include "Game.h"
#include <cstdlib> // srand, rand
#include <ctime>

int main(int argc, char** argv)
{
	// set the random seed for the random number sequence
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	//std::rand() % 10;

	// setup SDL
	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		std::cout << "Error initializing SDL" << std::endl;
		return 1;
	}

	// setup window
	// SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN
	SDL_Window* window = SDL_CreateWindow("Cyborg Battle",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		Globals::screenWidth * Globals::screenScale, 
		Globals::screenHeight * Globals::screenScale, 
		SDL_WINDOW_SHOWN /*| SDL_WINDOW_FULLSCREEN*/);

	if (!window)
	{
		SDL_Quit();
		std::cout << "Window Error." << std::endl;
		return 1;
	}

	// setup renderer
	Globals::renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!Globals::renderer)
	{
		cleanup(window);
		SDL_Quit();
		std::cout << "Renderer Error." << std::endl;
		return 1;
	}

	// this is the size to draw things before we scale it
	// to the screen size dimensions
	SDL_RenderSetLogicalSize(Globals::renderer, Globals::screenWidth, Globals::screenHeight);

	// initialize sdl_image
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		SDL_Quit();
		std::cout << "SDL Image did not initialize." << std::endl;
		return 1;
	}

	// initialize text to font
	if (TTF_Init())
	{
		SDL_Quit();
		std::cout << "SDL TTF did not initialize." << std::endl;
		return 1;
	}

	// initialize mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		SDL_Quit();
		std::cout << "SDL mixer did not initialize." << std::endl;
		return 1;
	}

	// load up a texture to draw
	/*std::string resPath = getResourcePath();
	SDL_Texture* texture = loadTexture(resPath + "map.png", Globals::renderer);

	// run game for 5000 ticks (5000 ms)
	while (SDL_GetTicks() < 5000)
	{
		// clear the screen
		SDL_RenderClear(Globals::renderer);
		// draw what we want
		renderTexture(texture, Globals::renderer, 0, 0);
		// show image we've been rendering
		SDL_RenderPresent(Globals::renderer);
	}*/

	Game game("map.png", "cyborgtitle.png", "overlay.png");
	game.update();

	cleanup(Globals::renderer);
	cleanup(window);
	//cleanup(texture);

	SDL_Quit();
	return 0;
}