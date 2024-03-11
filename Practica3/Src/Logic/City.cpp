#include "City.h"
#include "Animator.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "RenderThread.h"
#include <iostream>
#include "Missile.h"
// Tamaño original 256x256

City::City(Vector2 pos, int width, int height, RenderThread* renderThread, Image* c)
	:_pos(pos), _width(width), _height(height), _destroyed(false)
{
	//Dimensiones Sprite 
	const int numframes_ = 8;
	float originalSpriteWidth = c->getWidth() / numframes_;
	float originalSpriteHeight = c->getHeight();
	//Resolucion Original
	float spriteResolution = originalSpriteWidth / originalSpriteHeight;


	//Con el factor de escala de tamaño podemos calcular el factor de escala de forma vertical y horizontal
	float hScaleFactor = _width / originalSpriteWidth;
	float vScaleFactor = _height / originalSpriteHeight;

	// Estos numeros estan fijados puesto que son datos dados en el enunciado
	// Collider: InicioX 94 pixeles / InicioY 91 pixeles / AnchoInicial 85 / AltoInicial 97 
	_posCollider = { _pos.getX() + 94 * hScaleFactor,_pos.getY() + 91 * vScaleFactor };
	_colliderWidth = 85 * hScaleFactor;
	_colliderHeight= 97 * vScaleFactor;

	_city = c;
	if (_city == NULL)
	{
		std::cerr << "No se han podido cargar las imagenes" << std::endl;
	}
	_animator = new Animator(_city, 1, 8, 0.25f);

	this->_renderThread = renderThread;
}

City::~City()
{
	delete _animator;
	_animator = nullptr;
}

void City::render()
{
	Command renderCommand{};

#ifdef _DEBUG // Collider
	renderCommand.type = DRAW_RECT;
	RectParam rectParam = { _posCollider, (int)_colliderWidth, (int)_colliderHeight, 0xFFFF0000,false };
	renderCommand.rectParam = rectParam;
	this->_renderThread->enqueCommand(renderCommand);
#endif // _DEBUG

	renderCommand.type = DRAW_SPRITE;
	SpriteParam spriteParam = { _animator->getCurrentSprite(), _pos, _width, _height };
	renderCommand.spriteParam = spriteParam;
	this->_renderThread->enqueCommand(renderCommand);
}

void City::update(float deltaTime)
{
	_animator->update(deltaTime);
}

bool City::destroy(Missile* missile)
{
	Vector2 otherPos = missile->getPos();
	bool collides = _posCollider.getX() < otherPos.getX() && _posCollider.getX() + _colliderWidth > otherPos.getX() && _posCollider.getY() < otherPos.getY() && _posCollider.getY() + _colliderHeight> otherPos.getY();
	if (!_destroyed && collides) {
		_animator->play();
		_destroyed = true;
		missile->restartJourney();
		return true;
	}
	return false;
}

void City::restart()
{
	_destroyed = false;
	_animator->reset();
}

int City::getX() const
{
	return _pos.getX();
}

int City::getY() const
{
	return _pos.getY();
}

int City::getWidth() const
{
	return _width;
}

int City::getHeight() const
{
	return _height;
}

bool City::isDestroyed()
{
	return _destroyed;
}
