//#define MEM_TEST_PC
//#define MEM_TEST_PS4
#include "Platform.h"
#include "Renderer.h"
#include "GameTime.h"
#include "Colors.h"
#include "ResourceManager.h"
#include "Utils.h"
#include "MissileScene.h"
#include "Command.h"
#include "RenderThread.h"
#include "Input.h"
#include "UserService.h"

#include <iostream>
#include <cstdint>
#include <list>
#include <stack>
#include <algorithm>
#include <chrono>
#include <thread>

//DUDA : Corregir lo de la Practica 2 , Comentarios y variables ,
#define NUM_DISPLAY_BUFFER 2

// Calcula y muestra los FPS(cuadros por segundo) en base al tiempo transcurrido.
void calculateAndDisplayFPS(GameTime& gameTime, float& timer, float duration) {
	timer += gameTime.getDeltaTime();

	if (timer > duration) {
		timer -= duration;
		std::cout << "FPS: " << 1.0f / gameTime.getDeltaTime() << std::endl;
	}
}

void practica() {
	// ----------- RENDER THREAD ------------------

	RenderThread renderThread;
	renderThread.start();

	//ResourceManager* mngr = ResourceManager::Instance();
	MissileScene scene = MissileScene();
	if (!scene.init(&renderThread))
	{
		std::cerr << "Scene: Init failed" << std::endl;
		renderThread.stop();
		return;
	}

	// COMANDOS ----------------------
	Command clearCommand;
	clearCommand.type = CLEAR;
	clearCommand.color = Color::Background();

	Command presentCommand;
	presentCommand.type = PRESENT;

	//Creamos el "medidor de tiempos"
	GameTime gameTime;

	float timer = 0;
	float duration = 5; // en segundos

	//Asi el tiempo realmente esta bien y no se tienen en cuenta 
	//tiempo innecesario como el que puedan añadir algunas inicializaciones
	gameTime.updateTimeRegistry(Platform::Instance()->getTicks(), Platform::Instance()->getFrec());

	volatile bool finalize;
#ifdef MEM_TEST_PS4
	finalize = true;
	float tiempoVivo = 20.0f;
	float tiempoTranscurrido = 0.0f;
	std::cout << " memoria ps4" << std::endl;
#else
	finalize = Platform::Instance()->tick();
#endif // MEM_TEST_PS4

	while (finalize)
	{
		//Actualiza deltatime	
		gameTime.updateTimeRegistry(Platform::Instance()->getTicks(), Platform::Instance()->getFrec());
		UserService::Instance()->update();
		Input::Instance()->tick();
		scene.update(gameTime.getDeltaTime());
		calculateAndDisplayFPS(gameTime, timer, duration);

		//Limpiamos la pantalla con el color del fondo
		renderThread.enqueCommand(clearCommand);
		//Renderer::Instance()->clear(Color::Background());
		//Renderizado de la escena
		scene.render();
		//Logica de renderizado
		renderThread.enqueCommand(presentCommand);
		//Renderer::Instance()->present();

		while (renderThread.pendingFrame() > NUM_DISPLAY_BUFFER) { 
			//std::this_thread::sleep_for(std::chrono::milliseconds(5)); 
			//Produce un  desbalanceo en nuestro calculo de  FPS por eso lo hemos quitado
			//Hubiese sido mas correcto que con el sleep funcionase el calculo pero
			//Pero por desconocimiento y falta de tiempo no hemos podido indagar mucho mas 
		}
#ifdef MEM_TEST_PS4
		//hacer que a los 5 segundos se termine
		tiempoTranscurrido += gameTime.getDeltaTime();
		if (tiempoTranscurrido > tiempoVivo) {
			finalize = false;
		}
#else
		finalize = Platform::Instance()->tick();
#endif // MEM_TEST_PS4
	}

	// Paramos hebra y liberamos escena
	renderThread.stop();
	scene.release();
}


int main()
{
#ifdef MEM_TEST_PC
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // MEM_TEST


	srand(static_cast<unsigned int>(time(NULL))); // Explicit cast to avoid warning

	//Inicializamos
	if (!Platform::Init())
	{
		std::cerr << "Platform init failed" << std::endl;
		return 1;
	}
	if (!Renderer::Init( 1920, 1080, "Missile command")) { // 1920, 1080

		std::cout << "Renderer init failed" << std::endl;
		Platform::Release();
		return 1;
	}
	if (!ResourceManager::Init())
	{
		std::cout << "ResourceManager init failed" << std::endl;
		Renderer::Release();
		Platform::Release();
		return 1;
	}
	if (!UserService::Init())
	{
		std::cout << "UserService init failed" << std::endl;
		ResourceManager::Release();
		Renderer::Release();
		Platform::Release();
		return 1;
	}
	if (!Input::Init())
	{
		std::cout << "Input init failed" << std::endl;
		UserService::Release();
		ResourceManager::Release();
		Renderer::Release();
		Platform::Release();
		return 1;
	}

	practica();

	//Liberamos memoria
	Input::Release();
	UserService::Release();//deja 1 de memoria en play
	ResourceManager::Release();
	Renderer::Release();//deja 1 de memoria videout
	Platform::Release();
	//Comprobar que solo queda esta basura (COMPROBANTE)
#if defined(MEM_TEST_PC) || defined(MEM_TEST_PS4)
	int* i = new int(2);
	std::cout << "Valor de prueba de memoria  " << *i << std::endl; //deja uno de memoria para asegurar dejar
#endif // MEMTEST

	return 0;
}

