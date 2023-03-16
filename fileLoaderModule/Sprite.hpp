#pragma once

#include "DisplayObjectContainer.hpp"
#include "PngLoader.hpp"

class Sprite: public DisplayObjectContainer {
public:
	Sprite(shared_ptr<Png> srcPng,
		   shared_ptr<SDL_Renderer> ren,
		   double centerRatioW, double centerRatioH,
		   int x = 0, int y = 0,
		   int w = 0, int h = 0,
		   double angle = 0.0,
		   Uint8 alpha = 255,
		   Uint8 red = 255, Uint8 green = 255, Uint8 blue = 255);
	void renderCopy() override;
};