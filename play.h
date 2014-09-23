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
const int mapTileX = 21;
const int mapTileY = 21;

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
	Tile * _tilesMap[mapTileX][mapTileY];

	Camera _camera;
	int _tileWidth;
	int _tileHeight;
};
#endif