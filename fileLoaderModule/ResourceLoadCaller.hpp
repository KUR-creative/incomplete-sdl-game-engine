//TODO:��������� ��� vector<string> paths�� �������� �ʾҴٸ� ��� ����� �Ѵ�.
//�װ� RES_LIB���� LoadedData�� ���� �� �� �������̴�.
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
	//�ʱ�ȭ
	explicit ResourceLoadCaller(ResourceLibrary& resLib);

	//����:���ð�ü 1:1 ������ ���� �δ����� �־��ش�.
	void initExtensionMap();
	//���ҽ��� extensionMap�� ���� resLib�� �־��ش�.
	void insertResources();

	//Ȯ���� or �̸��� ���� ��� �� string ��ȯ.
	string getFileExtension(const string& filePath);
	string getFileName(const string& filePath);
};

//helper macro
#define RES_LOAD_CALLER		ResourceLoadCaller::Instance()