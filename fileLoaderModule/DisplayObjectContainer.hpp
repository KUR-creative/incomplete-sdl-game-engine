/*
 *made by KUR
 *
 * ���� ���� ������� �������ȴ�.
 *
 * displayList���� DisplayObject�� ������ ������ ���� ������(getChild...)
 * �������� �ʴ´�.
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
	//x,y�� �θ��� center�� (0,0)�̶� �ΰ� ȯ���� ��ǥ
	//child�� �޴� ������ �ڽ��� x,y�� �θ� �Ʒ��� ���鼭 �״�� �����ȴ�.
	void addChild(weak_ptr<DisplayObject> child, int XinParent, int YinParent);
	void addChild(weak_ptr<DisplayObject> child);

	//TODO:���� �߰�����/����/������ iterator�� �����ϵ��� ������ ��.
	//������ ������ ������.
	shared_ptr<DisplayObject> getChildAt(int index);
	//����	
	shared_ptr<DisplayObject> operator[](int index);

	void renderCopy() = 0;

	
};


#include "Error.hpp"
class DisplayObjectContainerError: public Error {
public:
	int index;
	int listSize;

	//menu == 0: catch���� ���� ������
	DisplayObjectContainerError(){}
	
	//menu == 1: list�� �ε��� �ʰ�
	DisplayObjectContainerError(int index, int listSize)
		:Error(true, 1), index(index), listSize(listSize)
	{}

	//menu == 2: weak_ptr�� ����Ű�� ��ü�� �����Ǿ��ų� ����.
	//index == -1�̸� addChild(child)���� nullptr�� ����Ű�� child�� ���� ���.
	DisplayObjectContainerError(int index)
		: Error(true, 2), index(index)
	{}

	void printErrorReason() override{
		switch(menu) {
		case 1:
			printf("getChildAt(%d)����: �ε���[%d]�� ����Ʈ ������ [0] ~ [%d]�� ������ϴ�.", index, index, listSize - 1);
			break;

		case 2:
			if(index == -1) {
				printf("addChild(child)�� child�� ����Ű�� ��ü�� �����Ǿ��ų� �����ϴ�.");
			} else {
				printf("�ε���[%d]�� weak_ptr�� ����Ű�� ��ü�� �����Ǿ��ų� displayList���� ���ŵǾ����ϴ�.", index); 
			}
			break;
		}
	}
};