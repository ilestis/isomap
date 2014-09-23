#include <stdio.h>
#include "gameengine.h"
#include "gamestate.h"
#include "renderer.h"
#include <SDL.h>

void GameEngine::Init(const char* title, int width, int height,
	int bpp, bool fullscreen)
{
	int flags = 0;
	m_fullscreen = fullscreen;
	m_running = true;

	m_width = width;
	m_height = height;
	m_fps = 60;

	// Initialize SDL and subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		throw std::runtime_error("SDL init failed");
	if (TTF_Init() == -1)
		throw std::runtime_error("TTF init failed");
	
	// Create the SDL window
	SDL_CreateWindowAndRenderer(m_width, m_height, SDL_WINDOWPOS_CENTERED, &Renderer::g_window, &Renderer::g_renderer);
	SDL_RenderSetLogicalSize(Renderer::g_renderer, m_width, m_height);
	SDL_SetWindowTitle(Renderer::g_window, title);

	// Fullscreen mode
	if (fullscreen) {
		SDL_SetWindowFullscreen(Renderer::g_window, SDL_WINDOW_FULLSCREEN);
	}

	printf("Engine initialised\n");
}

void GameEngine::Cleanup()
{
	// Clear the states
	while (!states.empty()) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// switch back to windowed mode so other 
	// programs won't get accidentally resized
	if (m_fullscreen) {
		SDL_SetWindowFullscreen(Renderer::g_window, 0);
	}

	printf("Engine cleanued up\n");

	SDL_Quit();
}

void GameEngine::ChangeState(GameState* state)
{
	// Remove current state
	if (!states.empty()) {
		states.back()->Cleanup();
		states.pop_back();
	}

	states.push_back(state);
	states.back()->Init(this);
}

void GameEngine::PushState(GameState* state)
{
	// Pause our state
	if (!states.empty()) {
		states.back()->Pause();
	}

	states.push_back(state);
	states.back()->Init(this);
}

void GameEngine::PopState()
{
	// Remove current state
	if (!states.empty()) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// resume previous state
	if (!states.empty()) {
		states.back()->Resume();
	}
}

void GameEngine::HandleEvents()
{
	// Push the events to the state
	states.back()->HandleEvents(this);
}

void GameEngine::Update()
{
	// State handles update
	states.back()->Update(this);
}

void GameEngine::Draw()
{
	states.back()->Draw(this);
}

// Limit the FPS 
bool GameEngine::DoLoop()
{
	m_currentTick = SDL_GetTicks();
	if (m_currentTick < m_lastTick) {
		m_lastTick = 0;
	}

	if (m_lastTick + 1000 / m_fps < m_currentTick) {
		m_lastTick = m_currentTick;
		return true;
	}
	return false;
}