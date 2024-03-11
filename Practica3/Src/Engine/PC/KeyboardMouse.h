#pragma once
#include "InputState.h"
#include <cstdint>

union SDL_Event;

typedef int32_t SDL_Keycode;
typedef uint8_t Uint8;
struct SDL_MouseMotionEvent;

class KeyboardMouse
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
	/// Lectura de botones "arriba", "izquierda", "abajo", "derecha", "control derecho", "shift derecho" del teclado
	/// </summary>
	void proccessKey(const SDL_Keycode& key, const bool pressed);
	/// <summary>
	/// Lectura de botones del raton ("click izquierdo y derecho")
	/// </summary>
	void proccessMouseButton(Uint8 button, bool pressed);
	/// <summary>
	/// Lectura del movimiento del raton
	/// </summary>
	void proccessMouseMotion(const SDL_MouseMotionEvent& mouseMotion);
	/// <summary>
	/// Devuelve el InputState
	/// </summary>
	/// <returns>InputState</returns>
	InputState GetInputState();

private:

	//Teclado
	bool leftArrow, rightArrow, upArrow, downArrow, rightShiftPressed, rightControlPressed;

	// Raton
	bool mouseLeftButtonPressed, mouseRightButtonPressed;
	float mouseMotionX, mouseMotionY;
	InputState iea_;
};

