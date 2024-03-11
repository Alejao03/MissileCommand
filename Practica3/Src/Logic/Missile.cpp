#include "Missile.h"
#include "Renderer.h"
#include "Platform.h"
#include "RenderThread.h"
#include "Utils.h"
#include "City.h"
#include "PlayerTurret.h"
#include <math.h>
#include <iostream>

//Hay que vaciar las llamadas a missil antes de la constructora para que se construya como con restart journey 
Missile::Missile(Vector2 pos, Vector2 end , int dieHeight, std::vector<City*> _cities,std::vector<PlayerTurret*> turrets, RenderThread* renderThread, uint32_t color, int maxTime) : _pos(pos), _dieHeight(dieHeight), _color(color)
{
	_citiesList = _cities;
	_turrets = turrets;
	_startPos=pos;
	
	_direction.setX( end.getX() - _startPos.getX());
	_direction.setY(end.getY() - _startPos.getY());


	_speed = _direction.getY() / maxTime;

	//Normalizar direccion
	double length = sqrt(_direction.getX() * _direction.getX() + _direction.getY() * _direction.getY());
	_direction.setX(_direction.getX() / length);
	_direction.setY(_direction.getY() / length);

	_timer = 0.0f;
	_destroyed = false;
	this->_renderThread = renderThread;
}

bool Missile::update(float deltaTime)
{
	//Si no esta vivo se reposiciona
	if (!isAlive()) restartJourney();
	_pos.setX( _pos.getX() + _direction.getX() * deltaTime * _speed);
	_pos.setY( _pos.getY() + _direction.getY() * deltaTime * _speed);
	_timer += deltaTime;
	// Actualizar todas las ciudades en la lista
	for (City* city : _citiesList) {
		city->destroy(this);
	}
	for (PlayerTurret* turret : _turrets) {
		turret->destroy(this);
	}

	return true;
}

//Método para comprobar si el Misil está vivo
bool Missile::isAlive() 
{
	return!_destroyed&& _pos.getY() < _dieHeight;
}

void Missile::render()
{
	Command renderCommand{};
	//Renderizamos el misil
	renderCommand.type = DRAW_LINE;
	LineParam line = { _startPos,_pos, _color };
	renderCommand.lineParam = line;
	this->_renderThread->enqueCommand(renderCommand);
}

void Missile::restartJourney()
{
	_startPos.setX( Utils::randomInt(0, Renderer::Instance()->getWidth()));
	_direction.setX(Utils::randomInt(0, Renderer::Instance()->getWidth()) - _startPos.getX());
	_direction.setY( Renderer::Instance()->getHeight() - _startPos.getY());

	//Normalizar direccion
	double length = sqrt(_direction.getX() * _direction.getX() + _direction.getY() * _direction.getY());
	_direction.setX(_direction.getX() / length);
	_direction.setY(_direction.getY() / length);
	_destroyed = false;
	_pos = _startPos;
	_timer = 0.0f;
}

int Missile::getX() const
{
	return _pos.getX();
}

int Missile::getY() const
{
	return _pos.getY();
}

Vector2 Missile::getPos() const
{
	return _pos;
}

void Missile::destroy()
{
	_destroyed = true;
}
