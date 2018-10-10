#ifndef __j1INPUT_H__
#define __j1INPUT_H__

#include "j1Module.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
//#define LAST_KEYS_PRESSED_BUFFER 50

struct SDL_Rect;

enum j1EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum j1KeyState
{
	 KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class j1Input : public j1Module
{

public:

	j1Input();

	// Destructor
	virtual ~j1Input();

	//Llama al render antes de que este dispoinible
	bool Awake(pugi::xml_node&);

	//Llama antes de cargar primer frame
	bool Start();

	// LLama a cada loop iteracion
	bool PreUpdate();

	// Llama antes de salir
	bool CleanUp();

	// Reune los eventos win
	bool GetWindowEvent(j1EventWindow ev);

	// Chequea los key states(mouse y joy incluidos)
	j1KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	j1KeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	// Chequea si cierto evento de window a sucedido
	bool GetWindowEvent(int code);

	// Mouse/axis posicion
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

private:
	bool		windowEvents[WE_COUNT];
	j1KeyState*	keyboard;
	j1KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	int			mouse_motion_x;
	int			mouse_motion_y;
	int			mouse_x;
	int			mouse_y;
};

#endif // __j1INPUT_H__