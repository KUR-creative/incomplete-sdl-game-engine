//TODO:릴리즈용의 경우 vector<string> paths를 주입하지 않았다면 경고를 해줘야 한다.
//그건 RES_LIB에서 LoadedData를 꺼낼 알 수 있을것이다.
#pragma once

#include <string>
#include "ResourceLibrary.hpp"
#include "Singleton.hpp"

#include "FileLoader.hpp"
#include "Projector.hpp"

class ResourceLoadCaller: public Singleton<ResourceLoadCaller> {
friend class Singleton<ResourceLoadCaller>;

private:
	ResourceLibrary& resLib;
	unordered_map<string,unique_ptr<FileLoader>> extensionMap;
	
private:
	//초기화
	explicit ResourceLoadCaller(ResourceLibrary& resLib);

	//파일:원시객체 1:1 대응을 위한 로더들을 넣어준다.
	void initExtensionMap();
	//리소스를 extensionMap에 따라서 resLib에 넣어준다.
	void insertResources();

	//확장자 or 이름이 없는 경우 빈 string 반환.
	string getFileExtension(const string& filePath);
	string getFileName(const string& filePath);
};

//helper macro
#define RES_LOAD_CALLER		ResourceLoadCaller::Instance()