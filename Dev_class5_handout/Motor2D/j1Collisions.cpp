#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collisions.h"

j1Colliders::j1Colliders()
{
}

j1Colliders::~j1Colliders()
{
}

bool j1Colliders::Awake(pugi::xml_node &)
{
	return true;
}

bool j1Colliders::Start()
{
	return true;
}

bool j1Colliders::PreUpdate()
{
	return true;
}

bool j1Colliders::CleanUp()
{
	return true;
}

bool j1Colliders::CheckCollision(SDL_Rect r1, SDL_Rect r2) const
{
	return (r1.x < r2.x + r2.w &&
		r1.x + r1.w > r2.x &&
		r1.y < r2.y + r2.h &&
		r1.h + r1.y > r2.y);
}

void j1Colliders::AddColliders(SDL_Rect collider)
{
	colliders.add(collider);
}

void j1Colliders::AddDeath(SDL_Rect death)
{
	death_triggers.add(death);
}

void j1Colliders::AddVictory(SDL_Rect victory)
{
	victory_triggers.add(victory);
}

bool j1Colliders::Save(pugi::xml_node &)
{
	return true;
}

bool j1Colliders::Load(pugi::xml_node &)
{
	return true;
}

void j1Colliders::UnloadAllColliders()
{
	colliders.clear();
	death_triggers.clear();
	victory_triggers.clear();
}