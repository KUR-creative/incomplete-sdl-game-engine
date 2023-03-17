#pragma once

#include <SDL.h>
#include <memory>
using std::shared_ptr;

class DisplayObject {
private:
	shared_ptr<SDL_Renderer>	renderer;		
	shared_ptr<SDL_Texture>		texture;	

	//logical member
	SDL_Rect	rect;		//개념적 x,y,w,h
	double		angle;		//개념적 각도
	Uint8	alpha;			//개념적 알파
	Uint8	r, g, b;		//개념적 RGB값
	double	ratioW, ratioH;	//개념적 크기 비율
	const double	centerRatioW;	//원점을 설정한다. 0.5면 중심은 w/2가 된다.
	const double	centerRatioH;	//원점을 설정한다. 0.5면 중심은 h/2가 된다.
	
	//etc..
	DisplayObject*	parent;
	int	textureW;
	int	textureH;

	//render member
	SDL_Rect	renderRect;		//renderCopy에 필요한 x,y,w,h
	double		renderAngle;	//renderCopy에 필요한 각도
	SDL_Point	renderCenter;	//renderCopy에 필요한 회전중심 
	Uint8	renderAlpha;				//실제 블렌딩용 알파
	Uint8	renderR, renderG, renderB;	//실제 블렌딩용 RGB
	double	renderRatioW, renderRatioH;	//실제 렌더링 비율
	
	//renderCenter는 renderRect에 상대적인 값을 가질 뿐이다.
	//centerForChild는 표시객체의 원점이 실제 렌더링되는 화면상의 좌표를 의미한다.
	//자식에서 renderRect.x,y를 회전변환하여 구할 때 원점이 되는 좌표.
	//renderOrigin 정도로 이름을 붙일수도 있다..
	SDL_Point	centerForChild;

public:
	// (w && h) == 0 이면 tex의 w,h를 query한다.
	//centerRatioW,H는 원점(중심)을 설정한다. (0.5, 0.5)면 원점은 (w/2,h/2)가 된다.
	DisplayObject(shared_ptr<SDL_Texture> tex, 
				  shared_ptr<SDL_Renderer> ren,
				  double centerRatioW, double centerRatioH,
				  int x = 0, int y = 0,
				  int w = 0, int h = 0,	
				  double angle = 0.0,	
				  Uint8 alpha = 255,
				  Uint8 red = 255, Uint8 green = 255, Uint8 blue = 255);

	//DisplayObjectContainer::addChild() 에서만 호출하라.	
	void resetParent(DisplayObject* obj);
	//기본 구현 정의되어있음
	virtual void renderCopy() = 0; 
		
private:
	//렌더링시 호출됨.
	void resetRenderMember();

public:
	//getter
	const int X() const{ return rect.x; }
	const int Y() const{ return rect.y; }
	const int W() const{ return rect.w; }
	const int H() const{ return rect.h; }

	const double Angle() const{ return angle; }

	const Uint8	Alpha() const{ return alpha; }
	const Uint8	R() const{ return r; }
	const Uint8	G() const{ return g; }
	const Uint8	B() const{ return b; }

	int TextureW() const{ return textureW; }
	int TextureH() const{ return textureH; }
	double RatioW() const{ return ratioW; }
	double RatioH() const{ return ratioH; }

	//setter
	void X(int x);
	void Y(int y);
	void XY(int x, int y);

	void W(int width);
	void H(int height);
	void WH(int width, int height);

	void Angle(double a);

	void Alpha(int alp);
	void R(Uint8 red){ r = red; }
	void G(Uint8 green){ g = green; }
	void B(Uint8 blue){ b = blue; }
	void SetRGB(Uint8 red, Uint8 green, Uint8 blue){	
		r = red;
		g = green;
		b = blue;
	}
	
	//ratioW,H를 바꿔서 w,h를 바꾸는 함수도 있으면 좋을듯?
};

