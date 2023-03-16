#pragma once

#include <vector>
#include <string>

#include <io.h>
#include <stdio.h>
#include <cstring>
#include <direct.h>	//windows 종속, 그런데 아래 있는 함수는 POSIX인 듯?

#include <boost\archive\binary_iarchive.hpp>
#include <boost\archive\binary_oarchive.hpp>
#include <boost\serialization\vector.hpp>
using std::string;
using std::vector;

//이거는 릴리즈용에는 당연히 없는부분이다..
//개발자용 리소스로더에도 들어간다.
class IniSerialBuffer {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & filePaths;
	}

	vector<string> filePaths;

	//based on legacybass's simple code
	//(http://stackoverflow.com/questions/6133647/how-do-i-list-subdirectories-in-windows-using-c)
	// dir 하부 디렉토리 폴더와 파일 이름들을 출력한다.
	vector<string>& getFilePathVector(const char* dir);

public:
	IniSerialBuffer(const char* dir){
		filePaths = getFilePathVector(dir);
	}
	IniSerialBuffer(){ /*input mode*/ }
	vector<string> StrVec(){ return filePaths; }	
	void printArr();
};