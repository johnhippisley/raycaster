/* 
	Written by John Hippisley, 4/19/20
*/

#include "Engine.hpp"
#include <iostream>
#include <chrono> // std::chrono
#include <thread> // std::this_thread::sleep_for()
#include <SDL2/SDL.h>

const double CAMERA_SPEED = 2.0;

Engine::Engine(double targetFPS): targetFPS(targetFPS)
{
}

void Engine::run()
{
	std::cout << "Initializing..." << std::endl;
	init();
	double milisecondsPerFrame = 1000 / targetFPS;
	unsigned long long startTime, sleepFor;
	while(running)
	{
		startTime = milisecondsSinceEpoch();
		update();
		render();
		sleepFor = milisecondsPerFrame - (milisecondsSinceEpoch() - startTime);
		// Make sure engine runs at target FPS
		if(sleepFor > 0) std::this_thread::sleep_for(std::chrono::milliseconds(sleepFor));
	}
	exit();
}

void Engine::init() //initializs everything
{
	running = true;
	SDL_Init(SDL_INIT_EVERYTHING);
	graphics = new Graphics(320, 200, 2.0, "Ray Caster");
	int mapData[] = {1, 1, 1, 1, 1, 1, 1, 1,
				 	 1, 0, 0, 0, 0, 0, 1, 1,
					 1, 0, 1, 1, 0, 0, 0, 1,
				 	 1, 0, 0, 0, 0, 0, 0, 1,
					 1, 1, 1, 1, 1, 1, 1, 1};
	map = new Map(8, 5, 64);
	map->loadMapData((tileType*) mapData);
	// Initialize camera at (64, 64), a size of half a tile, height of half a tile, a view angle of 315, and FOV of 60
	camera = new Camera(80, 80, map->tileSize / 2, map->tileSize / 2, 60, graphics->getBufferWidth(), graphics->getBufferHeight());
	camera->setViewingAngle(315);
	// Put everything together into the raycaster object
	raycaster = new RayCaster(graphics, camera, map);
}


void Engine::update()
{
	// Handle events
	SDL_Event event;
	while(SDL_PollEvent(&event) != 0)
 	{
    	if(event.type == SDL_QUIT) running = false;
    	// User control of camera position/angle
    	else if(event.type == SDL_KEYDOWN)
    	{
    		switch(event.key.keysym.sym)
    		{
    			case SDLK_w:
    				camera->df = CAMERA_SPEED;
    				break;
    			case SDLK_a:
    				camera->dp = -CAMERA_SPEED;
    				break;
    			case SDLK_s:
    				camera->df = -CAMERA_SPEED;
    				break;
    			case SDLK_d:
    				camera->dp = CAMERA_SPEED;
					break;
    			case SDLK_LEFT:
    				camera->da = CAMERA_SPEED;
					break;
    			case SDLK_RIGHT:
    				camera->da = -CAMERA_SPEED;
    				break;
			}
    	}
    	else if(event.type == SDL_KEYUP)
    	{
    		switch(event.key.keysym.sym)
			{
    			case SDLK_w:
    				camera->df = 0;
    				break;
    			case SDLK_a:
    				camera->dp = 0;
    				break;
    			case SDLK_s:
    				camera->df = 0;
    				break;
    			case SDLK_d:
    				camera->dp = 0;
					break;
    			case SDLK_LEFT:
    				camera->da = 0;
					break;
    			case SDLK_RIGHT:
    				camera->da = 0;
    				break;
			}
    	}
	}
	camera->update(map);
}

void Engine::render()
{
	graphics->clearBuffer();
	raycaster->render();
	graphics->updateWindow();
}

unsigned long long Engine::milisecondsSinceEpoch()
{
	return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
}

void Engine::exit()
{
	graphics->~Graphics();
	SDL_Quit();
	std::cout << "Exiting..." << std::endl;
}
