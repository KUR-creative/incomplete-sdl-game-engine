#include "CsvSaver.h"
#include "SDL.h"
#include "Utilities.h"


bool CsvSaver::save(const string& folderPath, const string& onlyName, const string& data){
	//filePath에 대한 여러 처리를 해야함(파일이름을 넣은 경우... \\에 대한 경우...)
	string filePath = folderPath + onlyName;

	SDL_RWops* csvFile = SDL_RWFromFile(filePath.c_str(), "w");
	if(csvFile) {
		SDL_RWwrite(csvFile, data.c_str(), data.size() + 1, 1);
		SDL_FreeRW(csvFile);
		return true;
	} 
	else {
		logSDLError("CsvSaver::save(...):");
		return false;
	}	
}