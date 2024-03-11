#pragma once
#include <cstdint>
#include "Vector2.h"
struct Color;
class RenderThread;
class OurCircle
{
public:
	//winHeight -> para sacar la velocidad del circulo
	OurCircle(RenderThread* rThread, Vector2 pos, float r, int rM, uint32_t color, float time);
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
	/// Determina si una posicion se encuentra dentro del circulo
	/// </summary>
	///<param name="other">Posicion a examinar en Vector2D</param>
	/// <returns>true si se encuentra o no</returns>
	bool isInside(Vector2 other);
	/// <summary>
	/// Devuelve el radio del Circulo
	/// </summary>
	/// <returns>El radio del circulo</returns>
	float getR() const;
private:
	Vector2 _pos;
	float _r;
	float _rMax;
	float _speed;
	bool _grow = true;

	uint32_t _color;
	RenderThread* _renderThread;
};
