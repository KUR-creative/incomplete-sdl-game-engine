#pragma once

#include <cstdlib>

//TODO: MEC++을 보고 와서 하자. 이건 좀 아니야.

class Error {
public:
	bool isCritical;
	int menu;

	//isCritical == true: 예외 처리 이후 강제 종료.
	//menu == 0: catch문 등을 쓸 때를 위한 기본 생성자.
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