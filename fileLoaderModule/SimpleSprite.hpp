#pragma once

#include "DisplayObject.hpp"
#include "PngLoader.hpp"
//using std::shared_ptr;

class SimpleSprite: public DisplayObject {
public:
	SimpleSprite(shared_ptr<Png> srcPng,
				 shared_ptr<SDL_Renderer> ren,
				 int x, int y,
				 int w = 0, int h = 0,
				 double angle = 0.0,
				 SDL_RendererFlip flip = SDL_FLIP_NONE,
				 Uint8 alpha = 255,
				 Uint8 red = 255, Uint8 green = 255, Uint8 blue = 255);

	void renderCopy() override;
};