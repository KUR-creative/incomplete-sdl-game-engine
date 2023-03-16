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
	/* �� �׽�Ʈ 
	if(extensionMap["png"]) {
		std::cout << "����: ����" << '\n';
	} else {
		std::cout << "����: �� ��" << '\n';
	}

	if(extensionMap["txt"]) {
		std::cout << "����: ����" << '\n';
	} else {
		std::cout << "����: �� ��" << '\n';
	}

	if(extensionMap["Ȯ���ھƳ�"]) {
		std::cout << "����: ����" << '\n';
	} else {
		std::cout << "����: �� ��" << '\n';
	}
	*/
	
	insertResources();
	/*�ӽ����� �Լ� �׽�Ʈ 
	std::cout << '\n' << "================================" << '\n';
	string testNot("not extension");
	if(getFileExtension(testNot).size() == 0) {
		std::cout << "����!" << '\n';
	} else {
		std::cout << "����!" << '\n';
	}

	string testNot2("not ext\\ension");
	if(getFileExtension(testNot2).size() == 0) {
		std::cout << "����!" << '\n';
	} else {
		std::cout << "����!" << '\n';
	}

	string testYes("ass.tub");
	string result("tub");
	if(getFileExtension(testYes) == result) {
		std::cout << "����!" << '\n';
	} else {
		std::cout << "����!" << '\n';
	}

	string testName("not ext\\ension");
	string testResult("ension");
	if(getFileName(testName) == testResult) {
		std::cout << "����!" << '\n';
	} else {
		std::cout << "����!" << '\n';
	}

	std::cout << '\n' << "================================" << '\n';
	*/
}


//�� �Լ��� �߿��ϴ�. ���ҽ��� �������� ��� �ؼ����� �����Ѵ�.
//TODO: ������ �� �Լ��� �ٲ�߸� �Ǵٸ� 1:1������ �߰��� �����ѵ�, ���߿� �����丵�� �ʿ��� ���̴�.
void ResourceLoadCaller::initExtensionMap()
{
	extensionMap.insert({ "png", unique_ptr<FileLoader>(new PngLoader(PROJECTOR.Renderer())) });
	extensionMap.insert({ "txt", unique_ptr<FileLoader>( new TxtLoader() ) });
}

void ResourceLoadCaller::insertResources()
{
	// ini���� �ε��ϱ�
	IniSerialBuffer inputBuf;

	std::ifstream fin("testIniSerialBuffer.ini", std::ios::binary);
	if(!fin) {
		std::cout << "ResourceLoadCaller::insertResources() Error: can't open ini file" << '\n';
	}
	
	boost::archive::binary_iarchive biarchive(fin);
	biarchive >> inputBuf;
	
	fin.close();

	// vector<string>�о Ȯ���� �ľ��ϱ�
	vector<string> filePaths(inputBuf.StrVec());
	string pngExt("png"), txtExt("txt");

	for(const auto& path : filePaths) {
		auto nowExt = getFileExtension(path);
		auto nowName = getFileName(path);
		if(nowExt.size() == 0  ||  nowName.size() == 0) {
			std::cout << "Ȯ���ڰ� ���ų� ������ �ִ� �̸�" << nowExt.c_str();
			continue;
		}
		//Ȯ���� �̸��� �����ϴ� XXXLoader�� ���ϵ� �ε�.
		auto fileLoaderIter = extensionMap.find(nowExt);
		if(fileLoaderIter != extensionMap.end()) {
			(fileLoaderIter->second)->setPathStr(path);
			shared_ptr<LoadedData> loadedData = (fileLoaderIter->second)->load();
			resLib.insert(nowName, loadedData);
		} 
		else {
			std::cout << "�������� �ʴ� ���ҽ� ����: " << nowName.c_str() << '\n';
		}
	}
}

string ResourceLoadCaller::getFileExtension(const string& filePath)
{
	string result;
	
	int end = filePath.size() - 1; //Ȯ����(���ڿ�)�� �� ��ġ
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

	int end = filePath.size() - 1; //�̸�(���ڿ�)�� �� ��ġ
	for(int begin = filePath.size() - 1; begin >= 0; begin--) {
		if(filePath[begin] == '\\') {
			result = filePath.substr(begin + 1, end - begin);
			break;
		} 
	}

	return result;
}