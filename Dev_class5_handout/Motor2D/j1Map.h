#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

struct LayerProperties
{
	bool is_drawn = false;
	bool has_colliders = false;
};

struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

struct MapLayer
{
	p2SString name;
	int width;
	int height;
	uint* data = nullptr;

	LayerProperties properties;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<MapLayer*> layer_array;
	fPoint player_starting_value; 
	SDL_Texture* background_image; 
	float parallax_speed; 
	int camera_y_limit; 
	iPoint bone_position;
	fPoint background_offset;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	//Called to create colliders


	// Load new map
	bool Load(const char* path);

	iPoint MapToWorld(int x, int y) const;

	void Get(int* x, int* y); //la x es el numero, la y cuantos elementos hay en una fila

	SDL_Rect TileRect(int tileid, int iterator);

	void convert_to_real_world(int*, int*);

	//starting map
  
	int map = 0; //Starting map 

	SDL_Rect bone_rect = { 4513, 973,249,493 };


private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node);
	bool LoadBackground(pugi::xml_node& node);
	bool LoadMapProperties(pugi::xml_node& node);
	bool LoadLayerProperties(pugi::xml_node& node, LayerProperties& properties);

	void FindColliders(MapLayer* layer);
	void FindSpawn(MapLayer* layer);
	void FindDeath(MapLayer* layer);
	void FindVictory(MapLayer* layer);

	TileSet* GetTilesetFromTileId(int given_id) const;

public:

	MapData data;
	iPoint spawn_point;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__