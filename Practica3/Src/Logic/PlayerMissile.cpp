#include "PlayerMissile.h"
#include "RenderThread.h"
#include "Renderer.h"
#include "Utils.h"
#include "Missile.h"
#include "Colors.h"

PlayerMissile::PlayerMissile(Vector2 origin, RenderThread* renderThread, float maxTime)
{
	_renderThread = renderThread;
	_startPos = origin;
	_pos = origin;
	_maxTime = maxTime;
	_exploting = false;
	_explosion = nullptr;
	_animShoot = false;
	_timer = 0;
	_speed = 0.0f;
	_explosionTime = 2.0f / 6.0f;
	_diametro = SCREEN_WIDTH / 10;
}

PlayerMissile::~PlayerMissile()
{
	delete _explosion;
	while (!_secondaryExplosions.empty())
	{
	
		Explosion* aux = _secondaryExplosions.back();
		delete aux;
		aux = nullptr;
		_secondaryExplosions.pop_back();
	}
}

bool PlayerMissile::update(float deltaTime)
{
	if (_speed != 0.0f) {//Si la velocidad es igual a 0 aunque ejerciesemos estas dos lineas no se mover pero de esta manera mejoramos el coste
		if(!_animShoot) _animShoot = true;
		_pos.setX(_pos.getX() + _direction.getX() * deltaTime * _speed);
		_pos.setY(_pos.getY() + _direction.getY() * deltaTime * _speed);
		if (_pos.getY() <= _destination.getY()) {
			_exploting = true;
			_speed = 0.0f;			
			
			_explosion = new Explosion(_renderThread, _destination, this, _missiles, 0, (int)(_diametro / 2), _color, _explosionTime);
		}
	}

	if (_exploting) {
		if (!_explosion->update(deltaTime)) {
			_exploting = false;
			_animShoot = false;
			delete _explosion;
			_explosion = nullptr;
			_speed = 0.0f;
			_pos = _startPos; //Devolvemos a la posicion inicial
		}

	}

	if (!_secondaryExplosions.empty())
	{
		auto i = _secondaryExplosions.begin();
		while(i!=_secondaryExplosions.end())
		{
			Explosion* e = *i;
			if (!e->update(deltaTime)) {
				i=_secondaryExplosions.erase(i);
				delete e;
				e = nullptr;
			}
			else {
				i++;
			}
				
		}
	}

	_timer += deltaTime;

	return true;
}

void PlayerMissile::render()
{
	if (_speed != 0.0f) {
		Command renderCommand{};
		//Renderizamos el misil
		renderCommand.type = DRAW_LINE;
		LineParam line = { _startPos, _pos, _color };
		renderCommand.lineParam = line;
		this->_renderThread->enqueCommand(renderCommand);
	}
	if (_explosion != nullptr) {
		_explosion->render();
	}
	if (!_secondaryExplosions.empty())
	{
		for (Explosion* e : _secondaryExplosions)
		{
			e->render();
		}
	}
}

void PlayerMissile::restart()
{
	_speed = 0.0f; //Con esto no funciona misil
	_exploting = false; //Con esto no funciona explosion
	if (_explosion != nullptr) {
		delete _explosion;
		_explosion = nullptr;
	}
	while(!_secondaryExplosions.empty())
	{
		Explosion* aux = _secondaryExplosions.back();
		delete aux;
		aux = nullptr;
		_secondaryExplosions.pop_back();
		

	}
}

int PlayerMissile::getX() const
{
	return _pos.getX();
}

int PlayerMissile::getY() const
{
	return _pos.getY();
}

void PlayerMissile::setDestination(Vector2 dst)
{
	//Ha disparado hacia abajo
	if (dst.getY() > _startPos.getY())return;
	_destination = dst;
	_direction.setX(_destination.getX() - _startPos.getX());
	_direction.setY(_destination.getY() - _startPos.getY());

	_speed = SCREEN_HEIGHT / _maxTime; 

	//Normalizar direccion
	double length = sqrt(_direction.getX() * _direction.getX() + _direction.getY() * _direction.getY());
	_direction.setX(_direction.getX() / length);
	_direction.setY(_direction.getY() / length);

}

void PlayerMissile::setColor(uint32_t color)
{
	_color = color;
}

void PlayerMissile::setMissiles(std::vector<Missile*> missile)
{
	_missiles = missile;
}

bool PlayerMissile::getAnimationShoot()
{
	return _animShoot;
}

void PlayerMissile::createExplosion(Missile* m)
{
	if (m->isAlive()) {

	Explosion* e = new Explosion(_renderThread, m->getPos(), this, _missiles, 0, (int)(_diametro / 4), _color, _explosionTime);
	_secondaryExplosions.push_back(e);
	}
	
}