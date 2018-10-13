#ifndef __j1COLLISIONS_H__
#define __j1COLLISIONS_H__

#include "j1Module.h"
#include "p2List.h"


struct SDL_Rect;

class j1Colliders : public j1Module
{

public:

	j1Colliders();

	// Destructor
	virtual ~j1Colliders();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	bool CheckCollision(SDL_Rect r1, SDL_Rect r2) const;
	void AddColliders(SDL_Rect collider);
	void AddDeath(SDL_Rect death);
	void AddVictory(SDL_Rect victory);

	void UnloadAllColliders();

	// Save & Load
	bool Save(pugi::xml_node&);
	bool Load(pugi::xml_node&);

public:
	p2List<SDL_Rect> colliders;
	p2List<SDL_Rect> death_triggers;
	p2List<SDL_Rect> victory_triggers;

	bool debug_draw;
};

#endif // __j1COLLISIONS_H__