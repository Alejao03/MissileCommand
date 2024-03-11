#pragma once

#include "Vector2.h"

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include "UserService.h"
class Sprite;
class RenderThread;
class PlayerTurret;

class Cursor
{
public:
	Cursor(Vector2 position, Sprite* cursor, UserID userId, RenderThread* renderThread, std::vector<PlayerTurret*> turrets, int width, int height, float vel_ = 850, float giroscopeMultiplier_ = 500);
	~Cursor();
	/// <summary>
	/// Actualizacion de los cursores en el tiempo (atributos trigger , disparo y posicion) en base al identificador del input del user asociado al cuirsor
	/// Por medio del _userID
	/// <param name="deltaTime">tiempo que transcurre en la Aplicacion</param>
	/// </summary>
	void update(float deltaTime);
	/// <summary>
	/// Intenta realizar un disparo en una de las torretas del usuario _UserId que no este destruida ni disparando
	/// En caso contrario no se dispara 
	/// </summary>
	void shoot();
	/// <summary>
	/// Devuelve el Objeto Sprite de la clase cursos (como referencia)
	/// <summary>
	const Sprite* getSprite() const;
	/// <summary>
	/// Devuelve la posicion del Objeto en las coordenads X y Y  del cursor del tipo Vector2D 
	/// <summary>
	Vector2 getPos();
	/// <summary>
	/// Metodo que devuelven el identificador de usuario asociado al cursor creado
	/// <summary>
	/// <returns>_userID </returns>
	const int getUserId() const;

	/// <summary>
	/// Renderiza el cursor en su posicion con el alto y ancho dados
	/// </summary>
	void render();
private:
	std::vector<PlayerTurret*> _turrets;
	Vector2 _position;
	Vector2 _direction;
	Vector2 _size;
	int _width, height;
	float _joystickMultiplier;
	float _giroscopeMultiplier;
	bool _shootedWithButton;
	bool _shootedWithTrigger;
	bool _leftTriggerPressed;
	bool _rightTriggerPressed;

	Sprite* _sprite;

	RenderThread* _renderThread;

	UserID _userId;

	/// <summary>
	/// Actualiza la posicion del cursor en base al tiempo y al movimiento y aceleracion recibidos por el Input
	/// </summary>
	void updatePosition(float deltaTime, const float& motionX, const float& motionY, const float& accelerationX, const float& accelerationY);
	/// <summary>
	/// Controla por medio de booleanos con que boton se esta disparando (si con triggers o con los botones simples)
	/// </summary>
	void handleShoot(bool button);
	/// <summary>
	/// Examina con un umbral si se han pulsado alguno de los triggers (izquierdo o derecho)
	/// </summary>
	void handleTriggers(float triggerLeft, float triggerRight);
};