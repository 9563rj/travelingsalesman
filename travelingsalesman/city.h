#pragma once
#include <SDL.h>
#include <cmath>

class City {
public:
	City(double inputPosX, double inputPosY, int inputID, Uint8 inputRed, Uint8 inputGreen, Uint8 inputBlue);
	double getPosX();
	double getPosY();
	void drawCity(SDL_Renderer* renderer);
	int id;
	Uint8 red;
	Uint8 green;
	Uint8 blue;
private:
	double posX;
	double posY;
};