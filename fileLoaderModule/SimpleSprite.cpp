#include "SimpleSprite.hpp"

SimpleSprite::SimpleSprite(shared_ptr<Png> srcPng,
						   shared_ptr<SDL_Renderer> ren, 
						   int x, int y,
						   int w, int h,
						   double angle,
						   SDL_RendererFlip flip,
						   Uint8 alpha,
						   Uint8 red, Uint8 green, Uint8 blue)
	:DisplayObject(srcPng->Texture(), ren, x, y, w, h, angle, flip, alpha, red, green, blue)
{}

void SimpleSprite::renderCopy()
{
	DisplayObject::renderCopy();
}