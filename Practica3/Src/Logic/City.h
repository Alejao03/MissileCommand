#pragma once

#include "Image.h"
#include "Vector2.h"
class Animator;
class RenderThread;
class Missile;
class City
{
private:
	Vector2 _pos;
	int _width;
	int _height;
	Vector2 _posCollider;
	int _colliderWidth;
	int _colliderHeight;
	bool _destroyed;
	Animator* _animator;
	Image* _city;
	RenderThread* _renderThread;
public:
	/// <summary>
	/// Constructor de la ciudad
	/// </summary>
	/// <param name="pos">Posicion en la que se encuentra la ciudad</param>
	/// <param name="width">ancho de la ciudad</param>
	/// <param name="height">alto de la ciudad</param>
	/// <param name="renderThread">Hilo en el que se debe renderizar</param>
	/// <param name="image">imagen de la ciudad</param>
	City(Vector2 pos, int width, int height, RenderThread* renderThread, Image* image);
	~City();
	/// <summary>
	/// Renderiza la ciudad en su posicion con el alto y ancho dados
	/// </summary>
	void render();
	/// <summary>
	/// Actualiza la ciudad
	/// </summary>
	/// <param name="deltaTime"></param>
	void update(float deltaTime);
	/// <summary>
	/// Determina si se destruye la ciudad por el objeto en la posicion 
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <returns></returns>
	bool destroy(Missile* missile);
	/// <summary>
	/// Reinicia la ciudad a su estado inicial
	/// </summary>
	void restart();
	/// <summary>
	/// Posicion de la esquina superior izquierda de la ciudad en el eje horizontal
	/// </summary>
	/// <returns></returns>
	int getX()const;
	/// <summary>
	/// Posicion de la esquina superior izquierda de la ciudad en el eje vertical
	/// </summary>
	/// <returns></returns>
	int getY()const;
	/// <summary>
	/// Ancho de la ciudad
	/// </summary>
	/// <returns></returns>
	int getWidth()const;
	/// <summary>
	/// Alto de la ciudad
	/// </summary>
	/// <returns></returns>
	int getHeight()const;
	/// <summary>
	/// Devuelve si ha sido destruida
	/// </summary>
	/// <returns></returns>
	bool isDestroyed();
};

