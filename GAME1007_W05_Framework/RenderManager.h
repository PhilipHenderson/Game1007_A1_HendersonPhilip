#ifndef __RENDERMANAGER_H__
#define __RENDERMANAGER_H__
#include <SDL.h>

class RenderManager
{
public: // Public methods.
	static int Init(const char*, const int, const int, const int, const int, const Uint32, const Uint32);
	static void Quit();
	static SDL_Renderer* GetRenderer();
	static SDL_Window* GetWindow(); // Just in case this is ever needed.

private: // Private properties.
	static SDL_Window* m_pWindow;
	static SDL_Renderer* m_pRenderer;
	RenderManager() = default;
};

typedef RenderManager REMA;

#endif