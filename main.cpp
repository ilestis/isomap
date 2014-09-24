#include <string>
#include "gameengine.h"
#include "play.h"
#include <ctime>

int main(int argc, char** argv)
{
	printf("Start");
	srand(time(NULL));

	// Init engine
	GameEngine game;
	game.Init("isoMap V0.1", 1280, 960, 0, false);

	// Set state
	game.ChangeState(PlayState::Instance());

	while (game.Running()) {
		if (game.DoLoop()) {
			game.HandleEvents();
			game.Update();
			game.Draw();
		}
	}

	game.Cleanup();

	return 0;
}