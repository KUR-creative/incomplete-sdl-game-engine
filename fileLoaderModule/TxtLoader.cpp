#include <fstream>
#include <SDL.h>
#include "TxtLoader.hpp"
#include "Utilities.hpp"


unique_ptr<LoadedData> TxtLoader::load(){
	std::ifstream fin(getFilePath());
	if(!fin) {
		std::cout << "파일 열기 실패!" << '\n';
		if(isPathStrEmpty()) {
			std::cout << "경로 문자열이 비어있습니다. 경로없는 TxtLoader생성이후 경로를 넣지 않고 파일을 로드했을 가능성이 큽니다." << '\n';
		}
		return nullptr;
	}

	string buf;
	int c;	//TODO : 이거 해주는 표준함수 없냐...?
	while((c = fin.get()) != EOF) {
		buf += c;
	}
	fin.close();

	return std::make_unique<Text>(buf);
}


#include <gtest\gtest.h>

TEST(단위테스트, TxtLoader){
	//표준으로 파일 로드.
	const std::string basePath = SDL_GetBasePath();
	string path(basePath + "resource\\test.txt");

	std::ifstream fin(path);
	if(!fin) {
		std::cout << "파일 열기 실패!" << '\n';
	}

	string file;
	int c;
	while((c = fin.get()) != EOF) {
		file += c;
	}
	fin.close();

	//TxtLoader를 이용한 파일로드
	TxtLoader txtLoader(path);	//const string& path
	FileLoader* fLoader = &txtLoader;
	unique_ptr<LoadedData> testText(fLoader->load());

	if(testText) {
		EXPECT_TRUE(testText.get());

		//파일 오픈에 성공한 경우
		//이제 txt를 string으로 메모리에 올려보자
		ASSERT_STREQ(file.c_str(), static_cast<Text*>(testText.get())->c_str());
	} else { //파일 오픈에 실패한 경우의 예외처리
		ASSERT_FALSE(testText);
	}
}

TEST(단위테스트, 경로없는TxtLoader생성){
	TxtLoader txtLoader;
	unique_ptr<LoadedData> testText(txtLoader.load());
	ASSERT_FALSE(testText.get());	//경로가 없다면 파일로드 실패->nullptr반환
	
	//경로 넣기
	const std::string basePath = SDL_GetBasePath();
	string path(basePath + "resource\\test.txt");
	txtLoader.setPathStr(path);
	testText = txtLoader.load();
	ASSERT_TRUE(testText.get());	//경로가 있다면 파일로드 가능.

	//로더를 사용하지 않은 파일로드와 비교
	std::ifstream fin(path);
	if(!fin) {
		std::cout << "파일 열기 실패!" << '\n';
	}
	string noLoader;
	int c;
	while((c = fin.get()) != EOF) {
		noLoader += c;
	}
	fin.close();

	ASSERT_STREQ(noLoader.c_str(), static_cast<Text*>(testText.get())->c_str() );

	

}