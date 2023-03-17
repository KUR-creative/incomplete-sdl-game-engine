//TODO:릴리즈용의 경우 vector<string> paths를 주입하지 않았다면 경고를 해줘야 한다.
//그건 RES_LIB에서 LoadedData를 꺼낼 알 수 있을것이다.
#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>

#include "FileLoader.hpp"
#include "Singleton.hpp"
using std::unordered_map;
using std::shared_ptr;
using std::static_pointer_cast;


#include "Error.hpp"
class ResLibError: public Error {
public:
	string hashStr;

	//menu == 0: LoadedData형식으로 꺼내려한 경우, catch문 사용
	ResLibError()
		: Error(false)
	{}
	//menu == 1: hash_key가 resourceMap에 없는 경우
	ResLibError(const string& hashStr)
		: Error(false,1), hashStr(hashStr)			//todo:나중에 isCritical == true
	{}

	void printErrorReason() override{
		switch(menu) {
		case 0:
			std::cout << "RES_LIB에서 shared_ptr<LoadedData>형식의 참조를 얻으려 했습니다." << '\n'
				<< "LoadedData를 상속하는 concrete class만 허용됩니다." << '\n';
			break;
		case 1:
			std::cout << "RES_LIB에 해쉬 키 \"" << hashStr
				<< "\"과 일치하는 LoadedData객체가 없습니다." << '\n';
			break;
		}
	}
};


class ResourceLibrary: public Singleton<ResourceLibrary> {
friend class Singleton<ResourceLibrary>;

private:
	unordered_map<string, shared_ptr<LoadedData>> resourceMap;
	
public:
	//할당 실패시 false TODO: throw를 이용하도록 바꾸기.
	bool insert(const string& key, shared_ptr<LoadedData> item);

	//Data를 이용해 반환type을 얻고, HashStr을 이용해 해쉬key를 얻는다.
	template<typename Data>
	shared_ptr<Data> operator[](const Data& keyObj){
		auto result = resourceMap.find(keyObj.HashStr());
		if(result == resourceMap.end()) {
			ResLibError err(keyObj.HashStr());
			throw err;
		} else {
			return static_pointer_cast<Data>(result->second);
		}

	}

	template<>
	shared_ptr<LoadedData> operator[](const LoadedData& keyObj){
		ResLibError err;
		throw err;	
	}

private:
	ResourceLibrary(){}
};

//helper macro
#define RES_LIB		ResourceLibrary::Instance()
