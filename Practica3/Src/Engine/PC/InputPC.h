#pragma once
// Input no se incluye en Renderer/Platform

#include "InputState.h"
#include "SDLEventListener.h"
#include "Keyboard.h"
#include "KeyboardMouse.h"
#include "GameController.h"
#include <vector>
#include <cstdint>
#include <unordered_map>

// Para que no se mueva demasiado rapido como pasaba antes, asi funciona más similar a 
// PS4 con los mismos valores de velocidad en cursor


struct _SDL_GameController;
typedef int32_t SDL_Keycode;
typedef struct _SDL_GameController SDL_GameController;
union SDL_Event;

class InputPC : public SDLEventListener
{
	struct KeyBoard
	{
		uint32_t id;
		Keyboard* k;
	};
	struct MouseKey
	{
		uint32_t id;
		KeyboardMouse* kM;
	};
	
private:
	static InputPC* instance_;
	KeyBoard _keyboard;
	MouseKey _mouseKey;
	InputPC();
	
	~InputPC();
	std::unordered_map<uint32_t, GameController*> gameControllers;
	std::vector<SDL_Event> events;

	bool privateInit();
	// Input parameters
	// Necesitamos guardar todos los valores para no tener que hacer encuesta, al guardar los valores
	// sabemos que hubo en la iteración anterior, y si no hay eventos no perdermos tiempo haciendo encuesta
	
	// Reiniciar la estructura InputState iea y tambien los valores de mouse motion
	// debido a que no hay ningún evento que lo anule a diferencia de button down - button up
	// asi que tenemos que resetearlo nosotros en cada iteración

	/// <summary>
	/// Reiniciamos los InputStates
	/// </summary>
	void resetTick();

	// Utilidades

	// Para asignar botones del ratón y de teclado tras leer el input y actualizar variables
	// El estado de iea.x e iea.o tambien lo mantenemos y ya se encargara el cursor de tenerlo en cuenta
	// solo el primer frame en que se pulso

	/// <summary>
	/// Actualizamos los InputStates
	/// </summary>
	void setInputAfterTick();
public:
	/// <summary>
	/// Método que inicializa el Input
	/// </summary>
	/// <returns>true si todo ha ido bien, false en caso de que algo falle</returns>
	static bool Init();
	/// <summary>
	/// Método que libera la memoria que haya creado el Input
	/// </summary>
	static void Release();
	/// <summary>
	/// 
	/// </summary>
	/// <returns>puntero a la instancia del Input, nullptr si no existe </returns>
	static InputPC* Instance();
	/// <summary>
	/// Procesamiento de los eventos de SDL, lectura de mando, teclado y raton
	/// </summary>
	void tick();
	/// <summary>
	/// Recibe los eventos de SDL
	/// </summary>
	void receive(const SDL_Event& event) override;
	/// <summary>
	/// Devuelve el InputState correspondiente al ID del usuario
	/// </summary>
	/// <returns>El InputState perteneciente al ID</returns>
	InputState getInput(uint32_t userId);
};
//DUDA: Recolocar algunos metodos, no se si receive es public