/*
 *made by KUR
 *
 * 빨리 들어온 순서대로 렌더링된다.
 *
 * displayList에서 DisplayObject의 참조를 가져올 수는 있으나(getChild...)
 * 권장하지 않는다.
 */
#pragma once

#include <list>
#include "DisplayObject.hpp"
using std::weak_ptr;
using std::list;

class DisplayObjectContainer: public DisplayObject {
private:
	list<weak_ptr<DisplayObject>> displayList;

public:
	DisplayObjectContainer(shared_ptr<SDL_Texture> tex,
						   shared_ptr<SDL_Renderer> ren,
						   double centerRatioW, double centerRatioH,
						   int x = 0, int y = 0,
						   int w = 0, int h = 0,
						   double angle = 0.0,
						   Uint8 alpha = 255,
						   Uint8 red = 255, Uint8 green = 255, Uint8 blue = 255);
	//x,y는 부모의 center를 (0,0)이라 두고 환산한 좌표
	//child만 받는 버전은 자식의 x,y가 부모 아래로 가면서 그대로 보존된다.
	void addChild(weak_ptr<DisplayObject> child, int XinParent, int YinParent);
	void addChild(weak_ptr<DisplayObject> child);

	//TODO:빠른 중간삽입/삭제/참조용 iterator를 보관하도록 개선할 것.
	//없으면 에러를 던진다.
	shared_ptr<DisplayObject> getChildAt(int index);
	//참조	
	shared_ptr<DisplayObject> operator[](int index);

	void renderCopy() = 0;

	
};


#include "Error.hpp"
class DisplayObjectContainerError: public Error {
public:
	int index;
	int listSize;

	//menu == 0: catch문을 위한 생성자
	DisplayObjectContainerError(){}
	
	//menu == 1: list의 인덱스 초과
	DisplayObjectContainerError(int index, int listSize)
		:Error(true, 1), index(index), listSize(listSize)
	{}

	//menu == 2: weak_ptr이 가리키던 객체가 해제되었거나 없음.
	//index == -1이면 addChild(child)에서 nullptr을 가리키는 child를 넣은 경우.
	DisplayObjectContainerError(int index)
		: Error(true, 2), index(index)
	{}

	void printErrorReason() override{
		switch(menu) {
		case 1:
			printf("getChildAt(%d)에러: 인덱스[%d]가 리스트 허용범위 [0] ~ [%d]를 벗어났습니다.", index, index, listSize - 1);
			break;

		case 2:
			if(index == -1) {
				printf("addChild(child)의 child가 가리키는 객체가 해제되었거나 없습니다.");
			} else {
				printf("인덱스[%d]의 weak_ptr이 가리키던 객체가 해제되었거나 displayList에서 제거되었습니다.", index); 
			}
			break;
		}
	}
};