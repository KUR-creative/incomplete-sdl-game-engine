/* inline으로 하지 않으면 오류가 난다. 왜?
* class 내부의 멤버함수가 아니니까 자동으로 inline화 되지 않는것이다.
* 그러므로 간단한 함수를 모아놓고 헤더파일에서 써먹으려면 반드시 inline화를 해야한다.
*
*(1)cleanup function,(2)logSDLError	made by Will Usher
* http://www.willusher.io/sdl2%20tutorials/2014/08/01/postscript-1-easy-cleanup/
*
*/

// (1)cleanup
#pragma once

#ifndef UTILITIES_H
#define UTILITIES_H

#ifndef _UTILITY_
# include <utility>
#endif

#ifndef _SDL_H
# include <SDL.h>
#endif

/*
* Recurse through the list of arguments to clean up, cleaning up
* the first one in the list each iteration.
*/
template<typename T, typename... Args>
inline void cleanup(T *t, Args&&... args){
	//Cleanup the first item in the list
	cleanup(t);
	//Recurse to clean up the remaining arguments
	cleanup(std::forward<Args>(args)...);
}

/*
* These specializations serve to free the passed argument and also provide the
* base cases for the recursive call above, eg. when args is only a single item
* one of the specializations below will be called by
* cleanup(std::forward<Args>(args)...), ending the recursion
* We also make it safe to pass nullptrs to handle situations where we
* don't want to bother finding out which values failed to load (and thus are null)
* but rather just want to clean everything up and let cleanup sort it out
*/

template<>
inline void cleanup<SDL_Window>(SDL_Window *win){
	if(!win) {
		return;
	}
	SDL_DestroyWindow(win);
}
template<>
inline void cleanup<SDL_Renderer>(SDL_Renderer *ren){
	if(!ren) {
		return;
	}
	SDL_DestroyRenderer(ren);
}
template<>
inline void cleanup<SDL_Texture>(SDL_Texture *tex){
	if(!tex) {
		return;
	}
	SDL_DestroyTexture(tex);
}
template<>
inline void cleanup<SDL_Surface>(SDL_Surface *surf){
	if(!surf) {
		return;
	}
	SDL_FreeSurface(surf);
}

// (2)logSDLError
#ifndef _IOSTREAM_
# include <iostream>
#endif

#ifndef _INC_STRING
# include <string>
#endif

inline void logSDLError(const std::string& msg, std::ostream& os = std::cout)
{
	os << msg.c_str() << " Error: " << SDL_GetError() << std::endl;
}

#endif

#include <cassert>
inline void logSDLCriticalError(const std::string& msg, std::ostream& os = std::cout)
{
	os << msg.c_str() << " Error: " << SDL_GetError() << std::endl;
	assert(false);
}