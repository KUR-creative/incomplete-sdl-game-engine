#pragma once

#include <cstdlib>

//TODO: MEC++�� ���� �ͼ� ����. �̰� �� �ƴϾ�.

class Error {
public:
	bool isCritical;
	int menu;

	//isCritical == true: ���� ó�� ���� ���� ����.
	//menu == 0: catch�� ���� �� ���� ���� �⺻ ������.
	explicit Error(bool isCritical = false, int menu = 0)
		: isCritical(isCritical), menu(menu)
	{}

	void showErrorReason(){
		printErrorReason();
		if(isCritical) {
			printf("\n>>CRITICAL ERROR! abort() called. \n");
			abort();
		}
	}

private:
	virtual void printErrorReason() = 0;

};