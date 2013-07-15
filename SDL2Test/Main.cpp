#include <iostream>
#include <SDL.h>

// http://wiki.libsdl.org/moin.fcg/MigrationGuide#Looking_for_information
// http://wiki.libsdl.org/moin.fcg/SDL_RenderPresent

int main(int argc, char *argv[]) {
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		return -2;

	SDL_Window * window = SDL_CreateWindow("Steven Test SDL2",100,100,640,480, SDL_WINDOW_FULLSCREEN);
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
	
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	bool running = true;
	SDL_Event event;
	int red = 255;
	int green = 255;
	int blue = 255;
	int counter = 0;
	SDL_ShowCursor(0);
	
	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
				running = false;
			}
		}
		
		SDL_SetRenderDrawColor(renderer, red, green, blue, 0);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
		
		if (counter >= 150) {
			red = rand() % 255 + 1;
			green = rand() % 255 + 1;
			blue = rand() % 255 + 1;
			counter = 0;
		}
		else {
			counter++;
		}
	}

	SDL_DestroyRenderer(renderer);
	renderer = nullptr;

	SDL_DestroyWindow(window);
	window = nullptr;

	return 0;
}

