/*made by KUR
 *
 *����
 * �Ű������� �ִ� �����ڸ� ���� Ŭ������ �̱��� ���� ����
 * �� �ѹ��� ������ �����ϸ� �ݵ�� �������� �ؾ߸� �Ѵ�.
 * ������ ����/�ڵ� ��� ����
 * ���� ������ �����Ѵ�.
 * �����ڸ� ���� ���� ������ ����Ŭ�������� ����Ѵ�.
 *
 *
 *�̱����� �����Ϸ���
 * 1. ����Ŭ������ Singleton<����Ŭ�����̸�>�� public ����ϰ��Ѵ�.
 * 
 *�����ڸ� ���� ������ �����Ϸ���
 * 1. ����Ŭ�������� �����ڸ� private���� �����.
 * 2. ����Ŭ������ friend class Singleton<����Ŭ�����̸�>;�� �����ش�
 *
 *����Ŭ������ �����ڰ� �Ű������� ���� ��
 * 1. ����Ŭ����::createInstance(����,Ŭ������,�Ű�,����) �̷��� ȣ��
 *
 *
 *���α׷��� ������ �����(http://gallog.dcinside.com/zepeh)�� �ҽ��� �����߽��ϴ�. 
 *�����մϴ�!
 */
#pragma once

#include <memory>
#include <cassert>
#include <xutility>
using std::unique_ptr;

template <class T>
class Singleton {
public:
	template<typename ...Args>
	static void createInstance(Args&&... args){
		if(instance == nullptr) {				
			instance.reset(new T( std::forward<Args>(args)... ));
		} else {
			std::_DEBUG_ERROR("�̱��� ��ü�� �ߺ������Ͽ���. ���ʿ��� createInstance ȣ���� �����϶�. ");
		}
	}

	static T& Instance(){
		assert(instance != nullptr  &&  "�̱��� ��ü�� �����Ǳ� ���� �����ߴ�.�̱��� ��ü�� �Լ� ȣ�� ������ �ٲ��");
		return *instance;
	}

	static void destroyInstance(){
		instance.reset();
	}

private:
	static unique_ptr<T> instance;

//uncopyable
protected:
	Singleton(){}
	~Singleton(){}
private:
	Singleton(const Singleton&);
	Singleton& operator=(const Singleton&);
	
};

template <class T>
unique_ptr<T> Singleton<T>::instance = nullptr;

