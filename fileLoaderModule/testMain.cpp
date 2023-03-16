//VS �޸� ���� Ž��
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
#include <direct.h>	//windows ����, �׷��� �Ʒ� �ִ� �Լ��� POSIX�� ��?

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

//Ÿ�� ���ܿ� ���ø�
template<typename T>
class TD;

int main(int argc, char* argv[])
{
	//CRT �޸𸮸� ������� ���� �ʱ�ȭ
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	cout << "���� ������?" << '\n';

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

	
	//���� �̱���� �ʱ�ȭ(���� �������谡 �־ createInstance�� ������ �ݵ�� ���Ѿ� �Ѵ�.)
	
	//�������ִ� �ʱ�ȭ�� ����ó���� try...catch���� ����ó�� Ŭ������ �̿��ؼ�
	//�� ���� ��ƵѼ��� �ִ�.

	//�޸� ���� �׽�Ʈ�� ���� �������� ����.
	//_CrtSetBreakAlloc(718);

	// �޸�! �޸𸮸� ����!!!
	_CrtDumpMemoryLeaks();

	//�����Ǵ� Ÿ�� üũ
	//TD<decltype(&SDL_DestroyWindow)> wut;
	//TD<decltype(SDL_DestroyWindow)> wuth;
	
	return RUN_ALL_TESTS();
}

/*//ũ��Ƽ�� ����
#include "Error.hpp"
class testCritical: public ErrorCritical < int > {
public:
	testCritical(int a): ErrorCritical(a){}
private:
	void showErrorReason() override{
		cout << "hahahah: " << value;
	}
};

TEST(����ó���׽�Ʈ, ũ��Ƽ�ÿ���){
	try {
		testCritical a(10);
		throw a;
	}
	catch (testCritical& err) {
		err.showReasonAndQuit();
	}
}
*/

TEST(�ʱ�ȭ, �̱���_�ʱ�ȭ){
	//���� �����ϱ� ������ �ݵ�� ������ ���Ѿ� �Ѵ�.
	//���߿� �ʱ�ȭ Ŭ�������� ĸ��ȭ�� ���̴�.
	Projector::createInstance("���� �׽�Ʈ: test game", 300, 30, 1000, 1000);
	ResourceLibrary::createInstance();
	ResourceLoadCaller::createInstance(RES_LIB);
}

TEST(�̱���_�����׽�Ʈ, Projector){
	//�������ʹ� SDL_Window�� SDL_Renderer�� ������ �̱��� ��ü�̴�.
	Projector::Instance();
	shared_ptr<SDL_Renderer> renderer = Projector::Instance().Renderer();
	ASSERT_TRUE(renderer);
	ASSERT_TRUE(PROJECTOR.Renderer());
	//Projector proj;
}


//�ν�Ʈ ����ȭ ���̺귯�� �н�_�׽�Ʈ
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
		strVec = { "a", "ab", "abc", "abcd", "a b c d e", "abc def", "�����ٶ�", "���� �־� Ȧ ������Ŀ" };
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

TEST(boost���̺귯��, Serialization�н�_�׽�Ʈ){
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
// XXXSaver�� ǥ�� ���� ��Ʈ���� �̿��ϴ°� ���� ���̴�.
TEST(FileSaver�׽�Ʈ, DirCsvSaver){
	const std::string basePath = SDL_GetBasePath();
	string path(basePath + "resource\\output.csv");
	string onlyDir(basePath + "resource\\");
	//��θ� �ְ� ���ϻ����� �Ϸ��ϸ� ��� �Ǵ°�??
	//������ �� ����� �׽�Ʈ��?
	//������ ���� ���� ����?
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

	//Cǥ���� fopen_s�� ���� fclose�� ���� ��¥�� ������ ������..
	//�����츸�� ���ñⰡ ������ �ȵǼ� ��� �����͸� ���ľߵȴ�. ������!
	//SDL_RW���� �Լ��� ���� exe������������ stream�� ������ �ʴ°Ű���.
	//CsvSaver csvWritter;
	//bool temp = csvWritter.save(onlyDir, "output.csv", inputStr);
	//ASSERT_TRUE(temp);

	//�ᱹ �׽�Ʈ�� �Ϸ��� 
	//1. �Ʒ��� �ּ��ļ� �ϴ� output.csv�� �����ϰ�
	//2. 2��° ���࿡�� ���� ���븸 �ּ��� �ļ� �ٽ� �����ؾ��Ѵ�... 
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
	//������ csv������ �ܺο��� ������ �������� �̴°ű� ������..
}
*/



TEST(ResourceLibrary�׽�Ʈ, RLib����_���_����){
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

TEST(ResourceLibrary�׽�Ʈ, fileLoader��_����){
	const std::string basePath = SDL_GetBasePath();
	string path(basePath + "resource\\test.txt");

	ifstream fin(path);
	if(!fin) {
		cout << "���� ���� ����!" << '\n';
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
	
	//���� ResourceLibrary�� fileLoader���� ����� ������
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
	
	//EXPECT_FALSE(rl[Png(key)]); //�ٸ� ���������� �ٿ�ĳ������ �������� ����.

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
TEST(�������_���ҽ��δ���_����, ini����_����_�����){
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

	//serialization �׽�Ʈ
	vector<string>& ovector = outputBuf.StrVec();
	vector<string>& ivector = inputBuf.StrVec();
	for(int i = 0; i < ovector.size(); i++) {
		ASSERT_STREQ(ovector[i].c_str(), ivector[i].c_str());
	}

	//���� ����� ���.
	inputBuf.printArr();
}


TEST(���Ϸε��ݷ�_�׽�Ʈ, ������){
	//�ϴ� ����� ���� �����, ���߿� ��ũ�η� �����ش�.
	ResourceLibrary& resLib = RES_LIB;
	ResourceLoadCaller& resLoadCaller = RES_LOAD_CALLER;	
	//������ �ϸ� �ȿ��� �ڵ����� ���� �ε��Ͽ� ���ҽ����̺귯���� �Ⱦ��ش�.
	//HitBox.as, smile.bmp, output.csv�� ���� ���ܸ� �ֿܼ� ����϶�.
	
	//����� ���� ����?
	hash_key ani("animation.png");
	hash_key full("full.png");
	hash_key test("test.txt");
	
	EXPECT_TRUE( resLib[Png(ani)] );
	EXPECT_TRUE(resLib[Png(full)]);
	EXPECT_TRUE(resLib[Text(test)]);

	//����� �������?
	string resourceDir = SDL_GetBasePath();
	resourceDir += "resource\\test.txt";
	ifstream fin(resourceDir);
	if(!fin) {
		cout << "���� ���� ����!" << '\n';
	}
	
	string testStr;
	int c;
	while( (c = fin.get()) != EOF ) {
		testStr += c;
	}
	
	fin.close();
	
	//Txt ���� Ȯ��
	ASSERT_STREQ(testStr.c_str(), resLib[Text(test)]->c_str());

	//Png ���� Ȯ��
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

TEST(�н��׽�Ʈ, SDL_ttf��_�����ڵ庯ȯ){
	//string�� wstring���� �ٲٴ� �Լ���... 
	char* ass = "ass���� ���";

	wchar_t wassc[30] = { 0, };
	size_t length = strlen(ass);
	mbstowcs_s(&length, wassc, ass, length);
	wstring was(wassc);

	wstring msg(L"�����ڵ� ���� ������ �̴ֹ� �̹� �׾�����? ��! ���������̴�");
	cout << "---";
	wcout << msg.c_str();
	//wstring msg = converter.from_bytes(rlText.getString()); char�� ��� wchar�� �ٲٳ�?

	//�̷��� �ϸ� char�� wchar_t�� �ٲ� �� �ִ�!
	//���߿� �� ��, ����� �˰� ���. .....
	string hang("�ѱ��� �� ����><�ٵ�?");
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

TEST(�̱�������_�����׽�Ʈ, SimpleSprite){
	//Png�� �����.
	//�߻�Ŭ���� DisplayObject�� ����Ѵ�.
	hash_key key("bird.png");
	shared_ptr<Png> ppng = RES_LIB[Png(key)];
	DisplayObject* sims = new SimpleSprite(ppng, PROJECTOR.Renderer(), 600, 600, 300, 300, 90);
	//�ڱ� �ڽ��� �������� �� �ִ�.
	sims->renderCopy();
	PROJECTOR.renderPresent();

	SDL_Delay(1000);
	SDL_RenderClear(PROJECTOR.Renderer().get());

	/*
	
	//���� ������ �� �� �ִ�. 
	SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);

	sims->Alpha(50);
	sims->renderCopy();
	PROJECTOR.renderPresent();

	SDL_Delay(1000);
	sims->Alpha(255);

	//r,g,b ������ �� �� �ִ�.
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

	

	// getter �׽�Ʈ
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

	//������ �����Ͽ� ȸ����ų �� �ִ�
	for(int i = 0; i < 100; i++) {
		sims->renderCopy();
		PROJECTOR.renderPresent();
		sims->Angle(sims->Angle() + 10);
	}
	
	//ȸ�� �߽��� �ٲ� �� �ִ�.
	sims->CenterXY(0,0);
	for(int i = 0; i < 100; i++) {
		sims->renderCopy();
		PROJECTOR.renderPresent();
		sims->Angle(sims->Angle() + 10);
	}


	//DisplayObject 2�� �̻� �׽�Ʈ: �ٸ� �ؽ���
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

	//DisplayObject 2�� �̻� �׽�Ʈ: �ؽ��� ����
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

TEST(�̱�������_�����׽�Ʈ, DisplayObjectContainer){
	//pDoc�� ����Ǿ� �ִ°� ��� addChild������ ����?
	//��¼��! �� doc�� ���°���!
	try 
	{
		hash_key key("bird.png");
		shared_ptr<Png> pbird = RES_LIB[Png(key)];

		// DisplayObjectContainer�� DisplayObject�� ����Ѵ�.
		shared_ptr<DisplayObjectContainer> docBird( new Sprite(pbird,PROJECTOR.Renderer(), 0.5,0.5, 300,400, 200,400 ) );
		DisplayObject* pDoc = docBird.get();
		/*
		// DOC�� ���������� DO�� ���о��� �������� �Ǵ����� üũ. ����� �������� ���߿�.
		SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);
		pDoc->renderCopy();
		PROJECTOR.renderPresent();
		SDL_Delay(1000);
		*/
		

		// �ڽ� DO �ֱ�.
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
		
		docBird->addChild(docCh1);	//addChild�� list�� ȣ��� ������� �ڽ��� �ִ´�.
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

		//publicȭ �׽�Ʈ�� - TODO: ���߿� �ּ�ĥ��.
		SDL_RenderClear(PROJECTOR.Renderer().get());
		docBird->renderCopy();
		PROJECTOR.renderPresent();
		/*
		printf("ȸ����ȯ ��");
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
		//�ٸ� ���� �׽�Ʈ root-�ڽ�-�ڽ�-�ڽ�... ����
		//�Ʒ��� ��� ������ ������ x,y,w,,h��ȭ������ 
		//������ �ڽ��� ������ ������ �ƹ��� ���̰� ������Ѵ�.. 
		docBird->addChild(docCh1, 100, 100);
		docCh1->addChild(docCh2, 100, 100);

		docCh2->addChild(docCh3, 100, 100);
		docCh3->addChild(docCh4, 100, 100);
		*/
		//���࿡ �ϳ��� DO�� DOC�� �ΰ� �̻��� �θ� ������ ��� ����?
		//���� �� ���� �ؾ���.
		//��� üũ�Ѵ�?

		/*
		// �ڽ� DO�� list���� ��������
		shared_ptr<DisplayObject> returned( docBird->getChildAt(0) );
		SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);
		returned->renderCopy();
		PROJECTOR.renderPresent();
		ASSERT_TRUE(returned.get() == docCh1.get());
		returned.reset();
		
		// addChild�� list�� ȣ��� ������� �ڽ��� �ִ´�.
		ASSERT_TRUE(docBird->getChildAt(0).get() == docCh1.get());
		ASSERT_TRUE(docBird->getChildAt(1).get() == docCh2.get());
		ASSERT_TRUE(docBird->getChildAt(2).get() == docCh3.get());
		ASSERT_TRUE(docBird->getChildAt(3).get() == docCh4.get());
		*/

		// DO�� ����� ��ǥ�� ����(�θ��� center(X,Y) = �ڽ��� (0,0))
		//1_(x,y)
		// 1.�⺻ addChild
		//ASSERT_TRUE(docCh2->X() == 0);
		//ASSERT_TRUE(docCh2->Y() == 0);
		// 2.��ǥ �ִ� addChild
		ASSERT_TRUE(docAng1->X() == docAng1->X());
		ASSERT_TRUE(docAng2->Y() == docAng2->Y());
		// 3.�θ�.center�� �������� ���� �� �޶����� ��ǥ
		
		//2_(w,h)
		// 0.������ w,h�� png => SDL_Texture�� w,h��.
		int w2 = 200, w3 = 200;
		hash_key cod("codesafer.png");
		shared_ptr<Png> pcos = RES_LIB[Png(cod)];
		shared_ptr<DisplayObject> origin(new Sprite(pcos, PROJECTOR.Renderer(), 10,10));
		shared_ptr<DisplayObject> whchange(new Sprite(pcos, PROJECTOR.Renderer(), 100,100, 0,0, w2, w3));
		int _w, _h;
		SDL_QueryTexture(pcos->Texture().get(), nullptr, nullptr, &_w, &_h);
		ASSERT_TRUE( _w == origin->TextureW() );
		ASSERT_TRUE( _h == origin->TextureH() );
		// 1.DO�� ������ w,h�� �ƴ� ���� ���� �� ������ �����Ѵ�.
		ASSERT_TRUE( (double)w2 / (double)_w  ==  whchange->RatioW() );
		ASSERT_TRUE( origin->RatioH() == (double)_h / origin->H() );
		/*SDL_RenderClear(PROJECTOR.Renderer().get());
		origin->renderCopy();
		PROJECTOR.renderPresent();
		SDL_Delay(4000);
		*/

		/*
			//���Ϸ�����
			SDL_SetRenderDrawColor(PROJECTOR.Renderer().get(), 0xFF, 0x00, 0x00, 0x00);
		//SDL_RenderClear(PROJECTOR.Renderer().get());//���� ������ drawColor�� �����.
		//SDL_SetRenderDrawColor(PROJECTOR.Renderer().get(), 0xFF, 0xFF, 0xFF, 0xFF);
		//���� ������ drawColor�� ���� ��´�.
		SDL_Rect fillrect = { 400, 400, 5, 5 };
		SDL_RenderFillRect(PROJECTOR.Renderer().get(), &fillrect);
		PROJECTOR.renderPresent();
		SDL_Delay(1000);
		SDL_SetRenderDrawColor(PROJECTOR.Renderer().get(), 0x00, 0x00, 0x00, 0x00);
		*/

		// 2.�ڽĵ��� �� ������ ���� �������ȴ�. ũ�⸦ �ø��� �ڽĵ鵵 �ٰ��� �þ���Ѵ�.
		//   ����� ��ȭ�� ���� �׽�Ʈ...
		//�ڽĺ�ȭ�� �ڽ����ڽı���.
	
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
		
		//��Ʈ��ȭ
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
		
		
		//3. 0.0�� �ƴ� angle�� �ʱ�ȭ�� �θ�.
		// look & feel !!
		SDL_RenderClear(PROJECTOR.Renderer().get());
		docBird->renderCopy();
		PROJECTOR.renderPresent();
		//SDL_Delay(1000);
		//3.1 �θ��� center�� �ٲ� ���
		SDL_RenderClear(PROJECTOR.Renderer().get());
		docBird->renderCopy();
		PROJECTOR.renderPresent();
		//SDL_Delay(4000);
		
		
		//�������� �������ְ�/������̴�
		// 1.(list�� ������������)�� ������� �������ȴ�.
		SDL_RenderClear(PROJECTOR.Renderer().get());	PROJECTOR.renderPresent();	SDL_Delay(200);
		docBird->renderCopy();
		PROJECTOR.renderPresent();
		
		// ������̰� ������ ��ȭ
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
		
		
		//2. w,h(������ ����)
		//3. angle
		//docBird->Angle(45);
		
		for(int i = 0; i < 360; i++) {
			SDL_RenderClear(PROJECTOR.Renderer().get());
			docBird->Angle(docBird->Angle() + 1);
				docBird->WH(docBird->W() - 1, docBird->H() - 1);
			docBird->renderCopy();
			//docAng1->XY(docCh3->centerForChild.x, docCh3->centerForChild.y);	//������ ��Ȯ�ϴ�....
			//docAng1->renderCopy();
			//docAng2->XY(docCh2->centerForChild.x, docCh2->centerForChild.y);
			//docAng2->renderCopy();
			PROJECTOR.renderPresent();
		}

		SDL_RenderClear(PROJECTOR.Renderer().get());
		docBird->renderCopy();
		//docAng1->XY(docCh3->centerForChild.x, docCh3->centerForChild.y);	//������ ��Ȯ�ϴ�....
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
			//docAng1->XY(docCh3->centerForChild.x, docCh3->centerForChild.y);	//������ ��Ȯ�ϴ�....
			//docAng1->renderCopy();
			//docAng2->XY(docCh2->centerForChild.x, docCh2->centerForChild.y);
			//docAng2->renderCopy();
			PROJECTOR.renderPresent();
		}

		//���� ���� ���¿��� ��ȭ�� �׽�Ʈ�غ��ƾ� �Ѵ�.
		//90������ �Ը� �ٲٸ� ��� �Ǵ°�?
	
		//4. alpha
		//0~255���� �޾Ƶ��δ�. ������ ����� 0�̳� 255�� �����.
		docAl1->Alpha(10000);
		ASSERT_EQ(255, docAl1->Alpha());
		docAl1->Alpha(-10000);
		ASSERT_EQ(0, docAl1->Alpha());
		//�Ϲ����� �� ����
		docAl1->Alpha(100);
		ASSERT_EQ(100, docAl1->Alpha());
		//�ʱ�� ������
		docAl1->Alpha(255);
		ASSERT_EQ(255, docAl1->Alpha());
		//�ڽı��� ������ �̾�����.(�ڽ� �� �߰��ϰ� �׽�Ʈ�� ����� ��)
			//renderAlpha�߰��ϱ�... �׸��� rgb�� �̰Ŷ� ����ϰ� �ϸ� �ɵ�.
		
	
		//5. r,g,b

		//������ �� �ִ�
		//

		// �ε����� �Ѿ�� ��� ����
		//docBird->getChildAt(1);
		// �ε����� weak_ptr�� ����Ű�� ��ü�� ���ų� ������ ��� ����
		//docCh1.reset();
		//docBird->getChildAt(0);
		// addChild(child)���� nullptr�� ����Ű�� child�� ���� ���.
		//shared_ptr<DisplayObject> noObj(nullptr);
		//docBird->addChild(noObj);
	}
	catch(ResLibError& err) {
		err.showErrorReason();
	}
	catch(DisplayObjectContainerError& err) {
		err.showErrorReason();
	}
	
	
	//iterater�� �����ؼ� ���� ������ ������ ����.
	//�ڽ� DO�� �Բ� ������

}

TEST(�����׽�Ʈ, Projector��root���){
	//���߿� ��..
	//�ᱹ DOC�� ��Ƴ��� �����̳ʴ� Projector�� ������� �ߴ�.
	//� �ڷᱸ���� ������ �۽�? 
	//�˻� ���ϰ� ���� ������ ����... forward_list�� ���.
}



/*
//��ư�� ���� �ӽ� �̺�Ʈ ����
//�̰ſ� ������������ ��� �����Ű�� �ɱ�?
//�̰� �۷ι� �޽����ý��� Ŭ������ �κ� �ƴϳ�?
//�ƴϸ� ��ǲ�ý��� Ŭ������ �κ��ΰ�?
//����. ���� ���� ����.
TEST(�����׽���, �̺�Ʈť_ó������){
	// main loop
	bool quit = false;
	SDL_Event e;

	while(!quit) {
		using namespace std;

		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				puts("�ݱ� ����!");
				quit = true;
			}
			if(e.type == SDL_KEYDOWN) {
				cout << "virtual key code	" << e.key.keysym.sym << endl;
				cout << "physical key code	" << e.key.keysym.scancode << endl;
				puts("Ű���� ����!");
			}
			if(e.type == SDL_MOUSEBUTTONDOWN) {
				puts("���콺 ����!");
				quit = true;
			}
		}
	}
}

TEST(�����׽�Ʈ, �̱���){
	PROJECTOR.destroyInstance();
}
*/