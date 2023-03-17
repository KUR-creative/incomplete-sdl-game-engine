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
	const string& HashStr() const{ return hashStr; }	//~LoadedData(){ printf("---LoadedData 죽었다!---\n"); }
};

class FileLoader {
private:
	string filePath;

public:
	explicit FileLoader(const string& path): filePath(path){}
	explicit FileLoader(){}//경로 없는 생성
	
	//실패시 nullptr반환
	virtual unique_ptr<LoadedData> load() = 0; //filePath로 힙에 할당시킨 다음 반환한다.

	void setPathStr(const string& newPath){ filePath = newPath; }

protected:
	const string& getFilePath() const { return filePath; }
	bool isPathStrEmpty() const { return (filePath.length() ? false : true); }

};

