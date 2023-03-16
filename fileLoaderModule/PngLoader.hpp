#pragma once

#include <memory>
#include <SDL.h>
#include "FileLoader.hpp"
using std::unique_ptr;
using std::shared_ptr;

						
class Png: public LoadedData {
private:
	shared_ptr<SDL_Texture> texture;
public:					
	//파일 로드용
	Png(SDL_Texture* tex){
		texture.reset(tex, &SDL_DestroyTexture);
	}

	//resource로드. 해쉬값을 같이 줘야한다.
	explicit Png(const hash_key& hashKey)
		: texture(nullptr), LoadedData(hashKey)
	{};

	shared_ptr<SDL_Texture> Texture(){ return texture; }
};


class PngLoader: public FileLoader {
private:
	shared_ptr<SDL_Renderer> renderer;
public:
	explicit PngLoader(shared_ptr<SDL_Renderer> ren, const string& path)
		: renderer(ren), FileLoader(path)
	{}
	explicit PngLoader(shared_ptr<SDL_Renderer> ren)	//경로없는 생성
		:renderer(ren)
	{}
	//실패시 nullptr 반환
	unique_ptr<LoadedData> load() override;	
};