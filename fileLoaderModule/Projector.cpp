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
	//SDL_RenderClear(renderer.get());	//��ɷ� ���� ���� �����
	// root.renderCopy();				//���⼭ ȭ���� ��� ������Ʈ�ϰ� 
	SDL_RenderPresent(renderer.get());	//�����θ��ϴ�.
}