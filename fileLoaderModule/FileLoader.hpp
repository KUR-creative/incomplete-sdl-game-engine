#pragma once

#include <string>
#include <memory>
using std::string;
using std::unique_ptr;


class hash_key: public string {
public:
	explicit hash_key(const char* str)
		:string(str)
	{}
	explicit hash_key(const string& str)
		:string(str)
	{}
};

class LoadedData {
private:
	string hashStr;
public:
	explicit LoadedData(){}
	explicit LoadedData(const hash_key& key)
		:hashStr(key)
	{}
	const string& HashStr() const{ return hashStr; }	//~LoadedData(){ printf("---LoadedData �׾���!---\n"); }
};

class FileLoader {
private:
	string filePath;

public:
	explicit FileLoader(const string& path): filePath(path){}
	explicit FileLoader(){}//��� ���� ����
	
	//���н� nullptr��ȯ
	virtual unique_ptr<LoadedData> load() = 0; //filePath�� ���� �Ҵ��Ų ���� ��ȯ�Ѵ�.

	void setPathStr(const string& newPath){ filePath = newPath; }

protected:
	const string& getFilePath() const { return filePath; }
	bool isPathStrEmpty() const { return (filePath.length() ? false : true); }

};

