#pragma once
#include <vector>
#include "Image.h"

class Sprite;

class Animator {
private:
	std::vector<Sprite*> animationFrames;
	Sprite* _currentSprite;
	int _currentFrame;
	float _frameDuration;
	float _timer;
	bool _canplay;
	std::vector<Sprite*> loadAnimation(const Image* image, int numRows, int numCols, int startCell, int endCell);
	void updateCurrentSprite();

public:
	/// <summary>
	/// Construcotr de la clase animator te permite guardar una animacion en el
	/// </summary>
	/// <param name="image">Imagen en la que se encuentra la animacion</param>
	/// <param name="numRows">nº de filas que ocupa la animacion</param>
	/// <param name="numCols">nº de columnas que ocupa la animacion</param>
	/// <param name="frameDuration">Tiempo que se desea que dure cada frame</param>
	/// <param name="startCell">celda en la que empieza la animacion</param>
	/// <param name="endCell">celda en la que termina la animacion(si no se proporciona considera que acaba en la numRows*numCols-1</param>
	Animator(const Image* image, int numRows, int numCols, float frameDuration, int startCell = 0, int endCell = 0);
	~Animator();
	/// <summary>
	/// Reproduce la animacion si lo tiene permitido
	/// </summary>
	/// <param name="deltaTime"></param>
	void update(float deltaTime);
	/// <summary>
	/// Permite que se reproduzca la animacion
	/// </summary>
	void  play();
	/// <summary>
	/// Vuelve a empezar la animacion
	/// </summary>
	void restartAnimation();
	/// <summary>
	/// Resetea la animacion a su frame inicial, pero no se vuelve a reproducir
	/// </summary>
	void reset();
	/// <summary>
	/// </summary>
	/// <returns>El sprite actual</returns>
	Sprite* getCurrentSprite();
};

