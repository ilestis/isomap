#pragma once
#include <SDL.h>

class Camera
{
public:
	Camera();
	~Camera();
	int getPositionX();
	int getPositionY();

	void handleEvent(SDL_Event& e);
	void update();
	void reset(int x, int y);


private:
	int positionX, positionY;
};

