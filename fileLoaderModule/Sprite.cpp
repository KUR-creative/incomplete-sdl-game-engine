#include "Sprite.hpp"

Sprite::Sprite(shared_ptr<Png> srcPng,
			   shared_ptr<SDL_Renderer> ren,
			   double centerRatioW, double centerRatioH,
			   int x, int y,
			   int w, int h,
			   double angle,
			   Uint8 alpha,
			   Uint8 red, Uint8 green, Uint8 blue)
	:DisplayObjectContainer(srcPng->Texture(), ren, centerRatioW, centerRatioH, x, y, w, h, angle, alpha, red, green, blue)
{}

void Sprite::renderCopy()
{
	//나중에 재귀 호출로 바꿔야한다.
	DisplayObjectContainer::renderCopy();
}