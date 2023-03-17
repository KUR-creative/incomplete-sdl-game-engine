//made by KUR
//프로그래밍 갤러리 유동닉 kukyakya님에게 도움받았습니다. 감사합니다!
#pragma once

#include <SDL.h>
#include <string>
#include "Singleton.hpp"
using std::string;
using std::shared_ptr;

class Projector: public Singleton<Projector>
{
friend class Singleton<Projector>;

struct WindowDeleter{
	void operator()(SDL_Window* win) const{
		SDL_DestroyWindow(win);
	}
};

private:
	const string	GAME_NAME;
	const int	INIT_X;
	const int	INIT_Y;
	const int	SCREEN_WIDTH;
	const int	SCREEN_HEIGHT;
	unique_ptr<SDL_Window,WindowDeleter>	window;		
	shared_ptr<SDL_Renderer>				renderer;	

public:
	void renderPresent();

	shared_ptr<SDL_Renderer> Renderer() { return renderer; }

private:
	explicit Projector(const string& windowName,
					   int initPosX, int initPosY,
					   int screenWidth, int screenHeight);
};

//helper macro
#define PROJECTOR	Projector::Instance()