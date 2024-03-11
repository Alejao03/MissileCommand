#include "OurCircle.h"
#include "Renderer.h"
#include "Platform.h"
#include <math.h>
#include <iostream>
#include "RenderThread.h"


OurCircle::OurCircle(RenderThread* rThread, Vector2 pos, float r, int rM, uint32_t color, float time) : _pos(pos),
_r(r), _color(color), _renderThread(rThread)
{
	_rMax = rM;
	_speed = _rMax / time;
}

bool OurCircle::update(float deltaTime)
{
	//En caso de crecer, se le suma al radio (velocidad * deltaTime)
	if (_grow)
		_r = _r + _speed * deltaTime;
	else //En caso contrario, se le resta
		_r = _r - _speed * deltaTime;

	//Si el radio ya ha llegado a su máximo, cambiamos el booleano _grow para que decrezca
	if (_r >= _rMax)
		_grow = false;

	// Handle State
	if (!isAlive())
		return false;

	return true;
}

//Método para comprobar si el Circulo está vivo
bool OurCircle::isAlive()
{
	return _r > 0;
}

bool OurCircle::isInside(Vector2 other)
{
	// Calcular la distancia al cuadrado entre los dos puntos
	int distanciaCuadrada = (other.getX() - _pos.getX()) * (other.getX() - _pos.getX()) +
		(other.getY() - _pos.getY()) * (other.getY() - _pos.getY());

	// Verificar si la distancia al cuadrado es menor o igual al radio al cuadrado
	return distanciaCuadrada <= (_r * _r);
}

float OurCircle::getR() const
{
	return _r;
}

void OurCircle::render()
{
	Command renderCommand{};
	//Renderizamos el misil
	renderCommand.type = DRAW_CIRCLE;
	CircleParam circle = {_pos,(int)_r, _color };
	renderCommand.circleParam = circle;
	this->_renderThread->enqueCommand(renderCommand);
}