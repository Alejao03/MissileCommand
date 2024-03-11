#pragma once

#include <vector> 
#include "Cursor.h"
#include "UserInfo.h"
#include "UserService.h"//Para tener el UserID

#define SCREEN_WIDTH Renderer::Instance()->getWidth()
#define SCREEN_HEIGHT Renderer::Instance()->getHeight()
#define NUM_MISSILES 5
#define NUM_CITIES 5
#define NUM_CURSORS 4
#define NUM_TURRETS NUM_CITIES-1
#define RESTART_TIME 3.0f

class Missile;
class City;
class RenderThread;
class PlayerTurret;
class Sprite;

class MissileScene 
{
private:
    RenderThread* _renderThread;
    std::vector<Missile*> _misiles;
    std::vector<City*> _cities;
    std::vector<PlayerTurret*> _turrets;
    std::vector<Sprite*> _cursorSprites;
    std::vector<UserID> _newConnectedUsers;
    std::vector<UserID> _disconnnectedUsers;
    std::vector<Cursor*> _cursors;
    Cursor* _cursor;
    int _citiesDestroyed;
    float _timer;
    float _refreshCitiesTime;
    int _floorHeight;
    float _cityWidth;
    bool _newUsers;


    /// <summary>
    /// Inicializa los misiles cargando sus imagenes a partir del ResourceManager
    /// Y las ajusta en su posicion en pantalla
    // <returns>Devuelve true si la inicialización fue exitosa, false en caso contrario.</returns>
    bool initCities();
    /// <summary>
    /// Inicializa las torretas en la escena del misil (creacion y posicionamiento del objetos PlayerTurret).
    /// Por defecto las torretas son todas azules si no tienen usuarios asociados
    /// </summary>
    /// <returns>Devuelve true si la inicialización fue exitosa, false en caso contrario.</returns>
    bool initTurrets();
    /// <summary>
    /// Reinicia la escena en general (llamada a los reinicios de los elementos : PlayerTurret/ Missile / Cities
    /// </summary>
    void restartScene();
    /// <summary>
    /// Inicializa tantos cursores nuevos haya como nuevos  usuarios conectados (Obteniendo su informacion por medio de newConnectedUsers)
    /// </summary>
    void addCursors();
    /// <summary>
    /// Borra tantos cursores nuevos haya como nuevos  usuarios desconectados (Obteniendo su informacion por medio de disconnectedUsers)
    /// </summary>
    void removeCursors();
    /// <summary>
    /// Ajusta el color de las torretas en funcion de el numero de torretas no destruidas y el numero /color de usuarios conectados
    /// </summary>
    void adjustTorretColor();
public:
    /// <summary>
    /// Escena de misiles bombardenado la ciudad
    /// </summary>
    /// <param name="w"> ancho de la pantalla</param>
    /// <param name="h">alto de la pantalla</param>
    /// <param name="missileAmmount">cantidad de misiles</param>
    /// <param name="citiesAmmount">cantidad de ciudades</param>
    MissileScene();
    /// <summary>
    /// Inicializa la escena
    /// </summary>
    /// <param name="hilo">hilo en el que se debe renderizar la escena</param>
    /// <returns>true si se inicia correctamente, false en caso contario</returns>
    bool init(RenderThread* hilo);
    /// <summary>
    /// Libera los recursos de la escena
    /// </summary>
    void release();
    /// <summary>
    /// actualiza todo lo de a escena
    /// </summary>
    /// <param name="deltaTime"></param>
    void update(float deltaTime) ;
    /// <summary>
    /// Renderiza la escena
    /// </summary>
    void render() ;
};

