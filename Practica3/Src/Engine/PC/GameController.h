#pragma once
#include "InputState.h"
#include "UserServicePC.h"

class GameController
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
	/// Lectura de botones del mando
	/// </summary>
	/// <param name="event"></param>
	/// <param name="pressed"></param>
	void processGameControllerButtonInput(const SDL_Event& event, const bool pressed);
	/// <summary>
	/// Lectura de joystick y gatillos del mando
	/// </summary>
	/// <param name="event"></param>
	void processGameControllerAxisMotion(const SDL_Event& event);
	/// <summary>
	/// Lectura de giroscopio del mando
	/// </summary>
	/// <param name="event"></param>
	void processGyro(const SDL_Event& event);
	/// <summary>
	/// Devuelve el InputState del mando
	/// </summary>
	/// <returns>InputState</returns>
	InputState GetInputState(); 
	/// <summary>
	/// Constructora del GameController
	/// </summary>
	GameController(SDL_GameController* controller, SDL_JoystickID controllerID);
	/// <summary>
	/// Cerramos el mando con SDL_GameControllerClose
	/// </summary>
	void removeController();
private:
	InputState iea_;
	SDL_GameController* _controller;
	SDL_JoystickID _controllerID;
	bool buttonXPressed, buttonOPressed;
	float leftAxisX, leftAxisY, triggerLeft, triggerRight, angleX, angleY;

	const uint16_t umbral = 10000;
	const uint16_t halfMax16 = UINT16_MAX / 2;
};