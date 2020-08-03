#ifndef CAMERA_H
#define CAMERA_H

#include "../level/Map.hpp"

// Everything will be in degrees
#define dtor(x) ((x) * (M_PI / 180.0))
#define dtan(x) (tan(dtor(x)))
#define dcos(x) (cos(dtor(x)))
#define dsin(x) (sin(dtor(x)))

double wrapAngle(double angle);

class Camera
{
	private:
		double viewingAngle;
		void move(double dx, double dy, Map* map);
		double wrapAngle();
	public:
		int projWidth, projHeight;
		double x, y, df, dp, da;
		double height, size, fov;
		Camera(double x, double y, double size, double height, double fov, int projWidth, int projHeight);
		void update(Map* map);
		void setViewingAngle(double angle);
		double getViewingAngle();
		double getDistanceToProjectionPlane();
};

#endif
