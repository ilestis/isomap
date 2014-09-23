#include "camera.h"


Camera::Camera()
{
	positionX = 0;
	positionY = 0;
}


Camera::~Camera()
{
}

// Handle input from SDL
void Camera::handleEvent(SDL_Event& e)
{

}

void Camera::reset(int x, int y)
{
	positionX = x;
	positionY = y;
}

void Camera::update()
{

}

int Camera::getPositionX()
{
	return positionX;
}

int Camera::getPositionY()
{
	return positionY;
}