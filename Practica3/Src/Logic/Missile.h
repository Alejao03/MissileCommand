#include <cstdint>
#include <vector>
#include "Vector2.h"
struct Color;
class City;
class RenderThread;
class PlayerTurret;
class Missile
{
public:
	Missile(Vector2 pos, Vector2 end,int dieHeight, std::vector<City*> _cities, std::vector<PlayerTurret*> turrets, RenderThread* renderThread ,uint32_t color, int maxTime = 4);
	/// <summary>
	/// Mueve el misil
	/// </summary>
	/// <param name="deltaTime">deltatime de la plataforma</param>
	/// <returns></returns>
	bool update(float deltaTime);
	/// <summary>
	/// Renderiza el recorrido y al propio misil
	/// </summary>
	void render();
	/// <summary>
	/// Determina si el misil ha explotado ya
	/// </summary>
	/// <returns>true si sigue vivo, false si debe morir</returns>
	bool isAlive();
	/// <summary>
	/// Reenvia al misil a la parte superior de la pantalla
	/// </summary>
	void restartJourney();
	/// <summary>
	/// Posicion en x del misil
	/// </summary>
	/// <returns>coordenada horizontal</returns>
	int getX()const;
	/// <summary>
	/// Posicion y del misil
	/// </summary>
	/// <returns>coordenada vertical</returns>
	int getY()const;
	/// <summary>
	/// Posicion  X Y del misil
	/// </summary>
	/// <returns>coordenadas XY en formato Vector2D</returns>
	Vector2 getPos() const;
	/// <summary>
	/// Mata al misil activando su booleano de destruccion
	/// </summary>
	void destroy();

private:
	Vector2 _pos;
	Vector2 _startPos;
	Vector2 _direction;
	float _speed;
	float _timer;
	int _dieHeight;
	bool _destroyed;
	std::vector<City*> _citiesList;
	std::vector<PlayerTurret*> _turrets;
	uint32_t _color;
	RenderThread* _renderThread;
};