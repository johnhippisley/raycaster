/*
	Written by John Hippisley, 4/18/20
*/

#include "Camera.hpp"
#include <iostream>
#include <cmath> // tan()

// Wraps co-terminal angles
double wrapAngle(double angle)
{
	while(angle < 0.0) angle += 360.0;
	while(angle >= 360.0) angle -= 360.0;
	return angle;
}

Camera::Camera(double x, double y, double size, double height, double fov, int projWidth, int projHeight):
x(x), y(y), df(0), dp(0), da(0), size(size), height(height), fov(fov), projWidth(projWidth), projHeight(projHeight)
{
}

void Camera::update(Map* map)
{
	viewingAngle += da;
	move(df * dcos(getViewingAngle()), -df * dsin(getViewingAngle()), map);
	move(dp * dcos(getViewingAngle() - 90), -dp * dsin(getViewingAngle() - 90), map);
}

/*
	The X and Y coordinates represent the center of the camera,
	where we're actually viewing from. The size member indicates
	how big the camera is.
*/
void Camera::move(double dx, double dy, Map* map)
{
	if(dx == 0.0 && dy == 0.0) return;
	else if(dx != 0.0 && dy != 0.0)
	{
		move(dx, 0.0, map);
		move(0.0, dy, map);
	}
	else if(dx > 1.0 || dy > 1.0 || dx < -1.0 || dy < -1.0) // Step through, moving part way if we can
	{
		for(int a = 0; a < (int) abs(dx); a++) move(dx > 0 ? 1.0 : -1.0, 0.0, map);
		move(dx - (int) dx, 0.0, map);
		for(int b = 0; b < (int) abs(dy); b++) move(0.0, dy > 0 ? 1.0 : -1.0, map);
		move(0.0, dy - (int) dy, map);
	}
	else
	{
		double nx = x + dx;
		double ny = y + dy;
		// Check if any of the corners would collide
		if(map->getTile((nx - size / 2) / map->tileSize, (ny - size / 2) / map->tileSize) == NO_TILE &&
		   map->getTile((nx + size / 2) / map->tileSize, (ny - size / 2) / map->tileSize) == NO_TILE &&
		   map->getTile((nx - size / 2) / map->tileSize, (ny + size / 2) / map->tileSize) == NO_TILE &&
		   map->getTile((nx + size / 2) / map->tileSize, (ny + size / 2) / map->tileSize) == NO_TILE)
		{
			// We're good to go with no collision!
			x = nx;
			y = ny;
		}
	}
}

double Camera::getViewingAngle()
{
	viewingAngle = ::wrapAngle(viewingAngle);
	return viewingAngle;
}

void Camera::setViewingAngle(double angle)
{
	viewingAngle = ::wrapAngle(angle);
}

double Camera::getDistanceToProjectionPlane()
{
	return (double) projWidth / 2 / dtan(fov / 2);
}
