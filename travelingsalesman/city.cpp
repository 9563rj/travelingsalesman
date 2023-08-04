#include "city.h"

extern int windowY;

City::City(double inputPosX, double inputPosY, int inputID, Uint8 inputRed, Uint8 inputGreen, Uint8 inputBlue) {
	posX = inputPosX;
	posY = inputPosY;
	id = inputID;
	red = inputRed;
	green = inputGreen;
	blue = inputBlue;
}

double City::getPosX() {
	return posX;
}

double City::getPosY() {
	return posY;
}

void City::drawCity(SDL_Renderer* renderer) {
	SDL_Rect rect;
	rect.w = 3;
	rect.h = 3;
	rect.x = posX - 1;
	rect.y = windowY - (posY + 1);
	SDL_SetRenderDrawColor(renderer, red, green, blue, SDL_ALPHA_OPAQUE);
	
	SDL_RenderFillRect(renderer, &rect);
}