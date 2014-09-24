#include "Renderer.h"
#include <string>
#include <algorithm>

SDL_Renderer* Renderer::g_renderer;
SDL_Window* Renderer::g_window;

Renderer::Renderer() {

}

void Renderer::Clear()
{
	SDL_RenderClear(Renderer::g_renderer);
}

SDL_Surface* Renderer::createSurface(int w, int h) {
	return SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
}

SDL_Surface* Renderer::loadSurface(char* File) {
	SDL_Surface* Surf_Return = NULL;
	if ((Surf_Return = IMG_Load(File)) == NULL) {
		return NULL;
	}
	return Surf_Return;
}

bool Renderer::blitSurface(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y) {
	if (Surf_Dest == NULL || Surf_Src == NULL)
	{
		return false;
	}
	SDL_Rect DestR;
	DestR.x = X;
	DestR.y = Y;
	SDL_BlitSurface(Surf_Src, NULL, Surf_Dest, &DestR);
	return true;
}

bool Renderer::blitSurface(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H) {
	if (Surf_Dest == NULL || Surf_Src == NULL)
	{
		return false;
	}
	SDL_Rect DestR;
	DestR.x = X;
	DestR.y = Y;
	SDL_Rect SrcR;
	SrcR.x = X2;
	SrcR.y = Y2;
	SrcR.w = W;
	SrcR.h = H;
	return (SDL_BlitSurface(Surf_Src, &SrcR, Surf_Dest, &DestR) == 0);

}
//for textures
SDL_Texture* Renderer::loadTexture(char* File) {
	SDL_Surface* newSurface = loadSurface(File);
	if (newSurface != NULL) {
		SDL_Texture* newTexture = SDL_CreateTextureFromSurface(Renderer::g_renderer, newSurface);
		SDL_FreeSurface(newSurface);
		return newTexture;
	}
	else {
		return NULL;
	}
}

//for textures with color key
SDL_Texture* Renderer::loadTexture(char* File, Uint32 key) {
	SDL_Surface* newSurface = loadSurface(File);
	if (newSurface != NULL) {
		SDL_SetColorKey(newSurface, SDL_TRUE, key);
		SDL_Texture* newTexture = SDL_CreateTextureFromSurface(Renderer::g_renderer, newSurface);
		SDL_FreeSurface(newSurface);
		return newTexture;
	}
	else {
		return NULL;
	}

}

//to scale texture appropriately when rendering
bool Renderer::renderTextureScale(SDL_Texture* texture, int xSrc, int ySrc, int xDest, int yDest, int wSrc, int hSrc, int wDest, int hDest) {
	SDL_Rect srcRect;
	srcRect.x = std::max(0, xSrc);
	srcRect.y = std::max(0, ySrc);
	int wTexture, hTexture;
	if (SDL_QueryTexture(texture, NULL, NULL, &wTexture, &hTexture) < 0)
		return false;
	srcRect.w = std::min(wSrc, wTexture - xSrc);
	srcRect.h = std::min(hSrc, hTexture - ySrc);
	if (xSrc < 0)
		srcRect.w += xSrc;
	if (ySrc < 0)
		srcRect.h += ySrc;

	SDL_Rect destRect;
	destRect.x = xDest;
	destRect.y = yDest;
	destRect.w = wDest;
	destRect.h = hDest;
	float xZoom = (float) wDest / wSrc;
	if (xSrc < 0) {
		float xPad = xSrc*xZoom;
		destRect.x -= (int) xPad;
		destRect.w += (int)xPad;
	}
	float yZoom = (float) hDest / hSrc;
	if (ySrc < 0) {
		float yPad = ySrc*yZoom;
		destRect.y -= (int)yPad;
		destRect.h += (int)yPad;
	}
	if (wTexture - xSrc < wSrc) {
		destRect.w -= (int) (wSrc - (wTexture - xSrc)) * xZoom;
	}
	if (hTexture - ySrc < hSrc) {
		destRect.h -= (int) (hSrc - (hTexture - ySrc)) * yZoom;
	}
	return (SDL_RenderCopy(Renderer::g_renderer, texture, &srcRect, &destRect) == 0);
}

//to render texture without changing its dimensions
bool Renderer::renderTextureNoScale(SDL_Texture* texture, int xSrc, int ySrc, int xDest, int yDest, int wSrc, int hSrc) {
	SDL_Rect srcRect;
	srcRect.x = std::max(0, xSrc);
	srcRect.y = std::max(0, ySrc);
	int wTexture, hTexture;
	if (SDL_QueryTexture(texture, NULL, NULL, &wTexture, &hTexture) < 0)
		return false;
	srcRect.w = std::min(wSrc, wTexture - xSrc);
	srcRect.h = std::min(hSrc, hTexture - ySrc);
	if (xSrc < 0)
		srcRect.w += xSrc;
	if (ySrc < 0)
		srcRect.h += ySrc;

	SDL_Rect destRect;
	destRect.x = xDest;
	destRect.y = yDest;
	if (xSrc < 0)
		destRect.x -= xSrc;
	if (ySrc < 0)
		destRect.y -= ySrc;
	destRect.w = srcRect.w;
	destRect.h = srcRect.h;
	return (SDL_RenderCopy(Renderer::g_renderer, texture, &srcRect, &destRect) == 0);
}

//just plain rendering
bool Renderer::renderTexture(SDL_Texture* texture, int xDest, int yDest) {
	SDL_Rect destRect;
	destRect.x = xDest;
	destRect.y = yDest;
	if (SDL_QueryTexture(texture, NULL, NULL, &destRect.w, &destRect.h) < 0)
		return false;

	return (SDL_RenderCopy(Renderer::g_renderer, texture, NULL, &destRect) == 0);
}

bool Renderer::renderTexture(SDL_Texture* texture, int xSrc, int ySrc, int xDest, int yDest, int w, int h) {
	return renderTextureScale(texture, xSrc, ySrc, xDest, yDest, w, h, w, h);
}

bool Renderer::renderTexture(SDL_Texture* texture, bool scale) {
	if (scale) {
		return (SDL_RenderCopy(Renderer::g_renderer, texture, NULL, NULL) == 0);
	}
	else {
		int wTexture;
		int hTexture;
		if (SDL_QueryTexture(texture, NULL, NULL, &wTexture, &hTexture) < 0)
			return false;
		return renderTexture(texture, 0, 0, 0, 0, wTexture, hTexture);
	}
}

bool Renderer::renderTexture(SDL_Texture* texture, SDL_Rect &destRect, SDL_Rect *clip) {
	//if (SDL_QueryTexture(texture, NULL, NULL, &destRect.w, &destRect.h) < 0)
	//	return false;

	return (SDL_RenderCopy(Renderer::g_renderer, texture, clip, &destRect) == 0);
}

void Renderer::Present()
{
	SDL_RenderPresent(Renderer::g_renderer);
}

bool Renderer::renderText(const std::string &message, const std::string &font, SDL_Color color, int size,
	int top, int left)
{
	TTF_Font *ttffont = nullptr;
	ttffont = TTF_OpenFont(font.c_str(), size);
	if (ttffont == nullptr)
		throw std::runtime_error("Failed to load font: " + font + TTF_GetError());

	SDL_Surface *surface = TTF_RenderUTF8_Blended(ttffont, message.c_str(), color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(Renderer::g_renderer, surface);

	SDL_Rect destination = { 0, 0, 0, 0 };
	destination.x = left;
	destination.y = top;

	TTF_SizeUTF8(ttffont, message.c_str(), &destination.w, &destination.h);

	// Draw
	SDL_RenderCopy(Renderer::g_renderer, texture, NULL, &destination);

	// Close
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	TTF_CloseFont(ttffont);
	return true;
}

bool Renderer::renderCollider(SDL_Rect collider, int color) {
	SDL_SetRenderDrawColor(Renderer::g_renderer, color, color, color, color);
	SDL_RenderDrawRect(Renderer::g_renderer, &collider);
	SDL_SetRenderDrawColor(Renderer::g_renderer, 0, 0, 0, 0);
	return true;
}

void Renderer::setScale(float scaleX, float scaleY)
{
	SDL_RenderSetScale(Renderer::g_renderer, scaleX, scaleY);
}