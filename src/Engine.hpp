#ifndef Engine_H
#define Engine_H

#include "gfx/Graphics.hpp"
#include "gfx/Camera.hpp"
#include "gfx/RayCaster.hpp"
#include "level/Map.hpp"

class Engine
{
	public:
		Engine(double targetFPS);
		void run();
		void exit();
	private:
		bool running;
		double targetFPS;
		Graphics* graphics;
		Camera* camera;
		Map* map;
		RayCaster* raycaster;
		unsigned long long milisecondsSinceEpoch();
		void update();
		void render();
		void init();
};

#endif
