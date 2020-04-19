/*
	Written by John Hippisley, 4/17/20
*/

#include "Map.hpp"
#include <cstdlib> // malloc()
#include <cstring> // memcpy()

Map::Map(int width, int height, int tileSize)
{
	this->width = width;
	this->height = height;
	this->tileSize = tileSize;
	tiles = (tileType*) malloc(width * height * sizeof(tileType));
}

Map::Map(int width, int height, int tileSize, tileType* tiles)
{
	this->width = width;
	this->height = height;
	this->tileSize = tileSize;
	loadMapData(tiles);
}

void Map::loadMapData(tileType* tiles)
{
	std::size_t size = sizeof(*tiles) * width * height;
	this->tiles = (tileType*) malloc(size);
	std::memcpy(this->tiles, tiles, size);
}

bool Map::tileInRange(int x, int y)
{
	return (x >= 0 && y >= 0 && x < width && y < height);
}

bool Map::coordInRange(double x, double y)
{
	return (x >= 0.0 && y >= 0.0 && x <= width * tileSize && y <= height * tileSize);
}

int Map::setTile(int x, int y, tileType type)
{
	if(tileInRange(x, y)) tiles[x + y * width] = type;
	return (int) !tileInRange(x, y);
}

tileType Map::getTile(int x, int y)
{
	if(tileInRange(x, y)) return tiles[x + y * width];
	else return NO_TILE;
}
