#pragma once
#include "InputState.h"
#include <cstdint>

union SDL_Event;

typedef int32_t SDL_Keycode;

class Keyboard
{
public:
	/// <summary>
	/// Reiniciamos el InputState
	/// </summary>
	void resetTick();
	/// <summary>
	/// Actualizamos el InputState
	/// </summary>
	void updateInputState();
	/// <summary>
	/// Lectura de botones "w", "a", "s", "d", "q", "e" del teclado
	/// </summary>
	void proccessEvent(const SDL_Keycode& key, const bool pressed);
	/// <summary>
	/// Devuelve el InputState
	/// </summary>
	/// <returns>InputState</returns>
	InputState GetInputState();
private:

	//Teclado
	bool wKey, aKey, sKey, dKey, qKey, eKey;
	InputState iea_;
};

