#pragma once
#include <string>
#include <SDL.h>


class Tile
{
public:
	Tile();
	~Tile();

	// Getters
	SDL_Rect position();
	SDL_Rect clip();
	SDL_Rect overlayClip();
	float height();
	float climate();
	bool hasOverlay();

	// Setters
	void position(int top, int left);
	void placement(int row, int col);
	void size(int width, int height);
	void type(int type);
	void height(float height);
	void addHeight(float difference);
	void addClimate(float difference);
	void overlay(int overlay);



private:
	int _positionX, _positionY;
	int _row, _col;
	int _width, _height;
	SDL_Rect _clip;
	int _type;
	float _heat;
	float _climate; // negative = arid, positif = wet

	const float MIN_HEAT = 0.0f;
	const float MAX_HEAT = 50.0f;


	int _overlay;
	SDL_Rect _overlayClip;
};

