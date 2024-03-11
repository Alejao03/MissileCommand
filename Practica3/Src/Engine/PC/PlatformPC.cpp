#include "PlatformPC.h"
#include <SDL.h>
#include <assert.h>
#include <iostream>
#include <chrono>
#include "SDLEventListener.h"

//Creacion de reloj para controlar el tiempo
typedef std::chrono::high_resolution_clock Clock;

PlatformPC* PlatformPC::_instance = nullptr;

PlatformPC::PlatformPC() {}

PlatformPC::~PlatformPC() {
	//Cerramos SDL
	SDL_Quit();
}

bool PlatformPC::initPrivate()
{
	//Init sdl and create window
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	else
	{
		std::cout << "SDL initialized!" << std::endl;
	}

	return true;
}

void PlatformPC::send(SDL_Event sdlEvent) const
{
	for (auto it = listeners.begin(); it != listeners.end(); ++it)
		(*it)->receive(sdlEvent);
}

bool PlatformPC::Init()
{
	assert(_instance == nullptr);

	_instance = new PlatformPC();
	//Control de aleatoriedad
	srand(time(nullptr));
	bool initTry = _instance->initPrivate();
	if (!initTry) {
		Release();
	}
	return initTry;
}

void PlatformPC::Release() {

	//Borramos instancia
	if (_instance != nullptr)
		delete _instance;
	_instance = nullptr;
}

PlatformPC* PlatformPC::Instance()
{
	return _instance;
}

//Procesamiento de los eventos de SDL (PollEvent)
bool PlatformPC::tick()
{
	bool quit = false;

	//Event handler
	SDL_Event e;

	//Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		//"la aplicación deberá cerrarse al pulsar cualquier tecla o hacer “click” con el ratón."
		if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		{
			quit = true;
		}
		// EVENTOs PARA Listeners de Input
		else {
			send(e);
		}
	}

	return !quit;
}

//Esta función mide el tiempo transcurrido en NS desde un punto de referencia
uint64_t PlatformPC::getTicks()
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now().time_since_epoch()).count();
}

//Esta función devuelve la frecuencia en hercios
uint64_t PlatformPC::getFrec()
{
	return (SDL_GetPerformanceFrequency() * 100.0);
}

FILE* PlatformPC::openBinaryFile(const std::string& name) const
{
	FILE* file = nullptr;
	const char* archivo = name.c_str();
	fopen_s(&file, archivo, "rb"); //permiso lectura
	if (file == NULL) {
		std::cerr << "Couldn't open File " + name << std::endl;
		std::cerr << "Error code: " << errno << std::endl;
	}

	return file;
}

void PlatformPC::addInputListener(SDLEventListener* inputListener)
{
	this->listeners.push_back(inputListener);
}

void PlatformPC::removeInputListener(SDLEventListener* inputListener)
{
	this->listeners.remove(inputListener);
}
