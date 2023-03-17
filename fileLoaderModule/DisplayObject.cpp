#include <limits>
#include "DisplayObject.hpp"
#include "Utilities.hpp"

namespace{
	int Uint8min = std::numeric_limits<Uint8>::min();
	int Uint8max = std::numeric_limits<Uint8>::max();
}

void rotationTransform(double cirAngle,
					   int oX, int oY,
					   int srcX, int srcY,
					   int& dstX, int& dstY);

//생성시에 center를 선택하게 한다. 백분률로 w,h 값을 줘서...
DisplayObject::DisplayObject(shared_ptr<SDL_Texture> tex,
							 shared_ptr<SDL_Renderer> ren,	
							 double centerRatioW, double centerRatioH,
							 int x, int y,
							 int w, int h,	
							 double angle,							
							 Uint8 alpha,
							 Uint8 red, Uint8 green, Uint8 blue)
							 :renderer(ren), texture(tex),
							 angle(angle),
							 alpha(alpha),
							 r(red), g(green), b(blue),
							 parent(nullptr),
							 centerRatioW(centerRatioW), centerRatioH(centerRatioH)
{
	//원본 texture크기 얻기
	if(SDL_QueryTexture(tex.get(), NULL, NULL, &textureW, &textureH) != 0) {
		logSDLError("DisplayObject{ SDL_QueryTexture }");
	}
	//좌표
	rect.x = x;
	rect.y = y;
	//개념적 크기
	if( !(w && h) ) {//하나라도 0이면
		rect.w = textureW;
		rect.h = textureH;
	} else {
		rect.w = w;		
		rect.h = h;
	}
	//개념적 비율
	ratioW = static_cast<double>(rect.w) / static_cast<double>(textureW);
	ratioH = static_cast<double>(rect.h) / static_cast<double>(textureH);
}

//todo:public이라 위험해..개선방법은 ?
void DisplayObject::resetParent(DisplayObject* obj)
{
	parent = obj;
}

void DisplayObject::renderCopy()
{
	resetRenderMember();
#ifdef _DEBUG	//언젠가 디버그도 resetParent아니고 그.. 최적화 빌드를 몇개 가져야 할 듯. 런타임에도 있을 수 있을지도..
	if(SDL_SetTextureColorMod(texture.get(), r, g, b)) {
		logSDLError("DisplayObject::renderCopy() { SetTextureColorMod }");
	}
	if(SDL_SetTextureAlphaMod(texture.get(), alpha)) {
		logSDLError("DisplayObject::renderCopy() { SetTextureAlphaMod }");
	}
	if(SDL_RenderCopyEx(renderer.get(), texture.get(), nullptr, &renderRect, renderAngle, &renderCenter, SDL_FLIP_NONE)) {
		logSDLError("DisplayObject::renderCopy() { RenderCopyEx }");
	}

#else // release
	SDL_SetTextureColorMod(texture.get(), r, g, b)
	SDL_SetTextureAlphaMod(texture.get(), alpha);
	SDL_RenderCopyEx(renderer.get(), texture.get(), nullptr, &renderRect, renderAngle, &center, SDL_FLIP_NONE);

#endif 
}

//회전중심 보정: SDL_RenderCopy에서 쓰이는 좌표는 SDL_Rect dstrect의 왼쪽 위 모서리(0.0)이다.
void DisplayObject::resetRenderMember()
{
	if(parent) {
		//1.크기, 비율
		renderRatioW = parent->renderRatioW * ratioW;
		renderRatioH = parent->renderRatioH * ratioH;

		renderRect.w = renderRatioW * textureW;
		renderRect.h = renderRatioH * textureH;

		//2.중심과 회전
		renderCenter.x = centerRatioW * renderRect.w;
		renderCenter.y = centerRatioH * renderRect.h;

		renderAngle = angle + parent->renderAngle;

		//3.렌더링원점 = 부모의 렌더링원점과 + 자신의 좌표
		centerForChild.x = parent->centerForChild.x + rect.x * renderRatioW;
		centerForChild.y = parent->centerForChild.y + rect.y * renderRatioH;
		//자식에서 renderRect.x,y를 회전변환하여 구할 때 원점이 되는 렌더링 원점을 구한다.
		//부모의 렌더링원점centerForChild를 원점으로 부모의 각만큼 회전변환.
		rotationTransform(parent->renderAngle,
						  parent->centerForChild.x, parent->centerForChild.y,
						  centerForChild.x, centerForChild.y,
						  centerForChild.x, centerForChild.y);

		//4.렌더링 좌표 = 부모에 의한 원점 보정		+ 자신의 좌표만큼 더해주기	
		renderRect.x = parent->centerForChild.x + (rect.x * renderRatioW);
		renderRect.y = parent->centerForChild.y + (rect.y * renderRatioH);
		//renderCopy에 쓰일 renderRect.x,y를 구하기 위한 회전변환 
		//부모의 렌더링원점centerForChild를 원점으로 부모의 각만큼 회전변환.
		rotationTransform(parent->renderAngle,
						  parent->centerForChild.x, parent->centerForChild.y,
						  renderRect.x, renderRect.y,
						  renderRect.x, renderRect.y);
		//회전변환된 좌표에 원점(parent->centerForChild)에서 왼쪽 모서리 끝(renderCopy에 쓰이는 점)으로 보정
		//원점에서 텍스쳐 모서리 끝(renderCopy에 쓰이는 점)까지의 길이는 항상 일정해야 하기에 회전변환의 대상이 아니다.
		renderRect.x -= renderCenter.x;
		renderRect.y -= renderCenter.y;
		
		
	} 
	else { //이건 root가 바뀌는 경우밖에 없다.
		renderRatioW = ratioW;
		renderRatioH = ratioH;

		renderRect.w = renderRatioW * textureW;
		renderRect.h = renderRatioH * textureH;

		renderCenter.x = centerRatioW * renderRect.w;
		renderCenter.y = centerRatioH * renderRect.h;

		renderAngle = angle;
		
		//좌표변화
		renderRect.x = rect.x - renderCenter.x;
		renderRect.y = rect.y - renderCenter.y;
		
		centerForChild.x = rect.x;
		centerForChild.y = rect.y;
	}
}

//setter
void DisplayObject::X(int x){
	rect.x = x; 
}
void DisplayObject::Y(int y){ 
	rect.y = y; 
}
void DisplayObject::XY(int x, int y){
	rect.x = x;	
	rect.y = y; 
}

void DisplayObject::W(int width){
	rect.w = width; 
	ratioW = static_cast<double>(rect.w) / static_cast<double>(textureW); //재계산
	
}
void DisplayObject::H(int height){
	rect.h = height; 
	ratioH = static_cast<double>(rect.h) / static_cast<double>(textureH); //재계산
}
void DisplayObject::WH(int width, int height){
	rect.w = width;	
	rect.h = height; 
	ratioW = static_cast<double>(rect.w) / static_cast<double>(textureW); //재계산
	ratioH = static_cast<double>(rect.h) / static_cast<double>(textureH); //재계산
}

void DisplayObject::Angle(double a){ 
	angle = a; 
}

void DisplayObject::Alpha(int alp){
	if(alp < Uint8min) {
		alp = Uint8min;
	} else if(alp > Uint8max) {
		alp = Uint8max;
	}
	alpha = static_cast<Uint8>(alp);
}



#include <cmath>
// cirAngle은 호도법 단위임.
void rotationTransform(double cirAngle,
					   int oX, int oY,
					   int srcX, int srcY,
					   int& dstX, int& dstY)
{
	double rad = cirAngle * 3.14159265 / 180.0;
	dstX = (srcX - oX)*cos(rad) - (srcY - oY)*sin(rad) + oX;
	dstY = (srcX - oX)*sin(rad) + (srcY - oY)*cos(rad) + oY;
}

//tdod:이 회전변환은 나중에 TMP를 써서 최적화시킬수도 있으나..(되나? 글쎄;..) 지금은 아녀
#include <gtest\gtest.h>
TEST(작은테스트, rotationTransform){
	SDL_Point o		= { 20, 50 };
	SDL_Point src	= { 60, 50 };
	double angle = 90.0;
	SDL_Point actualDst = { 0, 0 };
	SDL_Point expDst	= { 20, 90 };
	rotationTransform(angle, o.x,o.y, src.x,src.y, actualDst.x,actualDst.y);
	printf("\n%d\n", actualDst.x);
	printf("\n%d\n", actualDst.y);
	ASSERT_TRUE(expDst.x == actualDst.x);
	ASSERT_TRUE(expDst.y == actualDst.y);
}



