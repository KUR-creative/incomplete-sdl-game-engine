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
	//���� �ε��
	Png(SDL_Texture* tex){
		texture.reset(tex, &SDL_DestroyTexture);
	}

	//resource�ε�. �ؽ����� ���� ����Ѵ�.
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
	explicit PngLoader(shared_ptr<SDL_Renderer> ren)	//��ξ��� ����
		:renderer(ren)
	{}
	//���н� nullptr ��ȯ
	unique_ptr<LoadedData> load() override;	
};