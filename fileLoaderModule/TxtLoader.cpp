#include <fstream>
#include <SDL.h>
#include "TxtLoader.hpp"
#include "Utilities.hpp"


unique_ptr<LoadedData> TxtLoader::load(){
	std::ifstream fin(getFilePath());
	if(!fin) {
		std::cout << "���� ���� ����!" << '\n';
		if(isPathStrEmpty()) {
			std::cout << "��� ���ڿ��� ����ֽ��ϴ�. ��ξ��� TxtLoader�������� ��θ� ���� �ʰ� ������ �ε����� ���ɼ��� Ů�ϴ�." << '\n';
		}
		return nullptr;
	}

	string buf;
	int c;	//TODO : �̰� ���ִ� ǥ���Լ� ����...?
	while((c = fin.get()) != EOF) {
		buf += c;
	}
	fin.close();

	return std::make_unique<Text>(buf);
}


#include <gtest\gtest.h>

TEST(�����׽�Ʈ, TxtLoader){
	//ǥ������ ���� �ε�.
	const std::string basePath = SDL_GetBasePath();
	string path(basePath + "resource\\test.txt");

	std::ifstream fin(path);
	if(!fin) {
		std::cout << "���� ���� ����!" << '\n';
	}

	string file;
	int c;
	while((c = fin.get()) != EOF) {
		file += c;
	}
	fin.close();

	//TxtLoader�� �̿��� ���Ϸε�
	TxtLoader txtLoader(path);	//const string& path
	FileLoader* fLoader = &txtLoader;
	unique_ptr<LoadedData> testText(fLoader->load());

	if(testText) {
		EXPECT_TRUE(testText.get());

		//���� ���¿� ������ ���
		//���� txt�� string���� �޸𸮿� �÷�����
		ASSERT_STREQ(file.c_str(), static_cast<Text*>(testText.get())->c_str());
	} else { //���� ���¿� ������ ����� ����ó��
		ASSERT_FALSE(testText);
	}
}

TEST(�����׽�Ʈ, ��ξ���TxtLoader����){
	TxtLoader txtLoader;
	unique_ptr<LoadedData> testText(txtLoader.load());
	ASSERT_FALSE(testText.get());	//��ΰ� ���ٸ� ���Ϸε� ����->nullptr��ȯ
	
	//��� �ֱ�
	const std::string basePath = SDL_GetBasePath();
	string path(basePath + "resource\\test.txt");
	txtLoader.setPathStr(path);
	testText = txtLoader.load();
	ASSERT_TRUE(testText.get());	//��ΰ� �ִٸ� ���Ϸε� ����.

	//�δ��� ������� ���� ���Ϸε�� ��
	std::ifstream fin(path);
	if(!fin) {
		std::cout << "���� ���� ����!" << '\n';
	}
	string noLoader;
	int c;
	while((c = fin.get()) != EOF) {
		noLoader += c;
	}
	fin.close();

	ASSERT_STREQ(noLoader.c_str(), static_cast<Text*>(testText.get())->c_str() );

	

}