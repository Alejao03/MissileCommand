#include "PlayerTurret.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "RenderThread.h"
#include "PlayerMissile.h" 
#include "OurCircle.h"
#include <vector>
#include "Colors.h"
#include "Missile.h"
#include <iostream>  // Para imprimir mensajes de error
PlayerTurret::PlayerTurret(Vector2 pos, int width, int height, RenderThread* renderThread, Image* t, TurretColor color)
	: _pos(pos), _width(width), _height(height), _destroyed(false), _color(color), _canShoot(true)
{
	_turretImage = t;
	if (_turretImage == nullptr)
	{
		std::cerr << "No se han podido cargar las imágenes" << std::endl;
	}

	_renderThread = renderThread;
	_missile = new PlayerMissile({ pos.getX() + _width / 2,pos.getY() + height / 2 }, _renderThread);
	// Dimensiones Sprite
	const int numframes_ = 4;
	_originalSpriteWidth = (_turretImage->getWidth() / numframes_);
	_originalSpriteHeight = (_turretImage->getHeight());

	_posCollider = { _pos.getX(),_pos.getY() };
	_colliderWidth = width;
	_colliderHeight = height;

	for (int cell = 1; cell <= numframes_; ++cell) {
		float row = 0;
		float col = cell;
		float xImage = col * _originalSpriteWidth;
		float yImage = row * _originalSpriteHeight;

		Sprite* frame = new Sprite(_turretImage, { xImage - _originalSpriteWidth, yImage }, _originalSpriteWidth, _originalSpriteHeight);
		_turretSprites.push_back(frame);
	}
	AdjustUser(_color);
	_destroyed = false;
	_stopRender = false;
	_isAlive = true;
	_explosion = nullptr;
}

PlayerTurret::~PlayerTurret()
{
	delete _missile;
	delete _explosion;
	// Eliminar los sprites almacenados en el vector
	for (Sprite* sprite : _turretSprites) {
		delete sprite;
	}
	_turretSprites.clear();
}

void PlayerTurret::render()
{
	Command renderCommand{};

#ifdef _DEBUG // Collider
	renderCommand.type = DRAW_RECT;
	RectParam rectParam = { _posCollider, (int)_colliderWidth, (int)_colliderHeight, 0xFFFF0000,false };
	renderCommand.rectParam = rectParam;
	this->_renderThread->enqueCommand(renderCommand);
#endif // _DEBUG
	_missile->render(); //Se renderiza primero el misil, para que no se renderice por encima de la torreta

	if (!_stopRender) {
		renderCommand.type = DRAW_SPRITE;
		// Utilizar la posición ajustada del frame en función del color
		SpriteParam spriteParam = { _actualSprite, _pos, _width, _height };
		renderCommand.spriteParam = spriteParam;
		this->_renderThread->enqueCommand(renderCommand);
	}
	if (_explosion != nullptr)
		_explosion->render();

}

void PlayerTurret::AdjustUser(TurretColor color) //Este metodo estara en un update para veri
{

	_color = color;
	_spriteFrame = static_cast<int>(color);
	_colorUser = Color::getColorArray()[_spriteFrame];
	_actualSprite = _turretSprites[_spriteFrame]; //Ajustamos el sprite de la torreta en funcion del color
}

bool PlayerTurret::destroy(Missile* missile)
{
	Vector2 otherPos = missile->getPos();
	bool collides = _posCollider.getX() < otherPos.getX() && _posCollider.getX() + _colliderWidth > otherPos.getX() &&
		_posCollider.getY() < otherPos.getY() && _posCollider.getY() + _colliderHeight > otherPos.getY();
	if (!_destroyed && collides) {

		//Aqui dibujariamos el circulo que representaria la explosion de las torretas del usuario
		_destroyed = true;
		_isAlive = false;
		missile->restartJourney();
		_explosion = new OurCircle(_renderThread, { _pos.getX() + _width / 2,_pos.getY() + _height / 2 }, 0, _width * 2 / 3, Color::White(), 0.5f);
		return true;
	}
	return false;
}

void PlayerTurret::restart()
{
	_destroyed = false;
	_stopRender = false;
	_canShoot = true;
	_isAlive = true;
	_missile->restart();
}

int PlayerTurret::getX() const
{
	return _pos.getX();
}

int PlayerTurret::getY() const
{
	return _pos.getY();
}

int PlayerTurret::getWidth() const
{
	return _width;
}

int PlayerTurret::getHeight() const
{
	return _height;
}

uint32_t PlayerTurret::getColor() const
{
	return (uint32_t)_color;
}

void PlayerTurret::update(float deltaTime)
{
	_missile->update(deltaTime);

	if (!_missile->getAnimationShoot()) _canShoot = true; //Si ha terminado la animacion de misil y explosion, puede disparar de nuevo

	if (_explosion != nullptr) { //Muerte de la torreta
		if (!_explosion->update(deltaTime))
		{
			delete _explosion;
			_explosion = nullptr;
		}
		else {
			if (!_stopRender && _explosion->getR() * 2 > _width)//Cuando el diametro es mas grande que el ancho de la torreta entonces deja de renderizarse
				_stopRender = true;
		}
	}
}

void PlayerTurret::setMissiles(std::vector<Missile*> missile)
{
	_missile->setMissiles(missile);
}

void PlayerTurret::hasShoot(Vector2 pos)
{
	_canShoot = false;
	_missile->setColor(_colorUser);
	_missile->setDestination(pos);
}

bool PlayerTurret::getCanShoot()
{
	return _canShoot;
}

bool PlayerTurret::getAlive()
{
	return _isAlive;
}

bool PlayerTurret::isDestroyed()
{
	return _destroyed;
}