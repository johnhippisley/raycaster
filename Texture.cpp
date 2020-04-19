/*
	Written by John Hippisley, 4/14/20
*/

#include "Texture.hpp"
#include <iostream>

Texture::Texture(SDL_Surface* surface)
{
	width = surface->w;
	height = surface->h;
	pixels = (u32*) surface->pixels;
}

Texture::Texture(int width, int height, u32* pixels)
{
	this->width = width;
	this->height = height;
	this->pixels = pixels;
}

// Feature not used... yet
Texture::Texture(const char* pathToImage)
{
	SDL_Surface* surface = SDL_LoadBMP(pathToImage);
	if(surface)
	{
		width = surface->w;
		height = surface->h;
		pixels = (u32*) malloc(sizeof(u32) * width * height);
		/*
		  Must re-map pixels incase they are stored using a different bbp
		  If the bbp != 4, then we must re-divide the data with the correct
		  bbp, and assign each pixel to a u32. All pixels will be stored in
		  a u32 for simplicity. If it is 4, the we have to get rid of the 
		  alpha channel
		*/
		int bbp = surface->format->BytesPerPixel;
		for(int i = 0; i < (width * height); i++) pixels[i] = *(u32*)((u8*) surface->pixels + i * bbp) >> ((bbp - 3) * 8);
	} else std::cout << "Error loading '" << pathToImage << "'" << std::endl;
}

Texture::Texture(const char* pathToImage, int xPos, int yPos, int cropWidth, int cropHeight) 
{
	Texture* image = new Texture(pathToImage);
	//Now crop accordingly
	u32* newPixels = (u32*) malloc(sizeof(u32) * cropWidth * cropHeight);
	for(int x = 0; x < cropWidth; x++)
	{
		for(int y = 0; y < cropHeight; y++)
		{
			newPixels[x + y * cropWidth] = image->getPixel(xPos + x, yPos + y);
		}
	}
	pixels = newPixels;
	width = cropWidth;
	height = cropHeight;
}

u32* Texture::getPixels()
{
	return pixels;
}

bool Texture::inRange(int x, int y)
{
	return (x >= 0 && y >= 0 && x < width && y < height);
}

bool Texture::inRange(int index)
{
	if(index >= 0 && index < (width * height)) return true;
	else return false;
}

int Texture::setPixel(int x, int y, u32 color)
{
	if(inRange(x, y)) pixels[x + y * width] = color;
	else return 1;
}

int Texture::setPixel(int index, u32 color)
{
	if(inRange(index)) pixels[index] = color;
	else return 1;
}

u32 Texture::getPixel(int x, int y)
{
	if(inRange(x, y)) return pixels[x + y * width];
	else return 0;
}

u32 Texture::getPixel(int index)
{
	if(inRange(index)) return pixels[index];
	else return 0;
}
