/*
	Written by John Hippisley, 4/19/20
*/

#include "Graphics.hpp"
#include <iostream>

Graphics::Graphics(int width, int height, double scale, const char* windowTitle): windowScale(scale)
{
	window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width * scale, height * scale, 0);
    windowSurface = SDL_GetWindowSurface(window);
    imageSurface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
   	SDL_SetSurfaceBlendMode(windowSurface, SDL_BLENDMODE_NONE);
    SDL_SetSurfaceBlendMode(imageSurface, SDL_BLENDMODE_NONE);
	renderBuffer = new Texture(imageSurface);
}

Graphics::~Graphics()
{
	std::cout << "Cleaning up SDL..." << std::endl;
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Graphics::updateWindow()
{
	SDL_BlitScaled(imageSurface, NULL, windowSurface, NULL);
    SDL_UpdateWindowSurface(window);
}


int Graphics::getBufferWidth()
{
	if(renderBuffer != 0) return renderBuffer->width;
	else return -1;
}

int Graphics::getBufferHeight()
{
	if(renderBuffer != 0) return renderBuffer->height;
	else return -1;
}

int Graphics::getCenterX()
{
	return getBufferWidth() / 2;
}

int Graphics::getCenterY()
{
	return getBufferHeight() / 2;
}

int Graphics::setPixel(int x, int y, u32 color)
{
	return renderBuffer->setPixel(x, y, color);
}

void Graphics::clearBuffer()
{
	for(int i = 0; i < renderBuffer->width * renderBuffer->height; i++) renderBuffer->setPixel(i, 0);
}

void Graphics::drawRectangle(int xPos, int yPos, int width, int height, u32 color)
{
	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			setPixel(xPos + x, yPos + y, color);
		}
	}
}

void Graphics::drawLine(int x1, int y1, int x2, int y2, u32 color) // Revised Bresenham algorithm, supporting all directions
{
	// Seperate implementation for straight up horizontal/vertical lines
	if(x1 == x2) for(int y = (y1 < y2 ? y1 : y2); y <= (y2 > y1 ? y2 : y1); y++) setPixel(x1, y, color);
	else if(y1 == y2) for(int x = (x1 < x2 ? x1 : x2); x <= (x2 > x1 ? x2 : x1); x++) setPixel(x, y1, color);
	else
	{
		int direction; // 0 = SE, 1 = NE, 2 = SW, 3 = NW
		if(x2 > x1 && y2 > y1) direction = 0;
		else if(x2 > x1 && y2 < y1) direction = 1;
		else if(x1 > x2 && y2 > y1) direction = 2;
		else if(x1 > x2 && y2 < y1) direction = 3;
		else return;
		double dx = abs(x2 - x1);
		double dy = abs(y2 - y1); 
		double step = dx >= dy ? dx : dy;
		if(step == 0.0) return;
		if(direction == 1) dy *= -1;
		else if(direction == 2) dx *= -1;
		else if(direction == 3) 
		{
			dx *= -1;
			dy *= -1;
		} 
		double x = (double) x1;
		double y = (double) y1;
		for(int i = 0; i < step; i++)
		{
			setPixel((int) x, (int) y, color);
			x += dx / step;
			y += dy / step;
		}
	}
}

void Graphics::drawVector(int x, int y, double angle, double magnitude, u32 color)
{
	drawLine(x, y, x + dcos(angle) * magnitude, y - dsin(angle) * magnitude, color);
}

void Graphics::drawTexture(Texture* texture, int xPos, int yPos)
{
	for(int x = 0; x < texture->width; x++)
	{
		for(int y = 0; y < texture->height; y++)
		{
			setPixel(xPos + x, yPos + y, texture->getPixel(x, y));
		}
	}
}
