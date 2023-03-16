#pragma once
				
#include <memory>
#include "FileLoader.hpp"
using std::string;
using std::unique_ptr;

//���Ͽ��� load�� �������� �����(string wrapper) 
// ANSI(��Ƽ����Ʈ) encoded
class Text: public LoadedData {
private:
	string txt;
	
public:			
	//���� �ε��
	explicit Text(const char* txtData)
		: txt(txtData)
	{}
	explicit Text(const string& txtData)
		: txt(txtData)
	{}

	//resource�ε�. �ؽ��� �ֱ�.
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
	//��ξ��� ����
	explicit TxtLoader(){}
	
	//���нÿ� nullptr ��ȯ
	unique_ptr<LoadedData> load() override;	
};
