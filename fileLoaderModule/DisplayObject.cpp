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

//�����ÿ� center�� �����ϰ� �Ѵ�. ��з��� w,h ���� �༭...
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
	//���� textureũ�� ���
	if(SDL_QueryTexture(tex.get(), NULL, NULL, &textureW, &textureH) != 0) {
		logSDLError("DisplayObject{ SDL_QueryTexture }");
	}
	//��ǥ
	rect.x = x;
	rect.y = y;
	//������ ũ��
	if( !(w && h) ) {//�ϳ��� 0�̸�
		rect.w = textureW;
		rect.h = textureH;
	} else {
		rect.w = w;		
		rect.h = h;
	}
	//������ ����
	ratioW = static_cast<double>(rect.w) / static_cast<double>(textureW);
	ratioH = static_cast<double>(rect.h) / static_cast<double>(textureH);
}

//todo:public�̶� ������..��������� ?
void DisplayObject::resetParent(DisplayObject* obj)
{
	parent = obj;
}

void DisplayObject::renderCopy()
{
	resetRenderMember();
#ifdef _DEBUG	//������ ����׵� resetParent�ƴϰ� ��.. ����ȭ ���带 � ������ �� ��. ��Ÿ�ӿ��� ���� �� ��������..
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

//ȸ���߽� ����: SDL_RenderCopy���� ���̴� ��ǥ�� SDL_Rect dstrect�� ���� �� �𼭸�(0.0)�̴�.
void DisplayObject::resetRenderMember()
{
	if(parent) {
		//1.ũ��, ����
		renderRatioW = parent->renderRatioW * ratioW;
		renderRatioH = parent->renderRatioH * ratioH;

		renderRect.w = renderRatioW * textureW;
		renderRect.h = renderRatioH * textureH;

		//2.�߽ɰ� ȸ��
		renderCenter.x = centerRatioW * renderRect.w;
		renderCenter.y = centerRatioH * renderRect.h;

		renderAngle = angle + parent->renderAngle;

		//3.���������� = �θ��� ������������ + �ڽ��� ��ǥ
		centerForChild.x = parent->centerForChild.x + rect.x * renderRatioW;
		centerForChild.y = parent->centerForChild.y + rect.y * renderRatioH;
		//�ڽĿ��� renderRect.x,y�� ȸ����ȯ�Ͽ� ���� �� ������ �Ǵ� ������ ������ ���Ѵ�.
		//�θ��� ����������centerForChild�� �������� �θ��� ����ŭ ȸ����ȯ.
		rotationTransform(parent->renderAngle,
						  parent->centerForChild.x, parent->centerForChild.y,
						  centerForChild.x, centerForChild.y,
						  centerForChild.x, centerForChild.y);

		//4.������ ��ǥ = �θ� ���� ���� ����		+ �ڽ��� ��ǥ��ŭ �����ֱ�	
		renderRect.x = parent->centerForChild.x + (rect.x * renderRatioW);
		renderRect.y = parent->centerForChild.y + (rect.y * renderRatioH);
		//renderCopy�� ���� renderRect.x,y�� ���ϱ� ���� ȸ����ȯ 
		//�θ��� ����������centerForChild�� �������� �θ��� ����ŭ ȸ����ȯ.
		rotationTransform(parent->renderAngle,
						  parent->centerForChild.x, parent->centerForChild.y,
						  renderRect.x, renderRect.y,
						  renderRect.x, renderRect.y);
		//ȸ����ȯ�� ��ǥ�� ����(parent->centerForChild)���� ���� �𼭸� ��(renderCopy�� ���̴� ��)���� ����
		//�������� �ؽ��� �𼭸� ��(renderCopy�� ���̴� ��)������ ���̴� �׻� �����ؾ� �ϱ⿡ ȸ����ȯ�� ����� �ƴϴ�.
		renderRect.x -= renderCenter.x;
		renderRect.y -= renderCenter.y;
		
		
	} 
	else { //�̰� root�� �ٲ�� ���ۿ� ����.
		renderRatioW = ratioW;
		renderRatioH = ratioH;

		renderRect.w = renderRatioW * textureW;
		renderRect.h = renderRatioH * textureH;

		renderCenter.x = centerRatioW * renderRect.w;
		renderCenter.y = centerRatioH * renderRect.h;

		renderAngle = angle;
		
		//��ǥ��ȭ
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
	ratioW = static_cast<double>(rect.w) / static_cast<double>(textureW); //����
	
}
void DisplayObject::H(int height){
	rect.h = height; 
	ratioH = static_cast<double>(rect.h) / static_cast<double>(textureH); //����
}
void DisplayObject::WH(int width, int height){
	rect.w = width;	
	rect.h = height; 
	ratioW = static_cast<double>(rect.w) / static_cast<double>(textureW); //����
	ratioH = static_cast<double>(rect.h) / static_cast<double>(textureH); //����
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
// cirAngle�� ȣ���� ������.
void rotationTransform(double cirAngle,
					   int oX, int oY,
					   int srcX, int srcY,
					   int& dstX, int& dstY)
{
	double rad = cirAngle * 3.14159265 / 180.0;
	dstX = (srcX - oX)*cos(rad) - (srcY - oY)*sin(rad) + oX;
	dstY = (srcX - oX)*sin(rad) + (srcY - oY)*cos(rad) + oY;
}

//tdod:�� ȸ����ȯ�� ���߿� TMP�� �Ἥ ����ȭ��ų���� ������..(�ǳ�? �۽�;..) ������ �Ƴ�
#include <gtest\gtest.h>
TEST(�����׽�Ʈ, rotationTransform){
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



