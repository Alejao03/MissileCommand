#include "GameController.h"
#include <SDL.h>

void GameController::resetTick()
{
	iea_ = {
		0,0, //Joystick
		false,false, //Botones
		0.0f,0.0f, //Gatillos
		{ 0, 0, 0 } //Angulo
	};
}

void GameController::updateInputState()
{
	iea_.motionX = leftAxisX;
	iea_.motionY = leftAxisY;
	iea_.x = buttonXPressed;
	iea_.o = buttonOPressed;
	iea_.l2 = triggerLeft;
	iea_.r2 = triggerRight;
	iea_.angle.x = angleX;
	iea_.angle.y = angleY;

}

InputState GameController::GetInputState()
{
    return iea_;
}

GameController::GameController(SDL_GameController* controller, SDL_JoystickID id): buttonXPressed(false), buttonOPressed(false),
leftAxisX(0.0f), leftAxisY(0.0f), triggerLeft(0.0f), triggerRight(0.0f),angleX(0.0f),angleY(0.0f)
{
	_controller = controller;
	_controllerID=id;
	resetTick();
	SDL_GameControllerSetSensorEnabled(_controller, SDL_SENSOR_GYRO, SDL_TRUE);
}

void GameController::removeController()
{
	SDL_GameControllerClose(_controller);
}

void GameController::processGameControllerButtonInput(const SDL_Event& event, const bool pressed)
{
	if (event.cdevice.which != _controllerID)
		return;
	auto button = event.cbutton.button;

	if (button == SDL_CONTROLLER_BUTTON_A)
		buttonXPressed = pressed;

	if (button == SDL_CONTROLLER_BUTTON_B)
		buttonOPressed = pressed;
}

void GameController::processGameControllerAxisMotion(const SDL_Event& event)
{
	if (event.cdevice.which != _controllerID)
		return;
	auto value = std::abs(event.caxis.value) > umbral ? (float)event.caxis.value / halfMax16 :0 ;

	switch (event.caxis.axis)
	{
	case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
		triggerLeft = value;
		break;
	case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
		triggerRight = value;
		break;
	case SDL_CONTROLLER_AXIS_LEFTX:
		leftAxisX = value;
		break;
	case SDL_CONTROLLER_AXIS_LEFTY:
		leftAxisY = value;
	}
}
#include <iostream>
void GameController::processGyro(const SDL_Event& event)
{
	if (event.cdevice.which != _controllerID) return;
	if (event.csensor.sensor==SDL_SENSOR_GYRO) {
		angleX = -event.csensor.data[1] * 2;
		angleY = -event.csensor.data[0] * 2;
	}
}
