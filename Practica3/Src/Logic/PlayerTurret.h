#pragma once
#include "Vector2.h"
#include "Image.h"
#include <vector>
class RenderThread;
class Sprite;
class Missile;
class PlayerMissile;
class OurCircle;
//Respetamos el orden dado por el motor
enum class TurretColor {
	BLUE,
	RED,
	GREEN,
	PINK
};

class PlayerTurret {
public:
	PlayerTurret(Vector2 pos, int width, int height, RenderThread* renderThread, Image* turretImage, TurretColor color);
	~PlayerTurret();

	/// <summary>
	/// Renderiza la torreta y su explosión secundaria.
	/// </summary>
	void render();
	/// <summary>
	/// Obtiene la posición X actual de la torreta.
	/// </summary>
	/// <returns>Posición X de la torreta.</returns>
	int getX() const;
	/// <summary>
	/// Obtiene la posición Y actual de la torreta.
	/// </summary>
	/// <returns>Posición Y de la torreta.</returns>
	int getY() const;
	/// <summary>
	/// Obtiene el ancho actual de la torreta.
	/// </summary>
	/// <returns>Ancho de la torreta.</returns>
	int getWidth() const;
	/// <summary>
	/// Obtiene la altura actual de la torreta.
	/// </summary>
	/// <returns>Altura de la torreta.</returns>
	int getHeight() const;
	/// <summary>
	/// Obtiene el color actual de la torreta.
	/// </summary>
	/// <returns>Color de la torreta.</returns>
	uint32_t getColor() const;
	/// <summary>
	/// Actualiza el estado de la torreta.
	/// </summary>
	/// <param name="deltaTime">Tiempo transcurrido desde la última actualización.</param>
	void update(float deltaTime);
	/// <summary>
	/// Establece la lista de misiles para verificar colisiones.
	/// </summary>
	/// <param name="missile">Vector de punteros a misiles.</param>
	void setMissiles(std::vector<Missile*> missile);
	void hasShoot(Vector2 dir);
	/// <summary>
	/// Indica si la torreta puede disparar.
	/// </summary>
	/// <returns>True si la torreta puede disparar, false en caso contrario.</returns>
	bool getCanShoot();
	/// <summary>
    /// Indica si la torreta está viva.
	/// </summary>
	/// <returns>True si la torreta está viva, false en caso contrario.</returns>
	bool getAlive();
	/// <summary>
	/// Ajusta el color de la torreta en función del tipo de usuario.
	/// </summary>
	/// <param name="color">Color del usuario.</param>
	void AdjustUser(TurretColor color);
	/// <summary>
	/// Reinicia el estado de la torreta.
    /// </summary>
	void restart();
	/// <summary>
	/// Destruye la torreta si colisiona con un misil.
	/// </summary>
	/// <param name="missile">Puntero al misil que colisiona.</param>
	/// <returns>True si la torreta fue destruida, false en caso contrario.</returns>
	bool destroy(Missile* missile);
	/// <summary>
	/// Indica si la torreta fue destruida.
	/// </summary>
	/// <returns>True si la torreta fue destruida, false en caso contrario.</returns>
	bool isDestroyed();
private:
	Vector2 _pos;
	int _width;
	int _height;
	bool _canShoot;
	uint32_t _colorUser;
	Image* _turretImage;
	std::vector<Sprite*> _turretSprites;
	Sprite* _actualSprite;
	RenderThread* _renderThread;
	TurretColor _color;
	bool _destroyed;
	bool _stopRender;
	bool _isAlive;
	PlayerMissile* _missile;
	OurCircle* _explosion;
	// Collider
	Vector2 _posCollider;
	float _colliderWidth;
	float _colliderHeight;
	float _originalSpriteWidth;
	float _originalSpriteHeight;
	// Frame en la hoja de sprites
	int _spriteFrame;

};