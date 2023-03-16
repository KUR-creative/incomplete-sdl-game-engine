#include "PngLoader.hpp"
#include "Utilities.hpp"
#include <SDL_image.h>

//���� ���� ���н� nullptr ��ȯ�ؾ���.
unique_ptr<LoadedData> PngLoader::load(){
	const char* pngPath = getFilePath().c_str();

	SDL_Texture* texture = IMG_LoadTexture(renderer.get(), pngPath);
	if(!texture) { 
		logSDLError("PngLoader::load(){ IMG_LoadTexture }"); 
		if(isPathStrEmpty()) {
			std::cout << "��� ���ڿ��� ����ֽ��ϴ�. ��ξ��� PngLoader�������� ��θ� ���� �ʰ� ������ �ε����� ���ɼ��� Ů�ϴ�."<< '\n';
		}
		return nullptr;
	}
	//enable alpha blending
	if(SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND)) {
		logSDLError("PngLoader::load() { SetTextureBlendMode }");
	}

	return unique_ptr<LoadedData>(new Png(texture));
}


#include <gtest\gtest.h>
//���� �׽�Ʈ�� ������,������
SDL_Window* win = SDL_CreateWindow("PNGLoader unit test", 50, 50, 400, 200, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

TEST(�����׽�Ʈ, PngLoader){
	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//TEST things...
	const std::string basePath = SDL_GetBasePath();
	string path(basePath + "resource\\sad frog.png");

	//PngLoader�� ��ġ�� ���� �ؽ���(expected) �ε�
	shared_ptr<SDL_Renderer> pren;
	pren.reset(ren, &SDL_DestroyRenderer);
	SDL_Texture* tex1 = IMG_LoadTexture(ren, path.c_str());
	if(tex1) {
		//PngLoader�� �̿��� �ؽ���(actual) �ε�
		PngLoader pngLoader(pren, path);
		FileLoader* fLoader = &pngLoader;
		unique_ptr<LoadedData> png(fLoader->load());

		//���� �׽�Ʈ�� ���� �ؽ��� ����(������ �׽�Ʈ)
		SDL_Texture* tex2 = static_cast<Png*>(png.get())->Texture().get();

		int w1 = 0, h1 = 0;
		SDL_QueryTexture(tex1, nullptr, nullptr, &w1, &h1);

		int w2 = 10, h2 = 10;
		int temp = SDL_QueryTexture(tex2, nullptr, nullptr, &w2, &h2);
		if(temp != 0) {
			logSDLError("SDL_QueryTexture:tex2 ");
		}

		ASSERT_EQ(w1, w2);
		ASSERT_EQ(h1, h2);

		//������ ���� �Ǵ��Ͻö�...
		SDL_Rect dst1;	dst1.x = 0;	dst1.y = 0;	dst1.w = w1;	dst1.h = h1;
		SDL_Rect dst2;	dst2.x = 300;	dst2.y = 300;	dst2.w = w2;	dst2.h = h2;

		SDL_RenderCopy(ren, tex1, NULL, &dst1);
		SDL_RenderCopy(ren, tex2, NULL, &dst2);
		SDL_RenderPresent(ren);
		SDL_Delay(200);
		SDL_RenderPresent(ren);
		SDL_Delay(200);
	} else { //�̷��� ���� �� ���� �ƴѰ� ������...?
		logSDLError("LoadTexture_tex1");
		ASSERT_FALSE(tex1);
	}
}

TEST(�����׽�Ʈ, ��ξ���PngLoader����){
	SDL_Renderer* ren2 = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	shared_ptr<SDL_Renderer> pren;
	pren.reset(ren2, &SDL_DestroyRenderer);
	PngLoader pngLoader(pren);
	unique_ptr<LoadedData> testPng = pngLoader.load();
	ASSERT_FALSE(testPng.get());
	//��� �ֱ�
	const std::string basePath = SDL_GetBasePath();
	string path(basePath + "resource\\sad frog.png");
	pngLoader.setPathStr(path);
	testPng = pngLoader.load();
	ASSERT_TRUE(testPng.get());
}