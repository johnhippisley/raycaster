#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include "Texture.hpp"
#include "Camera.hpp"

typedef Uint32 u32;
class Graphics
{
	private:
		SDL_Window* window; 		// Pointer to window
		SDL_Surface* windowSurface; // imageSurface gets drawn to this and scaled
		SDL_Surface* imageSurface;  // What we draw to
		Texture* renderBuffer;		// Texture object represnting imageSurface
	public:
		double windowScale;
		Graphics(int width, int height, double scale, const char* windowTitle);
		~Graphics();
		void destroy();
		void updateWindow();
		void clearBuffer();
		int getBufferWidth();
		int getBufferHeight();
		int getCenterX();
		int getCenterY();
		int setPixel(int x, int y, u32 color);
		void drawRectangle(int xPos, int yPos, int width, int height, u32 color);
		void drawLine(int x1, int y1, int x2, int y2, u32 color);
		void drawVector(int x, int y, double angle, double magnitude, u32 color);
		void drawTexture(Texture* texture, int xPos, int yPos);
};

#endif
