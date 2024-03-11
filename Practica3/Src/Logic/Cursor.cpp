#include "Cursor.h"
#include "Renderer.h"
#include "Platform.h"
#include "Sprite.h"
#include "RenderThread.h"
#include "PlayerTurret.h"
#include <math.h>
#include <list>
#include <iostream>
#include <InputState.h>
#include "Input.h"
Cursor::Cursor(Vector2 position, Sprite* cursor_, UserID userId, RenderThread* renderThread, std::vector<PlayerTurret*> turrets, int width, int height, float vel_, float giroscopeMultiplier_) : _position(position), _direction(Vector2(0, 0)), _turrets(turrets), _userId(userId),
_joystickMultiplier(vel_), _giroscopeMultiplier(giroscopeMultiplier_), _sprite(cursor_), _size(Vector2(width, height)),
_leftTriggerPressed(false), _rightTriggerPressed(false), _width(width), height(height), _shootedWithButton(false), _shootedWithTrigger(false)
{
	_renderThread = renderThread;
}

Cursor::~Cursor()
{
}

void Cursor::update(float deltaTime)
{
	InputState e = Input::Instance()->getInput(_userId);

	updatePosition(deltaTime, e.motionX, e.motionY, e.angle.x, e.angle.y);
	handleTriggers(e.l2, e.r2);
	handleShoot(e.x || e.o);
}

void Cursor::render()
{
	Command renderCommand{};

	renderCommand.type = DRAW_SPRITE;
	SpriteParam spriteParam = { _sprite, _position, _width, height };
	renderCommand.spriteParam = spriteParam;
	_renderThread->enqueCommand(renderCommand);
}

const Sprite* Cursor::getSprite() const { return _sprite; }
Vector2 Cursor::getPos() { return _position; }


const int Cursor::getUserId() const
{
	return _userId;
}

void Cursor::updatePosition(float deltaTime, const float& motionX, const float& motionY, const float& accelerationX, const float& accelerationY)
{
	//deadzone giroscopio
	float accX = accelerationX > 0.01 || accelerationX < -0.01 ? accelerationX : 0;
	float accY = accelerationY > 0.01 || accelerationY < -0.01 ? accelerationY:0;

	_position += (Vector2(motionX, motionY) * _joystickMultiplier + Vector2(accX, accY) * _giroscopeMultiplier) * deltaTime;
	int widthScreen = Renderer::Instance()->getWidth();
	int heightScreen = Renderer::Instance()->getHeight();
	

	// Clamp positions
	_position.setX(std::min(std::max((int)_position.getX(), 0), widthScreen - _width));
	_position.setY(std::min(std::max((int)_position.getY(), 0), heightScreen - height));
}

void Cursor::handleShoot(bool button)
{
	bool trigger = _rightTriggerPressed || _leftTriggerPressed;

	if (button && !_shootedWithButton)
	{
		shoot();
		_shootedWithButton = true;
	}
	else if (!button)
		_shootedWithButton = false;

	if (trigger && !_shootedWithTrigger)
	{
		shoot();
		_shootedWithTrigger = true;
	}
	else if (!trigger)
		_shootedWithTrigger = false;
}

void Cursor::handleTriggers(float triggerLeft, float triggerRight)
{
	if (triggerLeft > 0.7f)
		_leftTriggerPressed = true;
	else if (triggerLeft < 0.3f)
		_leftTriggerPressed = false;

	if (triggerRight > 0.7f)
		_rightTriggerPressed = true;
	else if (triggerRight < 0.3f)
		_rightTriggerPressed = false;
}
void Cursor::shoot()
{
	
	for (PlayerTurret* t : _turrets)
	{
		if (t->getAlive() && t->getCanShoot() && UserService::Instance()->getUserColor(_userId) == t->getColor())
		{

			t->hasShoot(Vector2(_position.getX() + _width / 2, _position.getY() + height / 2));
			break;
		}
	}
}