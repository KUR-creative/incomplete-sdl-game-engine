#pragma once
/*
*class Uncopyable		from Effective C++ third
* ����, ������Ի����� ������� Ŭ������ private ��ӽ�Ű�� �ȴ�.
*/
class Uncopyable {
protected:
	Uncopyable(){ ; }
	~Uncopyable(){ ; }
private:
	Uncopyable(const Uncopyable&);
	Uncopyable& operator=(const Uncopyable&);
};