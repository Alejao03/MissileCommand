#include "InputPC.h"
#include <cassert>
#include <deque>
#include <SDL.h>
#include <iostream>
#include "Platform.h"
#include "UserServicePC.h"

InputPC* InputPC::instance_ = nullptr;
#define MAXUSERS 4;

InputPC::InputPC()
{
	_keyboard.k = new Keyboard();
	_mouseKey.kM = new KeyboardMouse();

	PlatformPC::Instance()->addInputListener(this);
}

InputPC::~InputPC()
{
	delete _keyboard.k;
	_keyboard.k = nullptr;
	delete _mouseKey.kM;
	_mouseKey.kM = nullptr;
	for (auto& pair : gameControllers) {
		// pair.first es la clave, pair.second es el valor
		delete pair.second;
		pair.second = nullptr;
	}
	gameControllers.clear();
	PlatformPC::Instance()->removeInputListener(this);
	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
}

bool InputPC::privateInit()
{
	if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0)
	{
		std::cout << "Problemas en la Inicializacion del Mouse Mode del Input";
		return false;
	}
	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
	{
		std::cout << "Problemas en la Inicializacion del GameController Subsystem del Input";
		return false;
	}
	return true;
}

bool InputPC::Init()
{
	assert(instance_ == nullptr);

	instance_ = new InputPC();

	bool initTry = instance_->privateInit();
	if (!initTry) {
		Release();
	}

	return initTry;
}

void InputPC::Release()
{
	if (instance_)
		delete instance_;
	instance_ = nullptr;
}

InputPC* InputPC::Instance()
{
	return instance_;
}

void InputPC::tick()
{


	std::vector<UserID> newC = UserServicePC::Instance()->getNewConnectedUsers();
	std::vector<UserID> leftC = UserServicePC::Instance()->getDisconnectedUsers();
	for (UserID user : newC) {
		if (UserServicePC::Instance()->getUserType(user) == InputType::TECLADO)
			_keyboard.id = user;
		else if (UserServicePC::Instance()->getUserType(user) == InputType::TECLADO_RATON)
			_mouseKey.id = user;
		else if (UserServicePC::Instance()->getUserType(user) == InputType::MANDO) {
			ControllerInfo cInfo = UserServicePC::Instance()->getControllerInfo(user);
			gameControllers.emplace(user, new GameController(cInfo._controller, cInfo._controllerID));
		}
	}
	for (UserID user : leftC) {
		if (UserServicePC::Instance()->getUserType(user) == InputType::MANDO) {
			auto c = gameControllers.find(user);
			if (c != gameControllers.end()) {

				c->second->removeController();
				delete c->second;
				c->second = nullptr;
				gameControllers.erase(user);
			}
		}
	}
	resetTick();
	for (const SDL_Event& e : events) {
		auto key = e.key.keysym.sym;
		switch (e.type)
		{
		case SDL_CONTROLLERBUTTONDOWN:
			//si la id de mi controlador coincide entonces seteo los botones
			for (const auto& pair : gameControllers) {
				// pair.first es la clave, pair.second es el valor
				pair.second->processGameControllerButtonInput(e, true);
			}
			break;
		case SDL_CONTROLLERBUTTONUP:
			for (const auto& pair : gameControllers) {
				pair.second->processGameControllerButtonInput(e, false);
			}
			break;
		case SDL_CONTROLLERAXISMOTION:
			for (const auto& pair : gameControllers) {
				pair.second->processGameControllerAxisMotion(e);
			}
			break;
		case SDL_CONTROLLERSENSORUPDATE:

			for (const auto& pair : gameControllers) {
				pair.second->processGyro(e);
			}
			break;
		case SDL_MOUSEMOTION:
			_mouseKey.kM->proccessMouseMotion(e.motion);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_mouseKey.kM->proccessMouseButton(e.button.button, true);
			break;
		case SDL_MOUSEBUTTONUP:
			_mouseKey.kM->proccessMouseButton(e.button.button, false);
			break;

			// Teclado, por si alguien quiere jugar así
		case SDL_KEYDOWN:
			_keyboard.k->proccessEvent(key, true);
			_mouseKey.kM->proccessKey(key, true);
			break;
		case SDL_KEYUP:
			_keyboard.k->proccessEvent(key, false);
			_mouseKey.kM->proccessKey(key, false);
			break;
		default:
			break;
		}
	}

	events.clear();

	// Con los datos obtenidos, actualizar la estructura iea
	setInputAfterTick();

}

void InputPC::setInputAfterTick()
{
	_mouseKey.kM->updateInputState();
	_keyboard.k->updateInputState();
	for (const auto& pair : gameControllers) {
		// pair.first es la clave, pair.second es el valor
		pair.second->updateInputState();
	}
}

void InputPC::resetTick()
{
	_mouseKey.kM->resetTick();
	_keyboard.k->resetTick();
	for (const auto& pair : gameControllers) {
		// pair.first es la clave, pair.second es el valor
		pair.second->resetTick();
	}

}

InputState InputPC::getInput(uint32_t userId)
{
	if (userId == _keyboard.id)
		return _keyboard.k->GetInputState();
	if (userId == _mouseKey.id)
		return _mouseKey.kM->GetInputState();
	return gameControllers.find(userId)->second->GetInputState();
}

void InputPC::receive(const SDL_Event& sdlEvent)
{
	this->events.push_back(sdlEvent);
}