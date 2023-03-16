#include "PngLoader.hpp"
#include "Utilities.hpp"
#include <SDL_image.h>

//파일 열기 실패시 nullptr 반환해야함.
unique_ptr<LoadedData> PngLoader::load(){
	const char* pngPath = getFilePath().c_str();

	SDL_Texture* texture = IMG_LoadTexture(renderer.get(), pngPath);
	if(!texture) { 
		logSDLError("PngLoader::load(){ IMG_LoadTexture }"); 
		if(isPathStrEmpty()) {
			std::cout << "경로 문자열이 비어있습니다. 경로없는 PngLoader생성이후 경로를 넣지 않고 파일을 로드했을 가능성이 큽니다."<< '\n';
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
//단위 테스트용 윈도우,렌더러
SDL_Window* win = SDL_CreateWindow("PNGLoader unit test", 50, 50, 400, 200, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

TEST(단위테스트, PngLoader){
	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//TEST things...
	const std::string basePath = SDL_GetBasePath();
	string path(basePath + "resource\\sad frog.png");

	//PngLoader를 거치지 않은 텍스쳐(expected) 로드
	shared_ptr<SDL_Renderer> pren;
	pren.reset(ren, &SDL_DestroyRenderer);
	SDL_Texture* tex1 = IMG_LoadTexture(ren, path.c_str());
	if(tex1) {
		//PngLoader를 이용한 텍스쳐(actual) 로드
		PngLoader pngLoader(pren, path);
		FileLoader* fLoader = &pngLoader;
		unique_ptr<LoadedData> png(fLoader->load());

		//쿼리 테스트를 위한 텍스쳐 추출(삭제될 테스트)
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

		//눈으로 보고 판단하시라...
		SDL_Rect dst1;	dst1.x = 0;	dst1.y = 0;	dst1.w = w1;	dst1.h = h1;
		SDL_Rect dst2;	dst2.x = 300;	dst2.y = 300;	dst2.w = w2;	dst2.h = h2;

		SDL_RenderCopy(ren, tex1, NULL, &dst1);
		SDL_RenderCopy(ren, tex2, NULL, &dst2);
		SDL_RenderPresent(ren);
		SDL_Delay(200);
		SDL_RenderPresent(ren);
		SDL_Delay(200);
	} else { //이렇게 쓰는 건 뭔가 아닌거 같은데...?
		logSDLError("LoadTexture_tex1");
		ASSERT_FALSE(tex1);
	}
}

TEST(단위테스트, 경로없는PngLoader생성){
	SDL_Renderer* ren2 = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	shared_ptr<SDL_Renderer> pren;
	pren.reset(ren2, &SDL_DestroyRenderer);
	PngLoader pngLoader(pren);
	unique_ptr<LoadedData> testPng = pngLoader.load();
	ASSERT_FALSE(testPng.get());
	//경로 넣기
	const std::string basePath = SDL_GetBasePath();
	string path(basePath + "resource\\sad frog.png");
	pngLoader.setPathStr(path);
	testPng = pngLoader.load();
	ASSERT_TRUE(testPng.get());
}