#include "Explosion.h"
#include "Renderer.h"
#include "Platform.h"
#include <math.h>
#include <iostream>
#include "RenderThread.h"
#include "Missile.h"
#include "PlayerMissile.h"

Explosion::Explosion(RenderThread* rThread, Vector2 pos, PlayerMissile* pM, std::vector<Missile*> missile, float r, int rM, uint32_t color, float time) : _pos(pos),
_r(r), _color(color), _renderThread(rThread), _missiles(missile), _pM(pM)
{
	_rMax = rM;
	_speed = _rMax / time;
}


bool Explosion::update(float deltaTime)
{
	//En caso de crecer, se le suma al radio (velocidad * deltaTime)
	if (_grow)
		_r = _r + _speed * deltaTime;
	else //En caso contrario, se le resta
		_r = _r - _speed * deltaTime;

	//Si el radio ya ha llegado a su máximo, cambiamos el booleano _grow para que decrezca
	if (_r >= _rMax)
		_grow = false;

	for (Missile* m : _missiles) {
		if (isInside(m->getPos())) {
			_pM->createExplosion(m);
			m->destroy();
		}
	}

	// Handle State
	if (!isAlive())
		return false;

	return true;
}

//Método para comprobar si el Circulo está vivo
bool Explosion::isAlive()
{
	return _r > 0;
}

bool Explosion::isInside(Vector2 other)
{
	// Calcular la distancia al cuadrado entre los dos puntos
	int distanciaCuadrada = (other.getX() - _pos.getX()) * (other.getX() - _pos.getX()) +
		(other.getY() - _pos.getY()) * (other.getY() - _pos.getY());

	// Verificar si la distancia al cuadrado es menor o igual al radio al cuadrado
	return distanciaCuadrada <= (_r * _r);
}

float Explosion::getR() const
{
	return _r;
}

void Explosion::render()
{
	Command renderCommand{};
	//Renderizamos el misil
	renderCommand.type = DRAW_CIRCLE;
	CircleParam circle = { _pos,(int)_r, _color };
	renderCommand.circleParam = circle;
	this->_renderThread->enqueCommand(renderCommand);
}