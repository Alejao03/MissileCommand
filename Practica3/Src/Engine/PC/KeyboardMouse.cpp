#include "KeyboardMouse.h"
#include <SDL.h>

# define MOUSE_MOTION_ADJUSTMENT 8

void KeyboardMouse::proccessKey(const SDL_Keycode& key, const bool pressed)
{
	switch (key) {
	case SDLK_LEFT:
		leftArrow = pressed;
		break;
	case SDLK_RIGHT:
		rightArrow = pressed;
		break;
	case SDLK_UP:
		downArrow = pressed;
		break;
	case SDLK_DOWN:
		upArrow = pressed;
		break;
		// Cualquiera de estos botones cuenta como boton de disparo
	case SDLK_RSHIFT:
		rightShiftPressed = pressed;
	case SDLK_RCTRL:
		rightControlPressed = pressed;
		break;
	default:
		break;
	}
}

void KeyboardMouse::proccessMouseButton(Uint8 button, bool pressed)
{
	if (button == SDL_BUTTON_LEFT)
		mouseLeftButtonPressed = pressed;
	else if (button == SDL_BUTTON_RIGHT)
		mouseRightButtonPressed = pressed;
}

void KeyboardMouse::proccessMouseMotion(const SDL_MouseMotionEvent& mouseMotion)
{
	mouseMotionX = (float)mouseMotion.xrel / MOUSE_MOTION_ADJUSTMENT;
	mouseMotionY = (float)mouseMotion.yrel / MOUSE_MOTION_ADJUSTMENT;
}

void KeyboardMouse::resetTick()
{
	iea_ = {
		0,0, //Joystick
		false,false, //Botones
		0.0f,0.0f, //Gatillos
		{ 0, 0, 0 } //Angulo
	};

	mouseMotionX = 0;
	mouseMotionY = 0;
}

InputState KeyboardMouse::GetInputState()
{
	return iea_;
}

void KeyboardMouse::updateInputState()
{
	//El jugador asignado al teclado+ratón podrá mover su punto de mira desplazando el
			//ratón o usando las teclas del cursor, y podrá disparar con los botones del ratón o con
			//las teclas Mayúscula o Control del lado derecho del teclado
	iea_.angle.x = mouseMotionX;
	iea_.angle.y = mouseMotionY;

	if (leftArrow)
		iea_.motionX += -1;

	if (rightArrow)
		iea_.motionX += 1;

	if (upArrow)
		iea_.motionY += 1;

	if (downArrow)
		iea_.motionY += -1;


	iea_.x = rightShiftPressed || mouseLeftButtonPressed;
	iea_.o = rightControlPressed || mouseRightButtonPressed;
	//NO TIENE TRIGGERS A SI QUE LO PONEMOS A 0
	iea_.l2 = 0;
	iea_.r2 = 0;
}
