#pragma once
				
#include <memory>
#include "FileLoader.hpp"
using std::string;
using std::unique_ptr;

//파일에서 load한 데이터의 저장소(string wrapper) 
// ANSI(멀티바이트) encoded
class Text: public LoadedData {
private:
	string txt;
	
public:			
	//파일 로드용
	explicit Text(const char* txtData)
		: txt(txtData)
	{}
	explicit Text(const string& txtData)
		: txt(txtData)
	{}

	//resource로드. 해쉬값 넣기.
	explicit Text(const hash_key& hashKey)
		: LoadedData(hashKey)
	{}

	const char* c_str() const{ return txt.c_str(); }
	const string& getString() const { return txt; }
};

class TxtLoader: public FileLoader{
public:
	explicit TxtLoader(const string& path)
		: FileLoader(path)
	{}
	//경로없는 생성
	explicit TxtLoader(){}
	
	//실패시에 nullptr 반환
	unique_ptr<LoadedData> load() override;	
};
