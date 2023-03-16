#include <fstream>
#include "ResourceLoadCaller.hpp"
#include "IniSerialBuffer.hpp"
#include "TxtLoader.hpp"
#include "PngLoader.hpp"
using std::make_shared;
using std::vector;

#include "Projector.hpp"

ResourceLoadCaller::ResourceLoadCaller(ResourceLibrary& resLib)
	:resLib(resLib)
{
	initExtensionMap();
	/* 또 테스트 
	if(extensionMap["png"]) {
		std::cout << "성공: 들어갔다" << '\n';
	} else {
		std::cout << "실패: 안 들어감" << '\n';
	}

	if(extensionMap["txt"]) {
		std::cout << "성공: 들어갔다" << '\n';
	} else {
		std::cout << "실패: 안 들어감" << '\n';
	}

	if(extensionMap["확장자아녀"]) {
		std::cout << "실패: 들어갔다" << '\n';
	} else {
		std::cout << "성공: 안 들어감" << '\n';
	}
	*/
	
	insertResources();
	/*임시적인 함수 테스트 
	std::cout << '\n' << "================================" << '\n';
	string testNot("not extension");
	if(getFileExtension(testNot).size() == 0) {
		std::cout << "성공!" << '\n';
	} else {
		std::cout << "실패!" << '\n';
	}

	string testNot2("not ext\\ension");
	if(getFileExtension(testNot2).size() == 0) {
		std::cout << "성공!" << '\n';
	} else {
		std::cout << "실패!" << '\n';
	}

	string testYes("ass.tub");
	string result("tub");
	if(getFileExtension(testYes) == result) {
		std::cout << "성공!" << '\n';
	} else {
		std::cout << "실패!" << '\n';
	}

	string testName("not ext\\ension");
	string testResult("ension");
	if(getFileName(testName) == testResult) {
		std::cout << "성공!" << '\n';
	} else {
		std::cout << "실패!" << '\n';
	}

	std::cout << '\n' << "================================" << '\n';
	*/
}


//이 함수는 중요하다. 리소스를 엔진에서 어떻게 해석할지 결정한다.
//TODO: 지금은 이 함수를 바꿔야만 또다른 1:1관계의 추가가 가능한데, 나중에 리팩토링이 필요할 것이다.
void ResourceLoadCaller::initExtensionMap()
{
	extensionMap.insert({ "png", unique_ptr<FileLoader>(new PngLoader(PROJECTOR.Renderer())) });
	extensionMap.insert({ "txt", unique_ptr<FileLoader>( new TxtLoader() ) });
}

void ResourceLoadCaller::insertResources()
{
	// ini파일 로드하기
	IniSerialBuffer inputBuf;

	std::ifstream fin("testIniSerialBuffer.ini", std::ios::binary);
	if(!fin) {
		std::cout << "ResourceLoadCaller::insertResources() Error: can't open ini file" << '\n';
	}
	
	boost::archive::binary_iarchive biarchive(fin);
	biarchive >> inputBuf;
	
	fin.close();

	// vector<string>읽어서 확장자 파악하기
	vector<string> filePaths(inputBuf.StrVec());
	string pngExt("png"), txtExt("txt");

	for(const auto& path : filePaths) {
		auto nowExt = getFileExtension(path);
		auto nowName = getFileName(path);
		if(nowExt.size() == 0  ||  nowName.size() == 0) {
			std::cout << "확장자가 없거나 문제가 있는 이름" << nowExt.c_str();
			continue;
		}
		//확장자 이름과 대응하는 XXXLoader로 파일들 로드.
		auto fileLoaderIter = extensionMap.find(nowExt);
		if(fileLoaderIter != extensionMap.end()) {
			(fileLoaderIter->second)->setPathStr(path);
			shared_ptr<LoadedData> loadedData = (fileLoaderIter->second)->load();
			resLib.insert(nowName, loadedData);
		} 
		else {
			std::cout << "지원하지 않는 리소스 형식: " << nowName.c_str() << '\n';
		}
	}
}

string ResourceLoadCaller::getFileExtension(const string& filePath)
{
	string result;
	
	int end = filePath.size() - 1; //확장자(문자열)의 끝 위치
	for(int begin = filePath.size() - 1; begin >= 0; begin--) {
		if(filePath[begin] == '.') {
			result = filePath.substr(begin+1, end - begin);
			break;
		} 
		else if(filePath[begin] == '\\') { 
			break;
		}
	}

	return result;
}

string ResourceLoadCaller::getFileName(const string& filePath)
{
	string result;

	int end = filePath.size() - 1; //이름(문자열)의 끝 위치
	for(int begin = filePath.size() - 1; begin >= 0; begin--) {
		if(filePath[begin] == '\\') {
			result = filePath.substr(begin + 1, end - begin);
			break;
		} 
	}

	return result;
}