//VS 메모리 누수 탐지
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Utilities.hpp"
#include <SDL.h>
#include <SDL_image.h>

#include <gtest\gtest.h>
#pragma comment( lib, "gtest_maind.lib" )
#pragma comment( lib, "gtestd.lib" )

#include <io.h>
#include <stdio.h>
#include <cstring>
#include <direct.h>	//windows 종속, 그런데 아래 있는 함수는 POSIX인 듯?

#include <string>
#include <vector>
#include <memory>

#include "FileLoader.hpp"
#include "TxtLoader.hpp"
#include "PngLoader.hpp"
using namespace std;

#include <fstream>

#include <boost\archive\binary_iarchive.hpp>
#include <boost\archive\binary_oarchive.hpp>
#include <boost\serialization\vector.hpp>

#include "Projector.hpp"
#include "ResourceLibrary.hpp"
#include "ResourceLoadCaller.hpp"

//타입 진단용 템플릿
template<typename T>
class TD;

int main(int argc, char* argv[])
{
	//CRT 메모리릭 디버깅을 위한 초기화
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	cout << "언제 나오냐?" << '\n';

	testing::InitGoogleTest(&argc, argv);

	//SDL2, SDL2extension initialization
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		logSDLError("SDL_Init");
		return 1;
	}
	if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		logSDLError("IMG_Init");
		SDL_Quit();
		return 1;
	}

	
	//전역 싱글톤들 초기화(서로 의존관계가 있어서 createInstance의 순서를 반드시 지켜야 한다.)
	
	//조각나있는 초기화시 예외처리를 try...catch문과 예외처리 클래스를 이용해서
	//한 곳에 모아둘수도 있다.

	//메모리 누수 테스트는 가장 마지막에 하자.
	//_CrtSetBreakAlloc(718);

	// 메모리! 메모리를 보자!!!
	_CrtDumpMemoryLeaks();

	//연역되는 타잎 체크
	//TD<decltype(&SDL_DestroyWindow)> wut;
	//TD<decltype(SDL_DestroyWindow)> wuth;
	
	return RUN_ALL_TESTS();
}

/*//크리티컬 에러
#include "Error.hpp"
class testCritical: public ErrorCritical < int > {
public:
	testCritical(int a): ErrorCritical(a){}
private:
	void showErrorReason() override{
		cout << "hahahah: " << value;
	}
};

TEST(예외처리테스트, 크리티컬에러){
	try {
		testCritical a(10);
		throw a;
	}
	catch (testCritical& err) {
		err.showReasonAndQuit();
	}
}
*/

TEST(초기화, 싱글톤_초기화){
	//서로 의존하기 때문에 반드시 순서를 지켜야 한다.
	//나중에 초기화 클래스에서 캡슐화할 것이다.
	Projector::createInstance("통합 테스트: test game", 300, 30, 1000, 1000);
	ResourceLibrary::createInstance();
	ResourceLoadCaller::createInstance(RES_LIB);
}

TEST(싱글톤_단위테스트, Projector){
	//프로젝터는 SDL_Window와 SDL_Renderer을 가지는 싱글톤 객체이다.
	Projector::Instance();
	shared_ptr<SDL_Renderer> renderer = Projector::Instance().Renderer();
	ASSERT_TRUE(renderer);
	ASSERT_TRUE(PROJECTOR.Renderer());
	//Projector proj;
}


//부스트 직렬화 라이브러리 학습_테스트
class TestSerialBuffer {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & strVec;
	}

	vector<string> strVec;

public:
	TestSerialBuffer(char output){
		strVec = { "a", "ab", "abc", "abcd", "a b c d e", "abc def", "가나다라", "갓뎀 애쓰 홀 마더뻐커" };
	}
	TestSerialBuffer(int input){}
	
	void printArr(){
		for(auto item : strVec) {
			cout << item.c_str() << '\n';
		}
		cout << endl;
	}

	vector<string>& StrVec(){
		return strVec;
	}
};

TEST(boost라이브러리, Serialization학습_테스트){
	TestSerialBuffer outputBuf('o');
	TestSerialBuffer inputBuf(1);
	
	ofstream fout("testStrVec.bin", ios::binary);
	boost::archive::binary_oarchive boarchive(fout);
	boarchive << outputBuf;
	fout.close();

	ifstream fin("testStrVec.bin", ios::binary);
	boost::archive::binary_iarchive biarchive(fin);
	biarchive >> inputBuf;
	fin.close();

	vector<string>& ovector = outputBuf.StrVec();
	vector<string>& ivector = inputBuf.StrVec();
	for(int i = 0; i < ovector.size(); i++) {
		ASSERT_STREQ(ovector[i].c_str(), ivector[i].c_str());
	}

	inputBuf.printArr();
}



/*
#include "CsvSaver.hpp"
// XXXSaver는 표준 파일 스트림을 이용하는게 답일 것이다.
TEST(FileSaver테스트, DirCsvSaver){
	const std::string basePath = SDL_GetBasePath();
	string path(basePath + "resource\\output.csv");
	string onlyDir(basePath + "resource\\");
	//경로를 주고 파일생성을 하려하면 어떻게 되는가??
	//생성이 된 경우의 테스트는?
	//생성이 되지 않은 경우는?
	string inputStr(
",,,ref times\
\nresources,sound,gun.mp3,1\
\n,,bang.mp3,2\
\n,,atk.mp3,10\
\n,png,dice.png,1\
\n,,man.png,1\
\n,,effect.png,100\
\n,txt,history.txt,3\
\n,,log.txt,2");

	//C표준의 fopen_s을 쓰고 fclose를 쓰면 진짜로 닫히긴 하지만..
	//윈도우만의 뭐시기가 적용이 안되서 출력 데이터를 고쳐야된다. 때려쳐!
	//SDL_RW류의 함수를 쓰면 exe종료전까지는 stream이 닫히지 않는거같다.
	//CsvSaver csvWritter;
	//bool temp = csvWritter.save(onlyDir, "output.csv", inputStr);
	//ASSERT_TRUE(temp);

	//결국 테스트를 하려면 
	//1. 아래를 주석쳐서 일단 output.csv를 생성하고
	//2. 2번째 실행에서 위의 내용만 주석을 쳐서 다시 실행해야한다... 
	SDL_RWops* csvTest = SDL_RWFromFile(path.c_str(), "r");
	
	char buf[256] = { 0, };
	if(csvTest) {
		SDL_RWread(csvTest, buf, sizeof(buf), 1);
	} else {
		logSDLError("SDL_RWops* open:csvTest");
	}
	
	string outCsvStr(buf);
	SDL_FreeRW(csvTest);

	ASSERT_STREQ(inputStr.c_str(), outCsvStr.c_str());
	//어차피 csv파일은 외부에서 엑셀로 보기위해 뽑는거긴 하지만..
}
*/



TEST(ResourceLibrary테스트, RLib생성_사용_해제){
	shared_ptr<Text> txt(new Text("Resource Library test"));
	
	ResourceLibrary& rl = RES_LIB;
	ASSERT_TRUE(rl.insert("key", txt));
	ASSERT_FALSE(rl.insert("key", txt));
	
	try {
		hash_key key("key"), nope("nope");
		ASSERT_TRUE(rl[Text(key)]);
		ASSERT_FALSE(rl[Text(nope)]);	
	}
	catch(ResLibError& err) {
		err.showErrorReason();
	}

	const std::string basePath = SDL_GetBasePath();
	SDL_Texture* tex = IMG_LoadTexture(PROJECTOR.Renderer().get(), (basePath + "resource\\" + "image.png").c_str());
	if(tex == nullptr) {
		logSDLError("LoadTexture");
	}
	shared_ptr<Png> ptex(new Png(tex));

	ASSERT_TRUE(rl.insert("png", ptex));
	hash_key png("png");
	ASSERT_TRUE(rl[Png(png)]);
	
}

TEST(ResourceLibrary테스트, fileLoader와_연결){
	const std::string basePath = SDL_GetBasePath();
	string path(basePath + "resource\\test.txt");

	ifstream fin(path);
	if(!fin) {
		cout << "파일 열기 실패!" << '\n';
	}
	string expStr;
	int c;
	while((c = fin.get()) != EOF) {
		expStr += c;
	}
	fin.close();

	TxtLoader tloader(path);
	FileLoader* floader = &tloader;
	shared_ptr<LoadedData> testStr = floader->load();

	ASSERT_STREQ(expStr.c_str(), static_cast<Text*>(testStr.get())->c_str());
	
	//이제 ResourceLibrary에 fileLoader에서 만든걸 넣으셈
	ResourceLibrary& rl = RES_LIB;
	try {
	
	}
	catch(ResLibError& err) {
		err.showErrorReason();
	}
	ASSERT_TRUE(rl.insert("key1", testStr));
	ASSERT_FALSE(rl.insert("key1", testStr)); 

	hash_key key("key1");
	hash_key key2("key2");
	try {
		ASSERT_TRUE(rl[Text(key)]);
		ASSERT_FALSE(rl[Text(key2)]);
		ASSERT_FALSE(rl[Text("key")]);
	}
	catch(ResLibError& err) {
		err.showErrorReason();
	}
	
	//EXPECT_FALSE(rl[Png(key)]); //다른 형식으로의 다운캐스팅을 막을수는 없다.

	string pathPng(basePath + "resource\\sad frog.png");
	PngLoader ploader(PROJECTOR.Renderer(), pathPng);
	floader = &ploader;
	
	unique_ptr<LoadedData> png(floader->load());
	shared_ptr<LoadedData> pngl(std::move(png));

	ASSERT_TRUE(rl.insert("pngk", pngl));
	hash_key pngk("pngk");
	ASSERT_TRUE( rl[Png(pngk)] );

	try {
		ASSERT_FALSE(rl[LoadedData(pngk)]);
	}
	catch(ResLibError& err) {
		err.showErrorReason();
	}

}


#include "IniSerialBuffer.hpp"
TEST(릴리즈용_리소스로더를_위한, ini이진_파일_만들기){
	string resourceDir = SDL_GetBasePath();
	resourceDir += "resource";

	IniSerialBuffer outputBuf(resourceDir.c_str());
	IniSerialBuffer inputBuf;

	ofstream fout("testIniSerialBuffer.ini", ios::binary);
	boost::archive::binary_oarchive boarchive(fout);
	boarchive << outputBuf;
	fout.close();

	ifstream fin("testIniSerialBuffer.ini", ios::binary);
	boost::archive::binary_iarchive biarchive(fin);
	biarchive >> inputBuf;
	fin.close();

	//serialization 테스트
	vector<string>& ovector = outputBuf.StrVec();
	vector<string>& ivector = inputBuf.StrVec();
	for(int i = 0; i < ovector.size(); i++) {
		ASSERT_STREQ(ovector[i].c_str(), ivector[i].c_str());
	}

	//생성 됬는지 출력.
	inputBuf.printArr();
}


TEST(파일로드콜러_테스트, 릴리즈){
	//일단 디버그 모드로 만들고, 나중에 매크로로 나눠준다.
	ResourceLibrary& resLib = RES_LIB;
	ResourceLoadCaller& resLoadCaller = RES_LOAD_CALLER;	
	//생성을 하면 안에서 자동으로 파일 로드하여 리소스라이브러리에 꽂아준다.
	//HitBox.as, smile.bmp, output.csv에 대한 예외를 콘솔에 출력하라.
	
	//제대로 들어는 갔니?
	hash_key ani("animation.png");
	hash_key full("full.png");
	hash_key test("test.txt");
	
	EXPECT_TRUE( resLib[Png(ani)] );
	EXPECT_TRUE(resLib[Png(full)]);
	EXPECT_TRUE(resLib[Text(test)]);

	//제대로 생성됬냐?
	string resourceDir = SDL_GetBasePath();
	resourceDir += "resource\\test.txt";
	ifstream fin(resourceDir);
	if(!fin) {
		cout << "파일 열기 실패!" << '\n';
	}
	
	string testStr;
	int c;
	while( (c = fin.get()) != EOF ) {
		testStr += c;
	}
	
	fin.close();
	
	//Txt 파일 확인
	ASSERT_STREQ(testStr.c_str(), resLib[Text(test)]->c_str());

	//Png 파일 확인
	SDL_Texture* tex1 = resLib[Png(full)]->Texture().get();
	SDL_Rect dst1;	dst1.x = 0;	dst1.y = 0;	dst1.w = 300;	dst1.h = 300;

	SDL_RenderCopy(Projector::Instance().Renderer().get(), tex1, NULL, &dst1);
	SDL_RenderPresent(Projector::Instance().Renderer().get());
	SDL_Delay(100);
}

//////SDL_ttf
#include <SDL_ttf.h>
#include <wchar.h>
SDL_Texture* renderText(const wstring& message,
						const string& fontFile,
						SDL_Color color,
						int fontSize,
						SDL_Renderer *renderer)
{
	//Open the font
	TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if(font == nullptr) {
		logSDLError("TTF_OpenFont");
		return nullptr;
	}
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderUNICODE_Solid(font, (Uint16*)message.c_str(), color);
	if(surf == nullptr) {
		TTF_CloseFont(font);
		logSDLError("TTF_RenderText");
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if(texture == nullptr) {
		logSDLError("CreateTexture");
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
}

#include <codecvt>
#include <cstdlib>
#include <atlconv.h>

TEST(학습테스트, SDL_ttf와_유니코드변환){
	//string을 wstring으로 바꾸는 함수는... 
	char* ass = "ass하하 욜로";

	wchar_t wassc[30] = { 0, };
	size_t length = strlen(ass);
	mbstowcs_s(&length, wassc, ass, length);
	wstring was(wassc);

	wstring msg(L"유니코드 만든 새끼들 애미는 이미 죽었겠지? 아! 내가병신이다");
	cout << "---";
	wcout << msg.c_str();
	//wstring msg = converter.from_bytes(rlText.getString()); char는 어떻게 wchar로 바꾸나?

	//이렇게 하면 char를 wchar_t로 바꿀 수 있다!
	//나중에 쓸 때, 제대로 알고 써라. .....
	string hang("한글이 안 나올><텐데?");
	wchar_t strUnicode[256] = { 0, };
	char	strMultibyte[256] = { 0, };
	strcpy_s(strMultibyte, 256, hang.c_str());
	int nLen = MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), strUnicode, nLen);
	wstring hangt(strUnicode);


	if(TTF_Init() != 0) {
		logSDLError("TTF_Init");
		SDL_Quit();
	}
	hash_key han("test.txt");
	Text rlText = *(RES_LIB[Text(han)]);
	cout << rlText.c_str();

	const std::string basePath = SDL_GetBasePath();
	const std::string resPath = basePath + "resource\\";

	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Texture* textImg = renderText(hangt, resPath + "HYGSRB.TTF", color, 28, PROJECTOR.Renderer().get());
	int iW, iH;
	SDL_QueryTexture(textImg, NULL, NULL, &iW, &iH);
	int x = 0;
	int y = 300;

	SDL_RenderClear(PROJECTOR.Renderer().get());
	SDL_Rect rect{ x, y, iW, iH };
	SDL_RenderCopy(PROJECTOR.Renderer().get(), textImg, nullptr, &rect);
	SDL_RenderPresent(PROJECTOR.Renderer().get());
}


#include "DisplayObject.hpp"
#include "SimpleSprite.hpp"

TEST(싱글톤의존_단위테스트, SimpleSprite){
	//Png로 만든다.
	//추상클래스 DisplayObject를 상속한다.
	hash_key key("bird.png");
	shared_ptr<Png> ppng = RES_LIB[Png(key)];
	DisplayObject* sims = new SimpleSprite(ppng, PROJECTOR.Renderer(), 600, 600, 300, 300, 90);
	//자기 자신을 렌더링할 수 있다.
	sims->renderCopy();
	PROJECTOR.renderPresent();

	SDL_Delay(1000);
	SDL_RenderClear(PROJECTOR.Renderer().get());

	/*
	
	//알파 수정을 할 수 있다. 
	SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);

	sims->Alpha(50);
	sims->renderCopy();
	PROJECTOR.renderPresent();

	SDL_Delay(1000);
	sims->Alpha(255);

	//r,g,b 변형을 할 수 있다.
	SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);
	sims->R(50);
	sims->renderCopy();
	PROJECTOR.renderPresent();
	SDL_Delay(1000);

	SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);
	sims->G(50);
	sims->renderCopy();
	PROJECTOR.renderPresent();
	SDL_Delay(1000);

	SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);
	sims->B(50);
	sims->renderCopy();
	PROJECTOR.renderPresent();
	SDL_Delay(1000);

	SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);
	sims->setRGB(255,255,255);
	sims->renderCopy();
	PROJECTOR.renderPresent();
	SDL_Delay(1000);

	SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);
	sims->Alpha(135);
	sims->renderCopy();
	PROJECTOR.renderPresent();
	SDL_Delay(1000);
	

	//x,y
	SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);
	sims->Alpha(255);
	sims->X(400);
	sims->renderCopy();
	PROJECTOR.renderPresent();
	SDL_Delay(1000);

	SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);
	sims->renderCopy();
	PROJECTOR.renderPresent();
	SDL_Delay(1000);

	SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);
	sims->renderCopy();
	PROJECTOR.renderPresent();
	SDL_Delay(1000);
	

	// w h
	SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);
	sims->Alpha(255);
	sims->W(500);
	sims->renderCopy();
	PROJECTOR.renderPresent();
	SDL_Delay(1000);

	SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);
	sims->Alpha(255);
	sims->H(500);
	sims->renderCopy();
	PROJECTOR.renderPresent();
	SDL_Delay(1000);

	SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);
	sims->Alpha(255);
	sims->WH(200,200);
	sims->renderCopy();
	PROJECTOR.renderPresent();
	SDL_Delay(1000);
	

	SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);
	sims->Alpha(255);
	sims->Angle(0.0);
	sims->WH(300, 300);
	sims->renderCopy();
	PROJECTOR.renderPresent();
	SDL_Delay(1000);

	//Flip test
	SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);
	sims->Alpha(255);
	sims->Angle(0.0);
	sims->Flip(SDL_FLIP_VERTICAL);
	sims->renderCopy();
	PROJECTOR.renderPresent();
	SDL_Delay(1000);

	

	// getter 테스트
	int x = 300;	int y = 100;
	int w = 300;	int h = 250;
	SDL_RendererFlip f = SDL_FLIP_HORIZONTAL;
	double angle = 135.0;
	Uint8 alpha = 200;
	Uint8 r = 150;	Uint8 g = 200;	Uint8 b = 10;

	//setting
	sims->XY(x, y);
	sims->WH(w, h);
	sims->Flip(f);
	sims->Angle(angle);
	sims->Alpha(alpha);
	sims->setRGB(r, g, b);
	sims->CenterXY(w/2,h/2);

	ASSERT_TRUE(sims->X() == x);
	ASSERT_TRUE(sims->Y() == y);
	ASSERT_TRUE(sims->W() == w);
	ASSERT_TRUE(sims->H() == h);
	ASSERT_TRUE(sims->Flip() == f);
	ASSERT_TRUE(sims->Angle() == angle);
	ASSERT_TRUE(sims->Alpha() == alpha);
	ASSERT_TRUE(sims->R() == r);
	ASSERT_TRUE(sims->G() == g);
	ASSERT_TRUE(sims->B() == b);
	ASSERT_TRUE(sims->CenterX() == w / 2);
	ASSERT_TRUE(sims->CenterY() == h / 2);

	//각도를 변형하여 회전시킬 수 있다
	for(int i = 0; i < 100; i++) {
		sims->renderCopy();
		PROJECTOR.renderPresent();
		sims->Angle(sims->Angle() + 10);
	}
	
	//회전 중심을 바꿀 수 있다.
	sims->CenterXY(0,0);
	for(int i = 0; i < 100; i++) {
		sims->renderCopy();
		PROJECTOR.renderPresent();
		sims->Angle(sims->Angle() + 10);
	}


	//DisplayObject 2개 이상 테스트: 다른 텍스쳐
	SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);
	sims->CenterXY(w/2,h/2);
	//sims->setRGB(255, 255, 255);
	sims->Alpha(255);
	sims->XY(100, 50);

	hash_key sad("sad frog.png");
	hash_key cod("codesafer.png");
	shared_ptr<Png> pfrog = RES_LIB[Png(sad)];
	shared_ptr<Png> pCode = RES_LIB[Png(cod)];
	DisplayObject* frog = new SimpleSprite(pfrog, PROJECTOR.Renderer(), 100, 250, 200, 200);
	DisplayObject* code = new SimpleSprite(pCode, PROJECTOR.Renderer(), 100, 500, 200, 200);

	SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);
	sims->renderCopy();
	frog->renderCopy();
	code->renderCopy();
	PROJECTOR.renderPresent();

	SDL_Delay(1000);

	//DisplayObject 2개 이상 테스트: 텍스쳐 공유
	DisplayObject* sims2 = new SimpleSprite(ppng, PROJECTOR.Renderer(), 400, 250, 200, 200);
	
	SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);
	sims->renderCopy();
	sims2->renderCopy();
	frog->renderCopy();
	code->renderCopy();
	PROJECTOR.renderPresent();

	SDL_Delay(1000);
	*/
	delete sims;
}

#include "DisplayObjectContainer.hpp"
#include "Sprite.hpp"

TEST(싱글톤의존_단위테스트, DisplayObjectContainer){
	//pDoc로 저장되어 있는걸 어떻게 addChild같은거 쓰냐?
	//어쩌긴! 걍 doc를 쓰는거지!
	try 
	{
		hash_key key("bird.png");
		shared_ptr<Png> pbird = RES_LIB[Png(key)];

		// DisplayObjectContainer는 DisplayObject를 상속한다.
		shared_ptr<DisplayObjectContainer> docBird( new Sprite(pbird,PROJECTOR.Renderer(), 0.5,0.5, 300,400, 200,400 ) );
		DisplayObject* pDoc = docBird.get();
		/*
		// DOC가 정상적으로 DO와 구분없이 렌더링이 되는지만 체크. 재귀적 렌더링은 나중에.
		SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);
		pDoc->renderCopy();
		PROJECTOR.renderPresent();
		SDL_Delay(1000);
		*/
		

		// 자식 DO 넣기.
		hash_key keyCh1("ch1.png");
		hash_key keyCh2("ch2.png");
		hash_key keyCh3("ch3.png");
		hash_key keyCh4("ch4.png");
		hash_key keyAng1("angle.png");
		hash_key keyAng2("angle.png");
		hash_key keyAl("alpha.png");
		shared_ptr<Png> pCh1 = RES_LIB[Png(keyCh1)];
		shared_ptr<Png> pCh2 = RES_LIB[Png(keyCh2)];
		shared_ptr<Png> pCh3 = RES_LIB[Png(keyCh3)];
		shared_ptr<Png> pCh4 = RES_LIB[Png(keyCh4)];
		shared_ptr<Png> pAng1 = RES_LIB[Png(keyAng1)];
		shared_ptr<Png> pAng2 = RES_LIB[Png(keyAng2)];
		shared_ptr<Png> pAlp = RES_LIB[Png(keyAl)];
		shared_ptr<DisplayObjectContainer> docCh1( new Sprite(pCh1, PROJECTOR.Renderer(), 0.5,0.5) );
		shared_ptr<DisplayObjectContainer> docCh2( new Sprite(pCh2, PROJECTOR.Renderer(), 0.0,0.0) );
		shared_ptr<DisplayObjectContainer> docCh3( new Sprite(pCh3, PROJECTOR.Renderer(), -0.5,-0.5, 50,100, 50,50) );

		shared_ptr<DisplayObjectContainer> docAl1( new Sprite(pAlp, PROJECTOR.Renderer(), 0,0, 0,0, 150,150) );
		shared_ptr<DisplayObjectContainer> docAl2( new Sprite(pAlp, PROJECTOR.Renderer(), 0.5,0.5, 150,150, 100,100) );
		shared_ptr<DisplayObjectContainer> docAl3( new Sprite(pAlp, PROJECTOR.Renderer(), 0.5,0.5, 150,150, 100,100) );
		shared_ptr<DisplayObjectContainer> docAl4( new Sprite(pAlp, PROJECTOR.Renderer(), 0.5,0.5, 150,150, 100,100) );
		//----
		shared_ptr<DisplayObjectContainer> docCh4( new Sprite(pCh4, PROJECTOR.Renderer(), 170,170) );
		shared_ptr<DisplayObjectContainer> docAng1( new Sprite(pAng1, PROJECTOR.Renderer(), 0.5,0.5) );///
		shared_ptr<DisplayObject> docAng2( new Sprite(pAng1, PROJECTOR.Renderer(), 0.5, 0.5) );
		shared_ptr<DisplayObject> doc_ch3( new Sprite(pCh3, PROJECTOR.Renderer(), 0, 0) );
		shared_ptr<DisplayObject> doc_ch4( new Sprite(pCh4, PROJECTOR.Renderer(), 0, 0) );
		
		docBird->addChild(docCh1);	//addChild는 list에 호출된 순서대로 자식을 넣는다.
		docCh1->addChild(docCh2);
		docCh2->addChild(docCh3);
		docBird->addChild(docAl1);
		docAl1->addChild(docAl2);
		//docBird->addChild(docCh3);
		//docBird->addChild(docCh4, 300, 300);
		//docCh2->addChild(docAng1);
		//docCh2->addChild(docAng2);
		//docAng1->addChild(doc_ch3);
		//docAng1->addChild(doc_ch4);

		//public화 테스트들 - TODO: 나중에 주석칠것.
		SDL_RenderClear(PROJECTOR.Renderer().get());
		docBird->renderCopy();
		PROJECTOR.renderPresent();
		/*
		printf("회전변환 전");
		printf("\n\n---- docBird ----\n");
		EXPECT_EQ(200,docBird->renderRect.x);
		EXPECT_EQ(200,docBird->renderRect.y);
		EXPECT_EQ(200,docBird->renderRect.w);
		EXPECT_EQ(400,docBird->renderRect.h);

		EXPECT_EQ(100,docBird->renderCenter.x);
		EXPECT_EQ(200,docBird->renderCenter.y);

		EXPECT_EQ(2.0,docBird->renderRatioW);
		EXPECT_EQ(2.0,docBird->renderRatioH);

		EXPECT_EQ(0.5,docBird->centerRatioW);
		EXPECT_EQ(0.5,docBird->centerRatioH);

		EXPECT_EQ(300,docBird->centerForChild.x);
		EXPECT_EQ(400,docBird->centerForChild.y);

		printf("\n\n---- docCh1 ----\n");
		EXPECT_EQ(250,docCh1->renderRect.x);
		EXPECT_EQ(300,docCh1->renderRect.y);
		EXPECT_EQ(100,docCh1->renderRect.w);
		EXPECT_EQ(200,docCh1->renderRect.h);

		EXPECT_EQ(50,docCh1->renderCenter.x);
		EXPECT_EQ(100,docCh1->renderCenter.y);

		EXPECT_EQ(2.0,docCh1->renderRatioW);
		EXPECT_EQ(2.0,docCh1->renderRatioH);

		EXPECT_EQ(0.5,docCh1->centerRatioW);
		EXPECT_EQ(0.5,docCh1->centerRatioH);

		EXPECT_EQ(300,docCh1->centerForChild.x);
		EXPECT_EQ(400,docCh1->centerForChild.y);

		printf("\n\n---- docCh2 ----\n");
		EXPECT_EQ(300,docCh2->renderRect.x);
		EXPECT_EQ(400,docCh2->renderRect.y);
		EXPECT_EQ(100,docCh2->renderRect.w);
		EXPECT_EQ(100,docCh2->renderRect.h);

		EXPECT_EQ(0,docCh2->renderCenter.x);
		EXPECT_EQ(0,docCh2->renderCenter.y);

		EXPECT_EQ(2.0,docCh2->renderRatioW);
		EXPECT_EQ(2.0,docCh2->renderRatioH);

		EXPECT_EQ(0.0,docCh2->centerRatioW);
		EXPECT_EQ(0.0,docCh2->centerRatioH);

		EXPECT_EQ(300,docCh2->centerForChild.x);
		EXPECT_EQ(400,docCh2->centerForChild.y);

		printf("\n\n---- docCh3 ----\n");
		EXPECT_EQ(400, docCh3->renderRect.x);
		EXPECT_EQ(550, docCh3->renderRect.y);
		EXPECT_EQ(100, docCh3->renderRect.w);
		EXPECT_EQ(100, docCh3->renderRect.h);

		EXPECT_EQ(-50, docCh3->renderCenter.x);
		EXPECT_EQ(-50, docCh3->renderCenter.y);

		EXPECT_EQ(1.0, docCh3->renderRatioW);
		EXPECT_EQ(1.0, docCh3->renderRatioH);

		EXPECT_EQ(-0.5, docCh3->centerRatioW);
		EXPECT_EQ(-0.5, docCh3->centerRatioH);

		EXPECT_EQ(350, docCh3->centerForChild.x);
		EXPECT_EQ(500, docCh3->centerForChild.y);
		*/
		/*
		//다른 구조 테스트 root-자식-자식-자식... 구조
		//아래로 깊게 퍼지는 구조도 x,y,w,,h변화에서는 
		//옆으로 자식이 퍼지는 구조와 아무런 차이가 없어야한다.. 
		docBird->addChild(docCh1, 100, 100);
		docCh1->addChild(docCh2, 100, 100);

		docCh2->addChild(docCh3, 100, 100);
		docCh3->addChild(docCh4, 100, 100);
		*/
		//만약에 하나의 DO나 DOC를 두개 이상의 부모가 가지면 어떻게 되지?
		//가질 수 없게 해야지.
		//어떻게 체크한담?

		/*
		// 자식 DO를 list에서 꺼내보기
		shared_ptr<DisplayObject> returned( docBird->getChildAt(0) );
		SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);
		returned->renderCopy();
		PROJECTOR.renderPresent();
		ASSERT_TRUE(returned.get() == docCh1.get());
		returned.reset();
		
		// addChild는 list에 호출된 순서대로 자식을 넣는다.
		ASSERT_TRUE(docBird->getChildAt(0).get() == docCh1.get());
		ASSERT_TRUE(docBird->getChildAt(1).get() == docCh2.get());
		ASSERT_TRUE(docBird->getChildAt(2).get() == docCh3.get());
		ASSERT_TRUE(docBird->getChildAt(3).get() == docCh4.get());
		*/

		// DO에 재귀적 좌표계 도입(부모의 center(X,Y) = 자식의 (0,0))
		//1_(x,y)
		// 1.기본 addChild
		//ASSERT_TRUE(docCh2->X() == 0);
		//ASSERT_TRUE(docCh2->Y() == 0);
		// 2.좌표 주는 addChild
		ASSERT_TRUE(docAng1->X() == docAng1->X());
		ASSERT_TRUE(docAng2->Y() == docAng2->Y());
		// 3.부모.center를 원점으로 삼을 때 달라지는 좌표
		
		//2_(w,h)
		// 0.최초의 w,h는 png => SDL_Texture의 w,h다.
		int w2 = 200, w3 = 200;
		hash_key cod("codesafer.png");
		shared_ptr<Png> pcos = RES_LIB[Png(cod)];
		shared_ptr<DisplayObject> origin(new Sprite(pcos, PROJECTOR.Renderer(), 10,10));
		shared_ptr<DisplayObject> whchange(new Sprite(pcos, PROJECTOR.Renderer(), 100,100, 0,0, w2, w3));
		int _w, _h;
		SDL_QueryTexture(pcos->Texture().get(), nullptr, nullptr, &_w, &_h);
		ASSERT_TRUE( _w == origin->TextureW() );
		ASSERT_TRUE( _h == origin->TextureH() );
		// 1.DO가 최초의 w,h가 아닌 값을 쓰면 그 비율을 저장한다.
		ASSERT_TRUE( (double)w2 / (double)_w  ==  whchange->RatioW() );
		ASSERT_TRUE( origin->RatioH() == (double)_h / origin->H() );
		/*SDL_RenderClear(PROJECTOR.Renderer().get());
		origin->renderCopy();
		PROJECTOR.renderPresent();
		SDL_Delay(4000);
		*/

		/*
			//기하렌더링
			SDL_SetRenderDrawColor(PROJECTOR.Renderer().get(), 0xFF, 0x00, 0x00, 0x00);
		//SDL_RenderClear(PROJECTOR.Renderer().get());//전에 설정한 drawColor로 지운다.
		//SDL_SetRenderDrawColor(PROJECTOR.Renderer().get(), 0xFF, 0xFF, 0xFF, 0xFF);
		//전에 설정한 drawColor로 점을 찍는다.
		SDL_Rect fillrect = { 400, 400, 5, 5 };
		SDL_RenderFillRect(PROJECTOR.Renderer().get(), &fillrect);
		PROJECTOR.renderPresent();
		SDL_Delay(1000);
		SDL_SetRenderDrawColor(PROJECTOR.Renderer().get(), 0x00, 0x00, 0x00, 0x00);
		*/

		// 2.자식들은 그 비율에 따라 렌더링된다. 크기를 늘리면 자식들도 다같이 늘어나야한다.
		//   재귀적 변화도 같이 테스트...
		//자식변화는 자식의자식까지.
	
		for(int i = 0; i < 40; i++) {
			SDL_RenderClear(PROJECTOR.Renderer().get());
			docCh1->W(docCh1->W() - i);
			docBird->renderCopy();
			PROJECTOR.renderPresent();
		}
		for(int i = 0; i < 40; i++) {
			SDL_RenderClear(PROJECTOR.Renderer().get());
			docCh1->H(docCh1->H() - i);
			docBird->renderCopy();
			PROJECTOR.renderPresent();
		}
		for(int i = 0; i < 40; i++) {
			SDL_RenderClear(PROJECTOR.Renderer().get());
			docCh1->WH(docCh1->W() + i, docCh1->H() + i);
			docBird->renderCopy();
			PROJECTOR.renderPresent();
		}
		
		//루트변화
		for(int i = 0; i < 50; i++) {
			SDL_RenderClear(PROJECTOR.Renderer().get());
			docBird->W(docBird->W() - i);
			docBird->renderCopy();
			PROJECTOR.renderPresent();
		}	
		
		for(int i = 0; i < 50; i++) {
			SDL_RenderClear(PROJECTOR.Renderer().get());
			docBird->H(docBird->H() - i);
			docBird->renderCopy();
			PROJECTOR.renderPresent();
		}
		for(int i = 0; i < 50; i++) {
			SDL_RenderClear(PROJECTOR.Renderer().get());
			docBird->WH(docBird->W() + i, docBird->H() + i);
			docBird->renderCopy();
			PROJECTOR.renderPresent();
		}
		
		
		//3. 0.0이 아닌 angle로 초기화된 부모.
		// look & feel !!
		SDL_RenderClear(PROJECTOR.Renderer().get());
		docBird->renderCopy();
		PROJECTOR.renderPresent();
		//SDL_Delay(1000);
		//3.1 부모의 center가 바뀐 경우
		SDL_RenderClear(PROJECTOR.Renderer().get());
		docBird->renderCopy();
		PROJECTOR.renderPresent();
		//SDL_Delay(4000);
		
		
		//렌더링은 순서가있고/재귀적이다
		// 1.(list의 오름차순으로)들어간 순서대로 렌더링된다.
		SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);
		docBird->renderCopy();
		PROJECTOR.renderPresent();
		
		// 재귀적이고 동적인 변화
		//1. x,y
		
		for(int i = 0; i < 50; i++) {
			SDL_RenderClear(PROJECTOR.Renderer().get());
			docBird->X(docBird->X() - 1);
			docBird->renderCopy();
			PROJECTOR.renderPresent();
		}
		for(int i = 0; i < 50; i++) {
			SDL_RenderClear(PROJECTOR.Renderer().get());
			docBird->Y(docBird->Y() - 1);
			docBird->renderCopy();
			PROJECTOR.renderPresent();
		}		
		for(int i = 0; i < 40; i++) {
			SDL_RenderClear(PROJECTOR.Renderer().get());
			docBird->XY(docBird->X() - 1, docBird->Y() - 1);
			docBird->renderCopy();
			PROJECTOR.renderPresent();
		}
		
		
		//2. w,h(위에서 했음)
		//3. angle
		//docBird->Angle(45);
		
		for(int i = 0; i < 360; i++) {
			SDL_RenderClear(PROJECTOR.Renderer().get());
			docBird->Angle(docBird->Angle() + 1);
				docBird->WH(docBird->W() - 1, docBird->H() - 1);
			docBird->renderCopy();
			//docAng1->XY(docCh3->centerForChild.x, docCh3->centerForChild.y);	//원점이 정확하다....
			//docAng1->renderCopy();
			//docAng2->XY(docCh2->centerForChild.x, docCh2->centerForChild.y);
			//docAng2->renderCopy();
			PROJECTOR.renderPresent();
		}

		SDL_RenderClear(PROJECTOR.Renderer().get());
		docBird->renderCopy();
		//docAng1->XY(docCh3->centerForChild.x, docCh3->centerForChild.y);	//원점이 정확하다....
		//docAng1->renderCopy();
		//docAng2->XY(docCh2->centerForChild.x, docCh2->centerForChild.y);
		//docAng2->renderCopy();
		PROJECTOR.renderPresent();
		

		//docBird->WH(300,300);
		for(int i = 0; i < 360; i++) {
			SDL_RenderClear(PROJECTOR.Renderer().get());
			docCh1->Angle(docCh1->Angle() - 1);
			docBird->X(docBird->X() + 1);
			docBird->renderCopy();
			//docAng1->XY(docCh3->centerForChild.x, docCh3->centerForChild.y);
			//docAng1->renderCopy();
			//docAng2->XY(docCh2->centerForChild.x, docCh2->centerForChild.y);
			//docAng2->renderCopy();
			PROJECTOR.renderPresent();
		}

		for(int i = 0; i < 360; i++) {
			SDL_RenderClear(PROJECTOR.Renderer().get());
			docBird->Angle(docBird->Angle() + 1);
			docBird->WH(docBird->W() + 1, docBird->H() + 1);
			docBird->renderCopy();
			//docAng1->XY(docCh3->centerForChild.x, docCh3->centerForChild.y);	//원점이 정확하다....
			//docAng1->renderCopy();
			//docAng2->XY(docCh2->centerForChild.x, docCh2->centerForChild.y);
			//docAng2->renderCopy();
			PROJECTOR.renderPresent();
		}

		//각을 돌린 상태에서 변화도 테스트해보아야 한다.
		//90도에서 규모를 바꾸면 어떻게 되는가?
	
		//4. alpha
		//0~255값만 받아들인다. 범위를 벗어나면 0이나 255로 만든다.
		docAl1->Alpha(10000);
		ASSERT_EQ(255, docAl1->Alpha());
		docAl1->Alpha(-10000);
		ASSERT_EQ(0, docAl1->Alpha());
		//일반적인 값 대입
		docAl1->Alpha(100);
		ASSERT_EQ(100, docAl1->Alpha());
		//초기로 돌리기
		docAl1->Alpha(255);
		ASSERT_EQ(255, docAl1->Alpha());
		//자식까지 영향이 이어진다.(자식 더 추가하고 테스트도 써놓을 것)
			//renderAlpha추가하기... 그리고 rgb도 이거랑 비슷하게 하면 될듯.
		
	
		//5. r,g,b

		//삭제할 수 있다
		//

		// 인덱스를 넘어서는 경우 예외
		//docBird->getChildAt(1);
		// 인덱스의 weak_ptr이 가리키던 객체가 없거나 해제된 경우 예외
		//docCh1.reset();
		//docBird->getChildAt(0);
		// addChild(child)에서 nullptr을 가리키는 child를 넣은 경우.
		//shared_ptr<DisplayObject> noObj(nullptr);
		//docBird->addChild(noObj);
	}
	catch(ResLibError& err) {
		err.showErrorReason();
	}
	catch(DisplayObjectContainerError& err) {
		err.showErrorReason();
	}
	
	
	//iterater를 보관해서 빠른 접근이 가능케 하자.
	//자식 DO도 함께 렌더링

}

TEST(통합테스트, Projector와root노드){
	//나중에 해..
	//결국 DOC를 모아놓는 컨테이너는 Projector가 가지기로 했다.
	//어떤 자료구조를 쓸지는 글쎄? 
	//검색 안하고 삽입 삭제는 많고... forward_list가 어떤지.
}



/*
//버튼을 위한 임시 이벤트 실험
//이거에 옵저버패턴을 어떻게 적용시키면 될까?
//이게 글로벌 메시지시스템 클래스의 부분 아니냐?
//아니면 인풋시스템 클래스의 부분인가?
//주의. 무한 루프 있음.
TEST(연구및실험, 이벤트큐_처리루프){
	// main loop
	bool quit = false;
	SDL_Event e;

	while(!quit) {
		using namespace std;

		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				puts("닫기 누름!");
				quit = true;
			}
			if(e.type == SDL_KEYDOWN) {
				cout << "virtual key code	" << e.key.keysym.sym << endl;
				cout << "physical key code	" << e.key.keysym.scancode << endl;
				puts("키보드 누름!");
			}
			if(e.type == SDL_MOUSEBUTTONDOWN) {
				puts("마우스 누름!");
				quit = true;
			}
		}
	}
}

TEST(종결테스트, 싱글톤){
	PROJECTOR.destroyInstance();
}
*/