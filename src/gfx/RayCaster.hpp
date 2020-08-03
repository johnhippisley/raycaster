#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "../level/Map.hpp"
#include "Graphics.hpp"
#include "Camera.hpp"

class RayCaster
{
	private:
		Graphics* graphics;
		double castRay(double ranAngle);
		double distance(double x1, double y1, double x2, double y2);
		double wrapAngle(double angle);
	public:
		Camera* camera;
		Map* map;
		RayCaster(Graphics* graphics, Camera* camera, Map* map);
		void render();
};

#endif
