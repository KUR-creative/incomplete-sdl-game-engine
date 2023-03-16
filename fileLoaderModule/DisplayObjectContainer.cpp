#include "DisplayObjectContainer.hpp"

DisplayObjectContainer::DisplayObjectContainer(shared_ptr<SDL_Texture> tex,
											   shared_ptr<SDL_Renderer> ren,
											   double centerRatioW, double centerRatioH,
											   int x, int y,
											   int w, int h,
											   double angle,
											   Uint8 alpha,
											   Uint8 red, Uint8 green, Uint8 blue)
	:DisplayObject(tex, ren, centerRatioW, centerRatioH, x, y, w, h, angle, alpha, red, green, blue)
{}

void DisplayObjectContainer::renderCopy()
{
	//this
	DisplayObject::renderCopy();
	//children
	for(auto item : displayList) {
		shared_ptr<DisplayObject> displayObj = item.lock();
		if(displayObj != nullptr) {
			displayObj->renderCopy();
		}//nullptr�� ���� ��¿��? ������ ���ϴ°��� ��
	}
}

// push_back�� �ƴ� �ٸ� ������� ���� ȿ�����̰� �� �� �ְ���.
// �޸𸮸� ��â �Ҵ��� �� ������ ���� �ִ°ž�. �׷��� �װ� ���߿�..(�̸� ����ȭ�� ��)
void DisplayObjectContainer::addChild(weak_ptr<DisplayObject> child, int XinParent, int YinParent)
{
	auto pChild = child.lock();
	if(pChild) {
		pChild->XY(XinParent, YinParent);
		pChild->resetParent(this);
		displayList.push_back(child);
	} 
	else {
		DisplayObjectContainerError err(-1);
		throw err;
	}	
}
void DisplayObjectContainer::addChild(weak_ptr<DisplayObject> child)
{
	auto pChild = child.lock();
	if(pChild) {
		pChild->resetParent(this);
		displayList.push_back(child);
	} else {
		DisplayObjectContainerError err(-1);
		throw err;
	}
}

shared_ptr<DisplayObject> DisplayObjectContainer::getChildAt(int index)
{
	if(index < displayList.size()) {
		auto iter = displayList.begin();

		for(int i = 0; i < index; i++) {
			++iter;
		}

		auto result = iter->lock();
		if(result != nullptr) {
			return result;
		} else {
			DisplayObjectContainerError err(index);
			throw err;
		}
	} 
	else {
		DisplayObjectContainerError err(index, displayList.size());
		throw err;
	}	
}

//iterator�� ����� �����ϵ��� ����
//�̰� getChildAt�̶� ���� �Ȱ��� ������
shared_ptr<DisplayObject> DisplayObjectContainer::operator[](int index)
{
	if(index < displayList.size()) {
		auto iter = displayList.begin();

		for(int i = 0; i < index; i++) {
			++iter;
		}

		auto result = iter->lock();
		if(result == nullptr) {
			DisplayObjectContainerError err(index);
			throw err;
		} else {
			return result;
		}
	} else {
		DisplayObjectContainerError err(index, displayList.size());
		throw err;
	}
}

