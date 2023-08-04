#include <iostream>
#include <SDL.h>
#include <vector>
#include <list>
#include <random>
#include <chrono>
#include <cmath>
#include "city.h"

int windowX = 600;
int windowY = 400;
int bufferX = windowX / 10;
int bufferY = windowY / 10;

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("Traveling Salesman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowX, windowY, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	// initialize screen to black
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_Rect initRect;
	initRect.x = 0;
	initRect.y = 0;
	initRect.h = windowY;
	initRect.w = windowX;
	SDL_RenderFillRect(renderer, &initRect);

	std::vector<City*> cities;
	std::list<City*> route;

	std::mt19937 mersenneTwister{static_cast<std::mt19937::result_type>(std::chrono::steady_clock::now().time_since_epoch().count())};
	//std::mt19937 mersenneTwister{};
	std::uniform_real_distribution<double> posDistributionX(bufferX, windowX - bufferX);
	std::uniform_real_distribution<double> posDistributionY(bufferY, windowY - bufferY);

	// margins 10% window size
	for (int i = 0; i < 762; i++) {
		cities.push_back(new City(posDistributionX(mersenneTwister), posDistributionY(mersenneTwister), i, 255, 255, 255));
	}
	/*cities.push_back(new City(100, 100, 0, 255, 255, 255));
	cities.push_back(new City(300, 100, 2, 255, 255, 255));
	cities.push_back(new City(200, 110, 1, 255, 255, 255));*/
	

	// start from first city created
	route.push_back(cities[0]);

	// nearest neighbor algorithm
	double totalDist = 0;
	for (int i = 1; i < cities.size(); i++) {
		double distToNearestNeighbor = DBL_MAX;
		City* nearestNeighbor = NULL;
		for (std::list<City*>::iterator it = route.begin(); it != route.end(); it++) {
			double distToThisCity = sqrt(((cities[i]->getPosY() - (*it)->getPosY()) * (cities[i]->getPosY() - (*it)->getPosY())) + ((cities[i]->getPosX() - (*it)->getPosX()) * (cities[i]->getPosX() - (*it)->getPosX())));
			if (distToThisCity < distToNearestNeighbor) {
				distToNearestNeighbor = distToThisCity;
				nearestNeighbor = (*it);
			}
		}
		std::list<City*>::iterator nearestNeighborIt = std::find(route.begin(), route.end(), nearestNeighbor);
		if (nearestNeighbor == NULL) {
			std::cout << "nearest neighbor is null" << std::endl;
			std::system("pause");
			std::exit(1);
		}
		if (nearestNeighborIt == route.end()) {
			std::cout << "failed to find nearest neighbor in route list" << std::endl;
			std::system("pause");
			std::exit(1);
		}
		
		if ((*nearestNeighborIt) == route.back()) {
			route.push_back(cities[i]);
		}
		else {
			nearestNeighborIt++;
			route.insert(nearestNeighborIt, cities[i]);
		}
		/*int j = 0;
		for (std::list<City*>::iterator it = route.begin(); it != route.end(); it++) {
			std::cout << j << " " << (*it)->id << std::endl;
			j++;
		}
		totalDist += distToNearestNeighbor;
		std::cout << "----" << std::endl;*/
	}
	std::cout << "Total Distance: " << totalDist << std::endl;

	bool exit = false;
	SDL_Event SDLevent;
	while (!exit) {

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		int i = 0;
		for (std::list<City*>::iterator it = route.begin(); it != route.end(); it++) {
			Uint8 red = i/3;
			Uint8 green = 255;
			Uint8 blue = i/3;

			(*it)->red = red;
			(*it)->green = green;
			(*it)->blue = blue;
			(*it)->drawCity(renderer);
			i++;
		}
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		std::list<City*>::iterator it1 = route.begin();
		std::list<City*>::iterator it2 = route.begin();
		it2++;
		while (it2 != route.end()) {
			SDL_RenderDrawLine(renderer, (*it1)->getPosX(), windowY - (*it1)->getPosY(), (*it2)->getPosX(), windowY - (*it2)->getPosY());
			it1++;
			it2++;
		}

		SDL_RenderPresent(renderer);

		while (SDL_PollEvent(&SDLevent)) {
			switch (SDLevent.type) {
			case(SDL_KEYDOWN):
				switch (SDLevent.key.keysym.sym) {
				case(SDLK_ESCAPE):
					exit = true;
					break;
				}
			}
		}

	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::exit(0);
}