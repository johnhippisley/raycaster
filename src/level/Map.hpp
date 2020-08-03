#ifndef MAP_H
#define MAP_H

typedef enum enum_tileType {NO_TILE, TILE_1} tileType; 

class Map
{
	private:
		tileType* tiles;
	public:
		int width, height, tileSize;
		Map(int width, int height, int tileSize);
		Map(int width, int height, int tileSize, tileType* tiles);
		void loadMapData(tileType* tiles);
		tileType getTile(int x, int y);
		int setTile(int x, int y, tileType type);
		bool tileInRange(int x, int y);
		bool coordInRange(double x, double y);
};

#endif
