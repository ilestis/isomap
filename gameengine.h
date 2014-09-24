#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SDL.h>
#include <vector>
#include <memory>
#include "renderer.h"

class GameState;

class GameEngine
{
public:
	void Init(const char* title, int width = 500, int height = 960, int bpp = 0, bool fullscreen = false);
	void Cleanup();

	void ChangeState(GameState* state);
	void PushState(GameState* state);
	void PopState();

	void GenerateMap();

	void HandleEvents();
	void Update();
	void Draw();

	bool Running() { return m_running; }
	void Quit() { m_running = false; }
	bool DoLoop();

	int m_width, m_height;

private:
	std::vector<GameState*> states;
	bool m_running, m_fullscreen;
	Uint32 m_currentTick, m_lastTick;
	int m_fps;

};

#endif