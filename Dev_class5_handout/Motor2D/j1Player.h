#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "Animation.h"
#include "j1Collisions.h"
#include "p2Point.h"

struct SDL_Texture;

class j1Player : public j1Module
{
public:

	j1Player();
	~j1Player();

	bool Start();
	bool PostUpdate();
	

	// Load / Save
	bool Load(pugi::xml_node&);
	 bool Save(pugi::xml_node&) const;

	void WallSlide();
	void Jump();
	void Slide();

public:

	Collider* collider;
	Collider* spike_test_collider; // Just to test deadly colliders

	SDL_Rect rect_after_sliding;
	Uint32 time = 0;
	bool allowtime = true;

	float player_height_before_sliding;
	bool jumping = false;
	bool walljumping = false;
	bool flip = false;
	bool StickToWall = false;
	bool dead = false;
	bool sliding = false;
	int jcontact = 0; 

	iPoint contact; 
	fPoint speed;
	fPoint position;
	iPoint sprite_distance;
	float gravity;
	float player_x_displacement;


public:

	SDL_Texture* graphics = nullptr;
	Animation* current_animation;
	Animation idle;
	Animation run;
	Animation jump;
	Animation fall;
	Animation death;
	Animation slide;
	Animation wallslideright;
	Animation wallslideleft;

};

#endif //__j1PLAYER_H__