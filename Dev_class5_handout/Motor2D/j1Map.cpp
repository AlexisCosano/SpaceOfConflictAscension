#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Collisions.h"
#include "j1Player.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Llama al render antes de que este disponible
bool j1Map::Awake(pugi::xml_node& config)
{
	 LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());
	

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

void j1Map::FindColliders(MapLayer* layer)
{
	int counter = 0;
	while (counter < layer->height * layer->width)
	{
		int id = layer->data[counter]; //devuelve el tipo de tileset

		if (id > 0)
		{
			int x = counter;
			int y = layer->width;
			Get(&x, &y);

			TileSet* tileset = data.tilesets.start->data;

			SDL_Rect r = tileset->GetTileRect(id);
			iPoint pos = MapToWorld(x, y);

			r.x = pos.x;
			r.y = pos.y;

			App->collision->AddColliders(r);
		}
		counter++;
	}
}

void j1Map::FindDeath(MapLayer* layer)
{
	int counter = 0;
	while (counter < layer->height * layer->width)
	{
		int id = layer->data[counter]; //devuelve el tipo de tileset

		if (id > 0)
		{
			int x = counter;
			int y = layer->width;
			Get(&x, &y);

			TileSet* tileset = data.tilesets.start->data;

			SDL_Rect r = tileset->GetTileRect(id);
			iPoint pos = MapToWorld(x, y);

			r.x = pos.x;
			r.y = pos.y;

			App->collision->AddDeath(r);
		}
		counter++;
	}
}

void j1Map::FindVictory(MapLayer* layer)
{
	int counter = 0;
	while (counter < layer->height * layer->width)
	{
		int id = layer->data[counter]; //devuelve el tipo de tileset

		if (id > 0)
		{
			int x = counter;
			int y = layer->width;
			Get(&x, &y);

			TileSet* tileset = data.tilesets.start->data;

			SDL_Rect r = tileset->GetTileRect(id);
			iPoint pos = MapToWorld(x, y);

			r.x = pos.x;
			r.y = pos.y;

			App->collision->AddVictory(r);
		}
		counter++;
	}
}

void j1Map::FindSpawn(MapLayer* layer)
{
	int counter = 0;
	while (counter < layer->height * layer->width)
	{
		int id = layer->data[counter]; //devuelve el tipo de tileset

		if (id > 0)
		{
			int x = counter;
			int y = layer->width;
			Get(&x, &y);

			TileSet* tileset = data.tilesets.start->data;

			iPoint pos = MapToWorld(x, y);

			spawn_point = pos;
		}
		counter++;
	}
}

void j1Map::Draw()
{
	if (map_loaded == false)
		return;


	//Blit background
    //App->render->Blit(data.background_image, data.background_offset.x - App->player->player_x_displacement * data.parallax_speed, data.background_offset.y);


	//Blit bone
	 //App->render->Blit(App->player->graphics, data.bone_position.x, data.bone_position.y, 1 , &bone_rect);


	 for (int iterator = 0; iterator < data.layer_array.count(); iterator++)
	 {
		 int counter = 0;
		 while (counter < data.layer_array.At(iterator)->data->height * data.layer_array.At(iterator)->data->width)
		 {
			 int id = data.layer_array.At(iterator)->data->data[counter]; //devuelve el tipo de tileset

			 if (id != 0)
			 {
				 int x = counter;
				 int y = data.layer_array.At(iterator)->data->width;
				 Get(&x, &y);

				 //X e Y son coordenadas del tileset

				 convert_to_real_world(&x, &y);

				 //Aqui en pixels
				 if (data.layer_array.At(iterator)->data->properties.is_drawn == true)
				 {
					 for (int i = 0; i < data.tilesets.count(); i++)
					 {
						 App->render->Blit(data.tilesets.At(i)->data->texture, x, y, 1, &TileRect(id, i));
					 }
				 }
			 }
			 counter++;
		 }
	 }
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * data.tile_width;
	ret.y = y * data.tile_height;

	return ret;
}

SDL_Rect j1Map::TileRect(int tileid, int iterator)
{

	tileid = tileid - data.tilesets.At(iterator)->data->firstgid;

	int row = tileid / data.tilesets.At(iterator)->data->num_tiles_width;

	int column = tileid - row*data.tilesets.At(iterator)->data->num_tiles_width;

	SDL_Rect rect;
	rect.w = data.tilesets.At(iterator)->data->tile_width;
	rect.h = data.tilesets.At(iterator)->data->tile_height;

	rect.x = data.tilesets.At(iterator)->data->margin + (rect.w + data.tilesets.At(iterator)->data->spacing)*column;

	rect.y = data.tilesets.At(iterator)->data->margin + (rect.h + data.tilesets.At(iterator)->data->spacing)*row;

	return rect;
}

TileSet* j1Map::GetTilesetFromTileId(int given_id) const
{
	TileSet* result = new TileSet;

	for (int i = 0; i < data.tilesets.count(); i++)
	{
		result = data.tilesets.At(i)->data;


		if (given_id == data.tilesets.At(i)->data->firstgid)
		{
			result = data.tilesets.At(i)->data;
		}
	}

	return result;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Quita los tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();
	data.layer_array.clear();
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;

	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}
	LoadBackground(map_file);
	LoadMapProperties(map_file);

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;

	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}
	// Load layer info ----------------------------------------------
	LoadLayer(map_file);

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}
		
		p2List_item<MapLayer*>* item_layer = data.layer_array.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("width: %d height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}



    //Posicion inicial del jugador
	App->player->position.x = data.player_starting_value.x;
	App->player->position.y = data.player_starting_value.y;

	

	map_loaded = ret;

	return ret;
}

// Carga las propiedades generales del mapa
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node)
{
	pugi::xml_node layer = node.child("map").child("layer");
	while (layer != nullptr)
	{
	    MapLayer* layer_data = new MapLayer;

		layer_data->height = layer.attribute("height").as_int();
		layer_data->width = layer.attribute("width").as_int();
		layer_data->name = layer.attribute("name").as_string();
		LoadLayerProperties(layer, layer_data->properties);

		layer_data->data = new uint[layer_data->height*layer_data->width];

		memset(layer_data->data, 0, sizeof(uint)*(layer_data->height*layer_data->width));

		int i = 0;
		pugi::xml_node tile_ = layer.child("data").child("tile");
		while (tile_ != nullptr)
		{
			layer_data->data[i] = tile_.attribute("gid").as_uint();
			tile_ = tile_.next_sibling("tile");
			i++;
		}

		if (layer_data->name == "SpawnPoint")
		{
			FindSpawn(layer_data);
			LOG("Spawn found.");
		}

		if (layer_data->name == "NoWalkable")
		{
			FindColliders(layer_data);
		}

		if (layer_data->name == "Death")
		{
			FindDeath(layer_data);
		}

		if (layer_data->name == "Win")
		{
			FindVictory(layer_data);
		}

		data.layer_array.add(layer_data);
		layer = layer.next_sibling("layer");
	}

	return true;
}

bool j1Map::LoadBackground(pugi::xml_node& node)
{
	data.background_image = App->tex->Load(PATH(folder.GetString(),node.child("map").child("imagelayer").child("image").attribute("source").as_string()));

	data.background_offset.x = node.child("map").child("imagelayer").attribute("offsetx").as_float();

	data.background_offset.y = node.child("map").child("imagelayer").attribute("offsety").as_float();

	return true;
}
bool j1Map::LoadMapProperties(pugi::xml_node& node)
{
	pugi::xml_node iterator = node.child("map").child("properties").child("property");
	while (iterator != nullptr)
	{ 
		p2SString name = iterator.attribute("name").as_string();

		if(name == "parallax_speed")
		{ 
			data.parallax_speed = iterator.attribute("value").as_float();
		}

		if (name == "player_starting_valuex")
		{
			data.player_starting_value.x = iterator.attribute("value").as_float();
		}

		if (name == "player_starting_valuey")
		{
			data.player_starting_value.y = iterator.attribute("value").as_float();
		}

		if( name == "camera_y_limit")
		{ 
		    data.camera_y_limit = iterator.attribute("value").as_float();
		}

		if (name == "bone_positionx")
		{
			data.bone_position.x = iterator.attribute("value").as_float();
		}

		if (name == "bone_positiony")
		{
			data.bone_position.y = iterator.attribute("value").as_int();
		}

		iterator = iterator.next_sibling();
	}

	return true;
}

void j1Map::Get(int* x, int* y)
{
	int row = *x / *y;
	int column = *x - row*(*y);

	*x = column;
	*y = row;
}

void j1Map::convert_to_real_world(int* x, int* y)
{
	*x = *x * data.tilesets.At(0)->data->tile_height;

	*y = *y * data.tilesets.At(0)->data->tile_width;
}

// Load a group of properties from a node and fill a list with it
bool j1Map::LoadLayerProperties(pugi::xml_node& node, LayerProperties& properties)
{
	bool ret = false;	

	for (pugi::xml_node property_child = node.child("properties").first_child(); property_child; property_child = property_child.next_sibling())
	{
		ret = true;
		pugi::xml_attribute collider_child = property_child.attribute("name");
		p2SString string = collider_child.as_string();
		
		if (string == "Draw")
		{
			properties.is_drawn = property_child.attribute("value").as_bool();
		}
		
		else if (string == "HasColliders")
		{
			properties.has_colliders = property_child.attribute("value").as_bool();
		}

		else
		{
			LOG("Error: no properties found.");
			ret = false;
		}
	}

	LOG("Layer '%s' metadata ===================", node.attribute("name").as_string());
	if (properties.is_drawn == true)
	{
		LOG("Draw is true.");
	}
	else
	{
		LOG("Draw is false.");
	}
	
	if (properties.has_colliders == true)
	{
		LOG("HasColliders is true.");
	}
	else
	{
		LOG("HasColliders is false.");
	}		

	return ret;
}
