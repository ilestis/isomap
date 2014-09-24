#include <stdio.h>
#include "play.h"

PlayState PlayState::m_PlayState;

void PlayState::Init(GameEngine* game)
{
	loadTextures();
	_tileWidth = 64;
	_tileHeight = 64;
	_currentXCoordinate = 0;
	_currentYCoordinate = 0;
	generateLayersAndTiles(3);

	// Prepare camera
	_camera.reset(game->m_width, game->m_height);

	// Font
	m_font = "./res/font/font.ttf";
	m_color = { 255, 0, 0 };

	printf("Play State initialised\n");
}

void PlayState::generateLayersAndTiles(int layers)
{
	printf("Initializing Layers and Tiles\n");
	int _tileHeightMargin = _tileHeight - (_tileHeight / 4);
	int _tileWidthMargin = _tileWidth / 2;

	int row, col;
	int posX, posY;
	int random = 0;

	// Max X and Y coordinates for the array
	int maxX = mapTileX - 1;
	int maxY = mapTileY - 1;

	// First loop through the grid to generate the base tiles
	for (row = 0; row <= mapTileY; row++) {
		for (col = 0; col <= mapTileX; col++) {

			// Calculate position
			posX = col * _tileWidth;
			posY = row * _tileHeightMargin;
			if (row % 2 != 0) {
				posX += _tileWidthMargin;
			}
			// add the offset to make the map look "fullscreen"
			posX -= _tileWidth / 2;
			posY -= _tileWidth / 4;

			// Create the tile object
			Tile* oneTile = new Tile();
			oneTile->placement(row, col);
			oneTile->position(posX, posY);
			oneTile->size(_tileWidth, _tileHeight);
			oneTile->height(-2.0); // start everything low to give water around a central point
			oneTile->type(1);

			// If we're on the edge of the map, make it lower to try
			// and force an ocean around the map.
			/*if (row <= 5 || col <= 5) {
				int diff = 40 - (row * col);
				if (diff < 0) {
					diff = 0;
				}
				oneTile->addHeight(-diff);
			}
			else if (row >= maxY - 5 || col >= maxX - 5) {
				int diff = 40 - ((row - maxY) * (col - maxX));
				if (diff < 0) {
					diff = 0;
				}
				oneTile->addHeight(-diff);
			}*/

			_tilesMap[row][col] = oneTile;

			oneTile = NULL;
			delete oneTile;
		}
	}

	bool even = false;
	int offset = 0;
	float diffHeight, diffClimate;

	// Now we loop through them again a couple of times to generate random "hight" and "climate"
	for (int loop = 0; loop < 5; loop++) {
		for (row = 1; row < maxY; row++) { // Avoid first and last one to keep it watery
			for (col = 1; col < maxX; col++) {
				diffHeight = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				diffClimate = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				
				// Add or substract height
				int random = rand() % 100;
				if (random >= 80) { // 20% chance double growth (mountain)
					diffHeight *= 2; // = 0 - diffHeight;
				}
				else if(random <= 5) { // 10% chancehuge dip
					diffHeight = 0 - (diffHeight * 2);
				}
				else if (random <= 25) {// 20% chance small dip
					diffHeight = 0 - diffHeight;
				}
				// Add or substract climate
				if (rand() % 10 >= 5) {
					diffClimate = 0 - diffClimate;
				}

				_tilesMap[row][col]->addHeight(diffHeight);
				_tilesMap[row][col]->addClimate(diffClimate);

				// On uneven rows, we need to add +1 as offset because of the grid structure
				even = row % 2 == 0;
				if (!even) {
					offset = 1;
				}
				else {
					offset = 0;
				}

				// Affect neighboring tiles with half the strength
				diffHeight /= 2;
				diffClimate /= 2;

				// add to ajacent left tiles
				if (col >= 1) {
					// Same row
					_tilesMap[row][col - 1]->addHeight(diffHeight);
					_tilesMap[row][col - 1]->addClimate(diffClimate);
					// Upper row left
					if (row >= 1) {
						_tilesMap[row - 1][col - 1 + offset]->addHeight(diffHeight);
						_tilesMap[row - 1][col - 1 + offset]->addClimate(diffClimate);
						if (col >= 2)  {
							_tilesMap[row - 1][col - 2 + offset]->addHeight(diffHeight / 2);
						}
					}
					// Lower row left
					if (row < maxY) {
						_tilesMap[row + 1][col - 1 + offset]->addHeight(diffHeight);
						_tilesMap[row + 1][col - 1 + offset]->addClimate(diffClimate);
						if (col >= 2)  {
							_tilesMap[row + 1][col - 2 + offset]->addHeight(diffHeight / 2);
						}
					}
					// Affect tiles even further to the left
					if (col >= 2)  {
						_tilesMap[row][col - 2]->addHeight(diffHeight / 2);
					}
				}
				// Add to the ajacent right tiles
				if (col < maxX) {
					// Same row
					_tilesMap[row][col + 1]->addHeight(diffHeight);
					_tilesMap[row][col + 1]->addClimate(diffClimate);

					// Upper row right
					if (row >= 1) {
						_tilesMap[row - 1][col + offset]->addHeight(diffHeight);
						_tilesMap[row - 1][col + offset]->addClimate(diffClimate);
					}
					// Lower row right
					if (row < maxY) {
						_tilesMap[row + 1][col + offset]->addHeight(diffHeight);
						_tilesMap[row + 1][col + offset]->addClimate(diffClimate);
					}
				}
			}
		}
	}


	
	int type = 0; 
	int overlay = 0;
	for (row = 0; row <= mapTileY; row++) {
		for (col = 0; col <= mapTileX; col++) {
			// Too low? Then it's water
			if (_tilesMap[row][col]->height() < MAP_PLAIN_HEIGHT) {
				type = 1;
			}
			else {
				// Default to plains
				type = 0;
				overlay = rand() % 100;

				// If it's high enough, give it a mountain
				if (_tilesMap[row][col]->height() >= MAP_MOUNTAIN_HEIGHT) {
					_tilesMap[row][col]->overlay(1);
				}
				// Or 4% change it has a village
				else if (overlay <= 4) {
					_tilesMap[row][col]->overlay(2);
				}

				// Too aride for green grass? make it into a desert
				if (_tilesMap[row][col]->climate() <= MAP_DESERT_THRESHOLD) {
					type = 2;
				}
				// Or a forest?
				else if (_tilesMap[row][col]->climate() >= MAP_FOREST_THRESHOLD) {
					type = 4;
				}

				
			}
			_tilesMap[row][col]->type(type);
			//printf("%d ;", _tilesMap[row][col]->height());
		}
		//printf("\n");
	}

	_requireUpdate = true;

	printf("Generated tiles.\n");
}

void PlayState::loadTextures()
{
	SDL_DestroyTexture(_hexaTexture);
	_hexaTexture = Renderer::loadTexture("./res/img/hexa.png");
}

void PlayState::Cleanup()
{
	// Cleanup textures
	SDL_DestroyTexture(_hexaTexture);

	
}

void PlayState::Pause()
{

}

void PlayState::Resume()
{

}

void PlayState::HandleEvents(GameEngine* game)
{
	SDL_Event e;

	while (SDL_PollEvent(&e)){
		//If user closes he window
		if (e.type == SDL_QUIT) {
			game->Quit();
			break;
		}
		//If user presses any key
		if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym){
			case SDLK_m:
				break;
				//For quitting, escape key
			case SDLK_r:
				PlayState::Reset();
				break;
			case SDLK_ESCAPE: case SDLK_q:
				game->Quit();
				break;
			default:
				break;
			}
		}
		// Mouse click on a tile
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			if (e.button.button == SDL_BUTTON_LEFT)
			{
				// Calculate if we click on an existing tile
				//printf("x %d y %d\n", e.button.x, e.button.y);
				int mouseX = e.button.x;
				int mouseY = e.button.y;
				mouseX += _tileWidth / 2;
				mouseY += _tileWidth / 4;
				int yMargin = _tileHeight - (_tileHeight / 4);
				int xCoordinate = floor(mouseX / _tileWidth);
				int yCoordinate = floor(mouseY / yMargin);
				//printf("xCord %d yCord %d\n", xCoordinate, yCoordinate);

				// If we're not on an even row, we need an offset to find the real X tile
				if (yCoordinate % 2 != 0) {
					xCoordinate = floor((mouseX - (_tileWidth / 2)) / _tileWidth);
				}

				if (xCoordinate >= 0 && xCoordinate <= (mapTileX - 1) && yCoordinate >= 0 && yCoordinate <= (mapTileY - 1)) {
					_currentXCoordinate = xCoordinate;
					_currentYCoordinate = yCoordinate;
					_requireUpdate = true;
				}
			}
		}

		// Handle elephant movement
		//elephant.handleEvent(e);
	}
}

void PlayState::Update(GameEngine* game)
{

}

void PlayState::Draw(GameEngine* game)
{
	if (_requireUpdate) {
		// Always clear first, and set the scale
		Renderer::Clear();
		Renderer::setScale(0.5, 0.5);

		//SDL_Surface * background;

		// Draw all the tiles
		for (int row = 0; row <= mapTileY; row++) {
			for (int col = 0; col <= mapTileX; col++) {
				Renderer::renderTexture(_hexaTexture, _tilesMap[row][col]->position(), &_tilesMap[row][col]->clip());

				if (_tilesMap[row][col]->hasOverlay()) {
					Renderer::renderTexture(_hexaTexture, _tilesMap[row][col]->position(), &_tilesMap[row][col]->overlayClip());
				}
			}
		}

		// Draw GUI info on the current tile
		Tile * currentTile = _tilesMap[_currentYCoordinate][_currentXCoordinate];
		_ssGuiElement.str("");
		_ssGuiElement << "Tile[" << _currentYCoordinate << "][" << _currentXCoordinate << "]: Height(" << currentTile->height() << ") Climate(" << currentTile->climate() << ")";
		Renderer::renderText(_ssGuiElement.str(), m_font, m_color, 32, 0, 0);
		currentTile = NULL;
		delete currentTile;

		Renderer::Present();

		_requireUpdate = false;
	}
}

/** Reset the data on the screen **/
void PlayState::Reset()
{
	loadTextures();
	PlayState::generateLayersAndTiles(3);
}