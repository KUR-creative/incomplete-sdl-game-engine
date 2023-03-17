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
		}//nullptr일 때는 어쩔겨? 렌더링 안하는거지 뭐
	}
}

// push_back이 아닌 다른 방법으로 좀더 효율적이게 할 수 있겠지.
// 메모리를 왕창 할당해 둔 다음에 값만 넣는거야. 그러나 그건 나중에..(이른 최적화는 악)
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

//iterator를 멤버로 보관하도록 개선
//이거 getChildAt이랑 완전 똑같음 ㅋㅋㅋ
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

