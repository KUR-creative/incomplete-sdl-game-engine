#pragma once
/*
*class Uncopyable		from Effective C++ third
* 복사, 복사대입생성을 막고싶은 클래스에 private 상속시키면 된다.
*/
class Uncopyable {
protected:
	Uncopyable(){ ; }
	~Uncopyable(){ ; }
private:
	Uncopyable(const Uncopyable&);
	Uncopyable& operator=(const Uncopyable&);
};