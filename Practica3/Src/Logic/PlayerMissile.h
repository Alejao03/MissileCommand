#pragma once
#include <cstdint>
#include <vector>
#include <list>
#include "Vector2.h"
#include "Explosion.h"
struct Color;
class RenderThread;
class OurCircle;

#define SCREEN_WIDTH Renderer::Instance()->getWidth()
#define SCREEN_HEIGHT Renderer::Instance()->getHeight()
class PlayerMissile
{
public:
	PlayerMissile(Vector2 origin, RenderThread* renderThread, float maxTime=3.0f);
	~PlayerMissile();
	/// <summary>
	/// Actualiza el estado del misil.
	/// </summary>
	/// <param name="deltaTime">Tiempo transcurrido desde la �ltima actualizaci�n.</param>
	/// <returns>Devuelve true si la actualizaci�n fue exitosa, false en caso contrario.</returns>
	bool update(float deltaTime);
	/// <summary>
	/// Renderiza el misil y sus explosiones secundarias.
	/// </summary>
	void render();
	/// <summary>
	/// Reinicia el estado del misil.
	/// </summary>
	void restart();
	/// <summary>
	/// Obtiene la posici�n X actual del misil.
	/// </summary>
	/// <returns>Posici�n X del misil.</returns>
	int getX()const;
	/// <summary>
	/// Obtiene la posici�n Y actual del misil.
	/// </summary>
	/// <returns>Posici�n Y del misil.</returns>
	int getY()const;
	/// <summary>
	/// Establece la posici�n de destino del misil y calcula su direcci�n y velocidad.
	/// </summary>
	/// <param name="dst">Posici�n de destino del misil.</param>
	void setDestination(Vector2 dst);
	/// <summary>
	/// Establece el color del misil.
	/// </summary>
	/// <param name="color">Color del misil.</param>
	void setColor(uint32_t color);
	/// <summary>
	/// Establece la lista de misiles para comprobar colisiones.
	/// </summary>
	/// <param name="missile">Vector de punteros a misiles.</param>
	void setMissiles(std::vector<Missile*>missile);
	/// <summary>
	/// Obtiene el estado de la animaci�n de disparo del misil.
	/// </summary>
	/// <returns>True si la animaci�n de disparo est� activa, false en caso contrario.</returns>
	bool getAnimationShoot();
	/// <summary>
	/// Crea una explosi�n secundaria en la posici�n de otro misil.
	/// </summary>
	/// <param name="m">Puntero al misil que provoca la explosi�n secundaria.</param>
	void createExplosion(Missile* m);
private:
	Vector2 _pos;
	Vector2 _startPos;
	Vector2 _direction;
	Vector2 _destination;
	float _explosionTime;
	float _speed;
	float _timer;
	float _diametro;
	bool _exploting;
	bool _animShoot;
	//bool _moving;
	float _maxTime;
	std::vector<Missile*> _missiles;
	std::list<Explosion*> _secondaryExplosions;
	uint32_t _color;
	RenderThread* _renderThread;
	Explosion* _explosion;

};

