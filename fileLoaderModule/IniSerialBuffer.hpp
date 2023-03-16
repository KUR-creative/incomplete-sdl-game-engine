#pragma once

#include <vector>
#include <string>

#include <io.h>
#include <stdio.h>
#include <cstring>
#include <direct.h>	//windows ����, �׷��� �Ʒ� �ִ� �Լ��� POSIX�� ��?

#include <boost\archive\binary_iarchive.hpp>
#include <boost\archive\binary_oarchive.hpp>
#include <boost\serialization\vector.hpp>
using std::string;
using std::vector;

//�̰Ŵ� ������뿡�� �翬�� ���ºκ��̴�..
//�����ڿ� ���ҽ��δ����� ����.
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
	// dir �Ϻ� ���丮 ������ ���� �̸����� ����Ѵ�.
	vector<string>& getFilePathVector(const char* dir);

public:
	IniSerialBuffer(const char* dir){
		filePaths = getFilePathVector(dir);
	}
	IniSerialBuffer(){ /*input mode*/ }
	vector<string> StrVec(){ return filePaths; }	
	void printArr();
};