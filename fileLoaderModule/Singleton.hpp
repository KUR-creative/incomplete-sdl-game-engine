/*made by KUR
 *
 *스펙
 * 매개변수가 있는 생성자를 가진 클래스도 싱글톤 적용 가능
 * 단 한번의 생성을 보장하며 반드시 수동으로 해야만 한다.
 * 삭제는 수동/자동 모두 가능
 * 복사 생성은 제한한다.
 * 생성자를 통한 생성 제한은 서브클래스에서 담당한다.
 *
 *
 *싱글톤을 적용하려면
 * 1. 서브클래스가 Singleton<서브클래스이름>을 public 상속하게한다.
 * 
 *생성자를 통한 생성을 제한하려면
 * 1. 서브클래스에서 생성자를 private으로 만든다.
 * 2. 서브클래스에 friend class Singleton<서브클래스이름>;을 적어준다
 *
 *서브클래스의 생성자가 매개변수를 가질 때
 * 1. 서브클래스::createInstance(서브,클래스의,매개,변수) 이렇게 호출
 *
 *
 *프로그래밍 갤러리 제페님(http://gallog.dcinside.com/zepeh)의 소스를 참고했습니다. 
 *감사합니다!
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
			std::_DEBUG_ERROR("싱글톤 객체를 중복생성하였다. 불필요한 createInstance 호출을 삭제하라. ");
		}
	}

	static T& Instance(){
		assert(instance != nullptr  &&  "싱글톤 객체가 생성되기 전에 참조했다.싱글톤 객체의 함수 호출 순서를 바꿔라");
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

