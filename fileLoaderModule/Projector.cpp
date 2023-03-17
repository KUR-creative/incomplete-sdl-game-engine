#include "Projector.hpp"

Projector::Projector(const string& windowName,
					 int initPosX, int initPosY,
					 int screenWidth, int screenHeight)
					 :GAME_NAME(windowName),
					 INIT_X(initPosX), INIT_Y(initPosY),
					 SCREEN_WIDTH(screenWidth), SCREEN_HEIGHT(screenHeight),
					 window(nullptr)
{
	window.reset(
		SDL_CreateWindow(
			GAME_NAME.c_str()
			, INIT_X, INIT_Y
			, SCREEN_WIDTH, SCREEN_HEIGHT
			, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
		)	
	);

	renderer.reset(
		SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
		, &SDL_DestroyRenderer
	);
}

void Projector::renderPresent() 
{
	//SDL_RenderClear(renderer.get());	//요걸로 전의 것을 지우고
	// root.renderCopy();				//여기서 화면을 모두 업데이트하고 
	SDL_RenderPresent(renderer.get());	//보여두립니다.
}