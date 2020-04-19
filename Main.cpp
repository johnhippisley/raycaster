/*
	Ray casting program written by John Hippisley, 4/19/20
*/

#include "Engine.hpp"
const double TARGET_FPS = 60.0;

int main(int argc, char* argv[])
{
	Engine engine(TARGET_FPS);
	engine.run();
	return 0;
}
