/*
	Written by John Hippisley, 4/20/20
	Folowing https://permadi.com/1996/05/ray-casting-tutorial-table-of-contents/
*/

#include "RayCaster.hpp"
#include <iostream>
#include <vector> // std::vector

// Color constants
const u32 CEILING_COLOR	=		 	 0x424242;
const u32 FLOOR_COLOR =			 	 0x706c6d;
const u32 WALL_COLOR =				 0x113f49;
const u32 MINIMAP_TILE_COLOR =	 	 0x135470;
const u32 MINIMAP_CAMERA_COLOR = 	 0xff0000;
const u32 MINIMAP_CAMERA_BOX_COLOR = 0x106003;
const u32 MINIMAP_RAY_COLOR =	 	 0x21201d;

// Other constants
const int RAY_LIMIT = 		    6400; // Max distance a ray can travel
const int MINIMAP_TILE_SIZE =   12;

RayCaster::RayCaster(Graphics* graphics, Camera* camera, Map* map):
graphics(graphics), camera(camera), map(map)
{
}

void RayCaster::render()
{
	// Draw floor and ceiling
	graphics->drawRectangle(0, 0, graphics->getBufferWidth(), graphics->getBufferHeight() / 2, CEILING_COLOR);
	graphics->drawRectangle(0, graphics->getBufferHeight() / 2, graphics->getBufferWidth(), graphics->getBufferHeight() / 2, FLOOR_COLOR);
	// Calculate boundaries for rays to be cast
	double angleBetweenRays = camera->fov / camera->projWidth;
	double firstRay = camera->getViewingAngle() - camera->fov / 2;
	double lastRay = camera->getViewingAngle() + camera->fov / 2;
	// Cast each ray
	int column = 0;
	std::vector<double> rayLengths;
	for(double rayAngle = lastRay; rayAngle >= firstRay; rayAngle -= angleBetweenRays)
	{
		double distanceToTile = castRay(::wrapAngle(rayAngle));
		rayLengths.push_back(distanceToTile);
		if(distanceToTile != -1)
		{
			double correctedDistance = distanceToTile * dcos(camera->getViewingAngle() - rayAngle); // Fix the fish-eye effect
			double projectedHeight = (double) map->tileSize / correctedDistance * camera->getDistanceToProjectionPlane();
			graphics->drawLine(column, graphics->getCenterY() - projectedHeight / 2, column, graphics->getCenterY() + projectedHeight / 2, WALL_COLOR);
		}
		column++;
	}

	// Draw 2D Minimap
	const double MINIMAP_SCALE = (double) MINIMAP_TILE_SIZE / map->tileSize;
	int sCameraX = camera->x * MINIMAP_SCALE;
	int sCameraY = camera->y * MINIMAP_SCALE;
	double sCameraSize = camera->size * MINIMAP_SCALE;
	// Draw tiles
	for(int y = 0; y < map->height; y++)
	{
		for(int x = 0; x < map->width; x++)
		{
			graphics->drawRectangle(x * MINIMAP_TILE_SIZE, y * MINIMAP_TILE_SIZE, MINIMAP_TILE_SIZE, MINIMAP_TILE_SIZE, 0);
			if(map->getTile(x, y) != NO_TILE)
				graphics->drawRectangle(x * MINIMAP_TILE_SIZE + 1, y * MINIMAP_TILE_SIZE + 1, MINIMAP_TILE_SIZE - 2,
					MINIMAP_TILE_SIZE - 2, MINIMAP_TILE_COLOR);
		}
	}
	// Draw player and FOV
	double angle = lastRay;
	for(int i = 0; i < (lastRay - firstRay) / angleBetweenRays; i++)
	{
		graphics->drawVector(sCameraX, sCameraY, angle, rayLengths.at(i) * MINIMAP_SCALE, MINIMAP_RAY_COLOR);
		angle -= angleBetweenRays;
	}
	graphics->drawRectangle(sCameraX - sCameraSize / 2, sCameraY - sCameraSize / 2, sCameraSize, sCameraSize, MINIMAP_CAMERA_BOX_COLOR);
	graphics->drawRectangle(sCameraX, sCameraY, 1, 1, MINIMAP_CAMERA_COLOR);
}

double RayCaster::distance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// Casts a ray and returns the distance to the first visible wall it hits
double RayCaster::castRay(double rayAngle)
{
	double Hx, Hy, Vx, Vy; // Coordinates for closest visibile wall on horizontal and vertical intersections
	bool hDied = false, vDied = false; // Whether or not the horizontal/vertical checking went over the render limit
	if(rayAngle != 0.0 && rayAngle != 180.0)
	{
		// Test horizontal intersections
		double Ax, Ay; // First intersection with grid lines
		double Xa, Ya; // Distances between next horizontal grid intersection
		if(rayAngle > 0.0 && rayAngle < 180.0) //Ray is facing up
	 	{
	 		Ay = ((int) camera->y) / map->tileSize * map->tileSize - 1;
	 		Ya = -map->tileSize;
	 	}
		else //Ray is facing down
		{
	 		Ay = ((int) camera->y) / map->tileSize * map->tileSize + map->tileSize;
			Ya = map->tileSize;
		}
		Xa = -Ya / dtan(rayAngle);
		Ax = camera->x + ((double) (camera->y - Ay)) / dtan(rayAngle);
		Hx = Ax;
		Hy = Ay;
		while(map->getTile(Hx / map->tileSize, Hy / map->tileSize) == NO_TILE)
		{
			// Advance ray to the next horizontal intersection
			Hx += Xa;
			Hy += Ya;
			if(distance(camera->x, camera->y, Hx, Hy) > RAY_LIMIT || !map->coordInRange(Hx, Hy)) // Kill ray once it exceeds a certain distance, or is out of bounds
			{
				hDied = true;
				break;
			}
		}
	} else hDied = true; // No point in checking completley horizontal rays for horozintal intersections
	if(rayAngle != 90.0 && rayAngle != 270.0)
	{
		// Test vertical intersections
		double Bx, By; // First intersection with grid lines
		double Xa, Ya; // Distance between next vertical grid lines
		if((rayAngle >= 0.0 && rayAngle < 90.0) || (rayAngle <= 360.0 && rayAngle > 270.0)) // Ray is facing right
		{
			Bx = ((int) camera->x) / map->tileSize * map->tileSize + map->tileSize;
			Xa = map->tileSize;
		}
		else //Ray is facing left
		{
			Bx = ((int) camera->x) / map->tileSize * map->tileSize - 1;
			Xa = -map->tileSize;
		}
		Ya = -(Xa * dtan(rayAngle));
		By = camera->y + ((double) (camera->x - Bx)) * dtan(rayAngle);
		Vx = Bx;
		Vy = By;
		while(map->getTile(Vx / map->tileSize, Vy / map->tileSize) == NO_TILE)
		{
			// Advance ray to the next horizontal intersection
			Vx += Xa;
			Vy += Ya;
			if(distance(camera->x, camera->y, Vx, Vy) > RAY_LIMIT || !map->coordInRange(Vx, Vy)) // Kill ray once it exceeds a certain distance, or is out of bounds
			{
				vDied = true;
				break;
			}
		}
	}
	else vDied = true; // No point in checking completley vertical rays for vertical intersections
	// Return distance to the nearest visible wall
	double hDistance = distance(camera->x, camera->y, Hx, Hy);
	double vDistance = distance(camera->x, camera->y, Vx, Vy);
	if(hDied && vDied) return -1;
	else if(hDied && !vDied) return vDistance;
	else if(!hDied && vDied) return hDistance;
	else return (hDistance < vDistance ? hDistance : vDistance);
}
