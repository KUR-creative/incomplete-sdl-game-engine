#pragma once

#include <SDL.h>
#include <memory>
using std::shared_ptr;

class DisplayObject {
private:
	shared_ptr<SDL_Renderer>	renderer;		
	shared_ptr<SDL_Texture>		texture;	

	//logical member
	SDL_Rect	rect;		//������ x,y,w,h
	double		angle;		//������ ����
	Uint8	alpha;			//������ ����
	Uint8	r, g, b;		//������ RGB��
	double	ratioW, ratioH;	//������ ũ�� ����
	const double	centerRatioW;	//������ �����Ѵ�. 0.5�� �߽��� w/2�� �ȴ�.
	const double	centerRatioH;	//������ �����Ѵ�. 0.5�� �߽��� h/2�� �ȴ�.
	
	//etc..
	DisplayObject*	parent;
	int	textureW;
	int	textureH;

	//render member
	SDL_Rect	renderRect;		//renderCopy�� �ʿ��� x,y,w,h
	double		renderAngle;	//renderCopy�� �ʿ��� ����
	SDL_Point	renderCenter;	//renderCopy�� �ʿ��� ȸ���߽� 
	Uint8	renderAlpha;				//���� ������ ����
	Uint8	renderR, renderG, renderB;	//���� ������ RGB
	double	renderRatioW, renderRatioH;	//���� ������ ����
	
	//renderCenter�� renderRect�� ������� ���� ���� ���̴�.
	//centerForChild�� ǥ�ð�ü�� ������ ���� �������Ǵ� ȭ����� ��ǥ�� �ǹ��Ѵ�.
	//�ڽĿ��� renderRect.x,y�� ȸ����ȯ�Ͽ� ���� �� ������ �Ǵ� ��ǥ.
	//renderOrigin ������ �̸��� ���ϼ��� �ִ�..
	SDL_Point	centerForChild;

public:
	// (w && h) == 0 �̸� tex�� w,h�� query�Ѵ�.
	//centerRatioW,H�� ����(�߽�)�� �����Ѵ�. (0.5, 0.5)�� ������ (w/2,h/2)�� �ȴ�.
	DisplayObject(shared_ptr<SDL_Texture> tex, 
				  shared_ptr<SDL_Renderer> ren,
				  double centerRatioW, double centerRatioH,
				  int x = 0, int y = 0,
				  int w = 0, int h = 0,	
				  double angle = 0.0,	
				  Uint8 alpha = 255,
				  Uint8 red = 255, Uint8 green = 255, Uint8 blue = 255);

	//DisplayObjectContainer::addChild() ������ ȣ���϶�.	
	void resetParent(DisplayObject* obj);
	//�⺻ ���� ���ǵǾ�����
	virtual void renderCopy() = 0; 
		
private:
	//�������� ȣ���.
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
	
	//ratioW,H�� �ٲ㼭 w,h�� �ٲٴ� �Լ��� ������ ������?
};

