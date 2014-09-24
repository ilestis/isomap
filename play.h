#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include <SDL.h>
#include "gameengine.h"
#include "gamestate.h"
#include "tile.h"
#include "camera.h"
#include "renderer.h"
#include <list>
#include <vector>
#include <sstream>
const int mapTileX = 41;
const int mapTileY = 41;
const float MAP_MOUNTAIN_HEIGHT = 10.5f;
const float MAP_PLAIN_HEIGHT = 6.3f;
const float MAP_FOREST_THRESHOLD = 0.5f;
const float MAP_DESERT_THRESHOLD = -0.5f;

class PlayState : public GameState
{
public:
	void Init(GameEngine* game);
	void Cleanup();
	void loadTextures();
	void generateLayersAndTiles(int layers);

	void Pause();
	void Resume();

	void HandleEvents(GameEngine* game);
	void Update(GameEngine* game);
	void Draw(GameEngine* game);
	
	void Reset();

	static PlayState* Instance() {
		return &m_PlayState;
	}

protected:
	PlayState() { }

private:
	static PlayState m_PlayState;

	SDL_Texture *_backgroundTexture, *_hexaTexture;
	SDL_Rect _backgroundClip, _backgroundPosition;
	SDL_Color m_color;
	std::string m_font;

	// Lists
	//std::vector<Tile*> _tiles;
	Tile * _tilesMap[mapTileY+1][mapTileX+1];


	std::stringstream _ssGuiElement;

	Camera _camera;
	int _tileWidth;
	int _tileHeight;

	// Current mouse coordinates
	int _currentXCoordinate, _currentYCoordinate;
	bool _requireUpdate;
};
#endif