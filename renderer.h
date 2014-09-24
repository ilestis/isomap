#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

#ifndef RENDERER_H
#define RENDERER_H

class Renderer
{
public:
	Renderer();
public:
	static SDL_Renderer* g_renderer;
	static SDL_Window* g_window;
public:
	static void Clear();
	static SDL_Surface* loadSurface(char* File);
	static SDL_Surface* createSurface(int w, int h);
	static bool blitSurface(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y);
	static bool blitSurface(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H);
	static SDL_Texture* loadTexture(char* File);
	static SDL_Texture* loadTexture(char* File, Uint32 key);
	static bool renderTextureScale(SDL_Texture* texture, int xSrc, int ySrc, int xDest, int yDest, int wSrc, int hSrc, int wDest, int hDest);
	static bool renderTextureNoScale(SDL_Texture* texture, int xSrc, int ySrc, int xDest, int yDest, int wSrc, int hSrc);
	static bool renderTexture(SDL_Texture* texture, int xDest, int yDest);
	static bool renderTexture(SDL_Texture* texture, int xSrc, int ySrc, int xDest, int yDest, int w, int h);
	static bool renderTexture(SDL_Texture* texture, bool scale);
	static bool renderTexture(SDL_Texture* texture, SDL_Rect &destRect, SDL_Rect *clip = NULL);
	static void setScale(float scaleX, float scaleY);

	static bool renderText(const std::string &message, const std::string &fontfile,
		SDL_Color color, int fontSize, int top, int left);
	static void Present();
	static bool renderCollider(SDL_Rect collider, int color);
};

#endif