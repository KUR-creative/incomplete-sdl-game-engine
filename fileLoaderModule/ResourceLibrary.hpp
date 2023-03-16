//TODO:��������� ��� vector<string> paths�� �������� �ʾҴٸ� ��� ����� �Ѵ�.
//�װ� RES_LIB���� LoadedData�� ���� �� �� �������̴�.
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

	//menu == 0: LoadedData�������� �������� ���, catch�� ���
	ResLibError()
		: Error(false)
	{}
	//menu == 1: hash_key�� resourceMap�� ���� ���
	ResLibError(const string& hashStr)
		: Error(false,1), hashStr(hashStr)			//todo:���߿� isCritical == true
	{}

	void printErrorReason() override{
		switch(menu) {
		case 0:
			std::cout << "RES_LIB���� shared_ptr<LoadedData>������ ������ ������ �߽��ϴ�." << '\n'
				<< "LoadedData�� ����ϴ� concrete class�� ���˴ϴ�." << '\n';
			break;
		case 1:
			std::cout << "RES_LIB�� �ؽ� Ű \"" << hashStr
				<< "\"�� ��ġ�ϴ� LoadedData��ü�� �����ϴ�." << '\n';
			break;
		}
	}
};


class ResourceLibrary: public Singleton<ResourceLibrary> {
friend class Singleton<ResourceLibrary>;

private:
	unordered_map<string, shared_ptr<LoadedData>> resourceMap;
	
public:
	//�Ҵ� ���н� false TODO: throw�� �̿��ϵ��� �ٲٱ�.
	bool insert(const string& key, shared_ptr<LoadedData> item);

	//Data�� �̿��� ��ȯtype�� ���, HashStr�� �̿��� �ؽ�key�� ��´�.
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
