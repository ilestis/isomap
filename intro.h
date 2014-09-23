#ifndef INTROSTATE_H
#define INTROSTATE_H

#include <SDL.h>
#include "gamestate.h"
#include "gameengine.h"
#include "gamestate.h"

class IntroState : public GameState
{
public:
	void Init(GameEngine* game);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(GameEngine* game);
	void Update(GameEngine* game);
	void Draw(GameEngine* game);

	static IntroState* Instance() {
		return &m_IntroState;
	}

protected:
	IntroState() { }

private:
	static IntroState m_IntroState;
};

#endif