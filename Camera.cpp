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

Camera::Camera(double x, double y, double height, double fov, int projWidth, int projHeight)
{
	this->x = x;
	this->y = y;
	this->df = 0;
	this->ds = 0;
	this->da = 0;
	this->height = height;
	this->fov = fov;
	this->projWidth = projWidth;
	this->projHeight = projHeight;
}

void Camera::update()
{
	// Move camera in plane parallel (df) and perpindicular (ds)
	x += df * dcos(getViewingAngle());
	x += ds * dcos(getViewingAngle() - 90);
	y -= df * dsin(getViewingAngle());
	y -= ds * dsin(getViewingAngle() - 90);
	viewingAngle += da;
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
