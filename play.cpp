#include <stdio.h>

#include "play.h"

PlayState PlayState::m_PlayState;

void PlayState::Init(GameEngine* game)
{
	loadTextures();
	_tileWidth = 64;
	_tileHeight = 64;
	generateLayersAndTiles(3);

	// Make elephant
	_camera.reset(game->m_width, game->m_height);

	// Font
	m_font = "./res/font/font.ttf";


	// Background data
	_backgroundClip = { 0, 0, game->m_width, game->m_height };
	m_color = { 0, 0, 0 };

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

	for (row = 0; row < mapTileY; row++) {
		for (col = 0; col < mapTileX; col++) {

			// Calculate position
			posX = col * _tileWidth;
			posY = row * _tileHeightMargin;
			if (row % 2 != 0) {
				posX += _tileWidthMargin;
			}
			//printf("col: %d row: %d posX: %d posY %d\n", col, row, posX, posY);


			Tile* oneTile = new Tile();
			oneTile->placement(row, col);
			oneTile->position(posX, posY);
			oneTile->size(_tileWidth, _tileHeight);
			oneTile->height(0.0f);
			oneTile->type(1);

			_tilesMap[row][col] = oneTile;
		}
	}

	int maxX = mapTileX - 1;
	int maxY = mapTileY - 1;

	bool even = false;
	int offset = 0;

	// Now we loop through them again to give the "heat"
	for (row = 1; row < maxY; row++) {
		for (col = 1; col < maxX; col++) {
			float diffHeight = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			if (rand() % 100 >= 50) { // Add or substract
				diffHeight = 0 - diffHeight;
			}
			_tilesMap[row][col]->addHeight(diffHeight);

			diffHeight /= 2;

			// On uneven rows, we need to add +1 as offset because of the grid structure
			even = row % 2 == 0;
			if (!even) {
				offset = 1;
			}
			else {
				offset = 0;
			}


			// add to ajacent left tiles
			if (col >= 1) {
				// Same row
				_tilesMap[row][col -1]->addHeight(diffHeight);
				// Upper row left
				if (row >= 1) {
					_tilesMap[row - 1][col - 1 + offset]->addHeight(diffHeight);
				}
				// Lower row left
				if (row < maxY) {
					_tilesMap[row + 1][col - 1 + offset]->addHeight(diffHeight);
				}
			}
			// Add to the ajacent right tiles
			if (col < maxX) {
				// Same row
				_tilesMap[row][col + 1]->addHeight(diffHeight);

				// Upper row left
				if (row >= 1) {
					_tilesMap[row - 1][col + offset]->addHeight(diffHeight);
				}
				// Lower row left
				if (row < maxY) {
					_tilesMap[row + 1][col + offset]->addHeight(diffHeight);
				}
			}

			// If we're an unevent row, we have to add to three above and below
			/*if (row % 2 != 0) {
				if (row >= 1) {
					if (col >= 1) {
					}
					_tilesMap[row - 1][col ]->addHeight(diffHeight);
					if (col < maxX) {
						_tilesMap[row - 1][col + 1]->addHeight(diffHeight);
					}
				}
				if (row < maxY) {
					if (col >= 1) {
					}
					_tilesMap[row + 1][col]->addHeight(diffHeight);
					if (col < maxX) {
						_tilesMap[row + 1][col + 1]->addHeight(diffHeight);
					}
				}
			}
			// Otherwise, 2 above and below
			else {
				if (row >= 1) {
					_tilesMap[row - 1][col]->addHeight(diffHeight);
					if (col < maxX) {
						_tilesMap[row - 1][col + 1]->addHeight(diffHeight);
					}
				}
				if (row < (mapTileY - 1)) {
					_tilesMap[row + 1][col]->addHeight(diffHeight); 
					if (col < maxX) {
						_tilesMap[row + 1][col + 1]->addHeight(diffHeight);
					}
				}
			}*/
		}
	}


	// Now we want to loop through everything again, and test each tile.
	// If it's a mountain, we want to add aridity all around it, to simulate
	// mountains blocking off rain to the eastern side of them.
	bool aridity;
	float diffAridity;
	for (row = 1; row < maxY; row++) {
		for (col = 1; col < maxX; col++) {
			aridity = false;
			if (_tilesMap[row][col]->height() > 1.0f) {
				aridity = true;
				diffAridity = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

				// Impact stuff above and below to the east ( col + 1 )
				_tilesMap[row - 1][col + 1]->addAridity(diffAridity);
				_tilesMap[row][col]->addAridity(diffAridity / 2);
				_tilesMap[row][col + 1]->addAridity(diffAridity);
				_tilesMap[row + 1][col + 1]->addAridity(diffAridity);
			}
			// Or if it's already a wee bit arid, spread it over
			else if (_tilesMap[row][col]->aridity() > 0.3f) {

				_tilesMap[row - 1][col + 1]->addAridity(0.2f);
				_tilesMap[row][col + 1]->addAridity(0.2f);
				_tilesMap[row + 1][col + 1]->addAridity(0.2f);
			}
		}
	}

	float height;
	int type = 0;
	for (row = 0; row < mapTileY; row++) {
		for (col = 0; col < mapTileX; col++) {
			height = _tilesMap[row][col]->height();
			if (height < 0.1f) {
				type = 1;
			}
			else {
				type = 0;

				if (height > 1.0f) {
					_tilesMap[row][col]->overlay(1);
				}

				// Too aride for green grass?
				if (_tilesMap[row][col]->aridity() > 0.2f) {
					type = 2;
				}

				
			}
			_tilesMap[row][col]->type(type);
			//printf("%d ;", _tilesMap[row][col]->height());
		}
		//printf("\n");
	}

	printf("Generated tiles.\n");
}

void PlayState::loadTextures()
{
	_hexaTexture = Renderer::loadTexture("./res/img/hexa.png");
	_backgroundTexture = Renderer::loadTexture("./res/img/background.jpg");
	/*m_elephant = Renderer::loadTexture("./res/img/elephant.png");
	m_bubble = Renderer::loadTexture("./res/img/bubbles.png");
	m_background = Renderer::loadTexture("./res/img/background.jpg");*/
}

void PlayState::Cleanup()
{
	// Cleanup textures
	//SDL_DestroyTexture(m_elephant);
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
			case SDLK_ESCAPE: case SDLK_q:
				game->Quit();
				break;
			default:
				break;
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
	// Always clear first
	Renderer::Clear();
	
	//SDL_Surface * background;

	// Draw all the tiles
	for (int row = 0; row < mapTileY; row++) {
		for (int col = 0; col < mapTileX; col++) {
			Renderer::renderTexture(_hexaTexture, _tilesMap[row][col]->position(), &_tilesMap[row][col]->clip());

			if (_tilesMap[row][col]->hasOverlay()) {
				Renderer::renderTexture(_hexaTexture, _tilesMap[row][col]->position(), &_tilesMap[row][col]->overlayClip());
			}
		}
	}
	/*for (auto &i : _tiles) {
		Renderer::renderTexture(i->texture(), i->position(), &i->clip());
	}*/

	Renderer::Present();
}

void PlayState::Reset()
{
	_backgroundClip.y = 0;
}