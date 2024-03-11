#pragma once

#include <cstdint>
#include <cstdio>
#include <string>
#include <list>

union SDL_Event;
class SDLEventListener;

class PlatformPC
{
private:
	PlatformPC();
	~PlatformPC();
	//Metodo que realmente inicializa todo
	bool initPrivate();
	static PlatformPC* _instance; //Instancia estatica de nuestro singleton explicito
	std::list<SDLEventListener*> listeners;
	void send(SDL_Event sdlEvent) const;

public:
	/// <summary>
	/// Procesamiento de los eventos de SDL
	/// </summary>
	/// <returns>true si el programa termina, false en caso de que continue</returns>
	bool tick();
	/// <summary>
	/// Método que inicializa la plataforma
	/// </summary>
	/// <returns>true si todo ha ido bien, false en caso de que algo falle</returns>
	static bool Init();
	/// <summary>
	/// Método que libera la memoria que haya creado la plataforma
	/// </summary>
	static void Release();
	/// <summary>
	/// 
	/// </summary>
	/// <returns>puntero a la instancia de la plataforma, nullptr si no existe </returns>
	static PlatformPC* Instance();
	/// <summary>
	/// Este metodo usa la biblioteca de Chrono para obtener el tiempo del programa
	/// </summary>
	/// <returns>tiempo en nanosegundos que lleva el programa en ejecucion</returns>
	uint64_t getTicks();
	/// <summary>
	/// La frecuencia es constante
	/// </summary>
	/// <returns>Devuelve la frecuencia de la plataforma destino</returns>
	uint64_t getFrec();
	/// <summary>
	/// Abre el archivo de la ruta que se le pasa suponiendo que es un archivo binario
	/// </summary>
	/// <param name="name">ruta al archivo binario que se desea abrir</param>
	/// <returns>nullptr si falla, puntero al fichero si lo consigue</returns>
	FILE* openBinaryFile(const std::string& name) const;

	void addInputListener(SDLEventListener* inputListener);
	void removeInputListener(SDLEventListener* inputListener);
};