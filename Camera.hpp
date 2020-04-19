#ifndef CAMERA_H
#define CAMERA_H

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
		double wrapAngle();
	public:
		int projWidth, projHeight;
		double x, y,  df, ds, da;
		double height /*(z)*/, fov;
		Camera(double x, double y, double height, double fov, int projWidth, int projHeight);
		void update();
		void setViewingAngle(double angle);
		void increaseViewingAngle(double degrees);
		double getViewingAngle();
		double getDistanceToProjectionPlane();
};

#endif
