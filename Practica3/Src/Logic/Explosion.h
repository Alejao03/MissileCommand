#pragma once
#include <cstdint>
#include <vector>
#include "Vector2.h"

struct Color;
class RenderThread;
class Missile;
class PlayerMissile;

class Explosion
{
public:
	//winHeight -> para sacar la velocidad del circulo
	Explosion(RenderThread* rThread, Vector2 pos, PlayerMissile* pM, std::vector<Missile*>missile, float r, int rM, uint32_t color, float time);
	/// <summary>
	/// Hace crecer o decrecer el circulo
	/// </summary>
	/// <param name="deltaTime">deltatime de la plataforma</param>
	/// <returns></returns>
	bool update(float deltaTime);
	/// <summary>
	/// Renderiza el circulo
	/// </summary>
	void render();
	/// <summary>
	/// Determina si el circulo sigue existiendo o no
	/// </summary>
	/// <returns>true si sigue vivo, false si debe morir</returns>
	bool isAlive();
	/// <summary>
	/// Determina si la posicion other del tipo Vector2D (con x e y) se encuentra dentro del circulo de la explosion
	/// </summary>
	/// <param_name='other'> posicion a examinar en formato Vector2D <\param>
	/// <returns>true si se encuentyra dentro</returns>
	bool isInside(Vector2 other);
	/// <summary>
	/// Devuelve el radio del circulo
	/// </summary>
	/// <returns>float _r </returns>
	float getR() const;
private:
	std::vector<Missile*> _missiles;
	PlayerMissile* _pM;
	Vector2 _pos;
	float _r;
	float _rMax;
	float _speed;
	bool _grow = true;

	uint32_t _color;
	RenderThread* _renderThread;
};
