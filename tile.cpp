#include "tile.h"


Tile::Tile()
{
	_positionX = 0;
	_positionY = 0;
	_width = 32;
	_height = 32;
	_type = 0;
	_heat = 0.0f;
	_aridity = 0.0f;
	_overlay = 0;

	_clip = { 0, 0, _width, _height };
}

void Tile::size(int width, int height)
{
	_width = width;
	_height = height;

	// Regenerate clip size
	_clip = { 0, 0, _width, _height };
}
void Tile::position(int top, int left)
{
	_positionX = top;
	_positionY = left;
}

SDL_Rect Tile::position()
{
	SDL_Rect pos = { _positionX, _positionY, _width, _height };
	return pos;
}

Tile::~Tile()
{
}

SDL_Rect Tile::clip()
{
	return _clip;
}

void Tile::type(int type)
{
	_type = type;

	// Regenerate clip position
	_clip = { type * _width, 0, _width, _height };
}

void Tile::height(float height)
{
	_heat = height;
}

void Tile::addHeight(float difference)
{
	_heat += difference;
	//printf("[%d][%d]: %f \n", _row, _col, _heat);
	if (_heat < -0.3f) {
		_heat = -0.3f;
	}
}

float Tile::height()
{
	return _heat;
}

void Tile::placement(int row, int col)
{
	_row = row;
	_col = col;
}

void Tile::addAridity(float difference)
{
	_aridity += difference;
}

float Tile::aridity()
{
	return _aridity;
}

void Tile::overlay(int overlay) {
	_overlay = overlay;

	// Regenerate clip position
	_overlayClip = { overlay * _width, _height, _width, _height };
}

SDL_Rect Tile::overlayClip()
{
	return _overlayClip;
}

bool Tile::hasOverlay()
{
	return _overlay != 0;
}