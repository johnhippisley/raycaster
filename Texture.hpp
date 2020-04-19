#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
typedef Uint32 u32;
typedef Uint8 u8;

class Texture
{
	private:
		u32* pixels;
		bool inRange(int x, int y);
		bool inRange(int index);
	public:
		Texture(int width, int height, u32* pixels);
		Texture(SDL_Surface* surface);
		Texture(const char* pathToImage);
		Texture(const char* pathToImage, int xPos, int yPos, int cropWidth, int Height);
		int width, height;
		u32* getPixels();
		int setPixel(int x, int y, u32 color);
		int setPixel(int index, u32 color);
		u32 getPixel(int x, int y);
		u32 getPixel(int index);
};

#endif
