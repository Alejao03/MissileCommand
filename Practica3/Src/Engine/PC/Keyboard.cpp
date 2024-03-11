#include "Keyboard.h"
#include <SDL.h>

void Keyboard::proccessEvent(const SDL_Keycode& key, const bool pressed)
{
	switch (key)
	{
	case SDLK_a:
		aKey = pressed;
		break;
	case SDLK_d:
		dKey = pressed;
		break;
	case SDLK_w:
		wKey = pressed;
		break;
	case SDLK_s:
		sKey = pressed;
		break;
		// Cualquiera de estos botones cuenta como boton de disparo
	case SDLK_q:
		qKey = pressed;
	case SDLK_e:
		eKey = pressed;
		break;
	default:
		break;
	}
}

void Keyboard::resetTick()
{
	iea_ = {
		0,0, //Joystick
		false,false, //Botones
		0.0f,0.0f, //Gatillos
		{ 0, 0, 0 } //Angulo
	};
}

InputState Keyboard::GetInputState()
{
    return iea_;
}

void Keyboard::updateInputState()
{
	// Lógica específica para teclado
	iea_.motionX = aKey ? -1.0f : (dKey ? 1.0f : 0.0f);
	iea_.motionY = wKey ? -1.0f : (sKey ? 1.0f : 0.0f);

	iea_.x = qKey;
	iea_.o = eKey;
	//NO TIENE TRIGGERS A SI QUE LO PONEMOS A 0
	iea_.l2 = 0;
	iea_.r2 = 0;
}
