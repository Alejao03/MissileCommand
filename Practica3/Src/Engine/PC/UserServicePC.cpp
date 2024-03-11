#include "UserServicePC.h"
#include <algorithm>
#include <assert.h>
#include <SDL.h>
#include <iostream>
#include "UserInfo.h"
#include "Platform.h"

#define MAX_CONTROLLERS 2

// UserService.cpp
UserServicePC* UserServicePC::_instance = nullptr;

UserServicePC::UserServicePC()
{
	_user = 0;
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	PlatformPC::Instance()->addInputListener(this);
	//DUDA ESTO AUNQUE NO FALLE DEBERIA DE IR EN OTRO LADO O NO HACE FALTA 
	_f1Pressed = false;
	_f2Pressed = false;
	_UserF1.conected = false;
	UserF2.conected = false;
	_UserF1.typeInput = InputType::TECLADO_RATON;
	UserF2.typeInput = InputType::TECLADO;
	connectedControllers = 0;
}

UserServicePC::~UserServicePC()
{
	clean();
	PlatformPC::Instance()->removeInputListener(this);
}

bool UserServicePC::Init()
{
	assert(_instance == nullptr);

	_instance = new UserServicePC();
	//Control de aleatoriedad
	srand(time(nullptr));

	return true; //DUDA: Que devolvemos
}

void UserServicePC::Release()
{
	//Borramos instancia
	if (_instance != nullptr)
		delete _instance;
	_instance = nullptr;
}

UserServicePC* UserServicePC::Instance()
{
	return _instance;
}

void UserServicePC::addUser(UserID userId)
{
	if (!isUserConnected(userId)) // Si no esta conectado el user
	{
		//Cogemos su color e Id y lo guardamos en el vector connectedUsers
		UserColor userColor = findAvailableColor();

		_colors.emplace(userId, userColor);
		_connectedUsers.push_back(userId);
		_newConnectedUsers.push_back(userId);
	}
}

UserColor UserServicePC::findAvailableColor()
{
	for (int i = 0; i < UserColorCount; ++i)
	{
		UserColor colorToCheck = static_cast<UserColor>(i);

		if (std::find_if(_connectedUsers.begin(), _connectedUsers.end(),
			[this, colorToCheck](const UserID& uID) { return static_cast<UserColor>(getUserColor(uID)) == colorToCheck; }) == _connectedUsers.end())
		{
			return colorToCheck;
		}

	}

	// Si todos los colores están en uso, simplemente asigna el primer color
	return static_cast<UserColor>(0);
}

void UserServicePC::removeUser(UserID userId)
{
	auto it = _connectedUsers.begin();
	while (it != _connectedUsers.end()) {
		if (*it == userId) {
			// Encontramos un elemento que cumple la condición, lo eliminamos
			_disconnectedUsers.push_back(*it);
			it = _connectedUsers.erase(it);  // La función erase devuelve el iterador siguiente al elemento borrado
		}
		else {
			++it;
		}
	}
	auto it2 = _colors.find(userId);
	if (it2 != _colors.end()) _colors.erase(it2);

}

bool UserServicePC::isUserConnected(UserID userId) //Busca si el User esta en el vector de connectedUsers
{
	return std::any_of(_connectedUsers.begin(), _connectedUsers.end(),
		[userId](const UserID& uID) { return uID == userId; });
	return false;
}

UserColor UserServicePC::getUserColor(UserID userId)
{
	auto it = std::find_if(_connectedUsers.begin(), _connectedUsers.end(),
		[userId](const UserID& uID) { return uID == userId; });

	if (it != _connectedUsers.end())
	{
		return _colors[*it];
	}

	return UserColor::UserColorCount;
}

bool UserServicePC::isEmpty()
{
	return _connectedUsers.empty();
}

void UserServicePC::update()
{
	_disconnectedUsers.clear();
	_newConnectedUsers.clear();

	for (const SDL_Event& e : _events) {
		switch (e.type) {
			// Gestion del gamepad
		case SDL_CONTROLLERDEVICEADDED: {
			if (connectedControllers < MAX_CONTROLLERS) //Si puede inclurise un nuevo mando
			{

				SDL_GameController* controller = SDL_GameControllerOpen(e.cdevice.which);
				if (controller) {
					SDL_Joystick* joystick = SDL_GameControllerGetJoystick(controller);
					SDL_JoystickID addedControllerID = SDL_JoystickInstanceID(joystick);
					_user++;
					addUser(_user);
					_userTypes[_user] = InputType::MANDO;
					// Asociar el color al ID del usuario en el mapa
					ControllerInfo cInfo;
					cInfo._controller = controller;
					cInfo._controllerID = addedControllerID;
					_controllerToUserIndex.emplace(_user, cInfo);
					connectedControllers++;

				}
			}
			break;
		}
		case SDL_CONTROLLERDEVICEREMOVED: {
			// Obtener el color asociado al ID del controlador
			auto it = _controllerToUserIndex.begin();
			while (it != _controllerToUserIndex.end()) {
				if (it->second._controllerID == e.cdevice.which)break;
				it++;
			}
			if (it != _controllerToUserIndex.end()) {
				// Acceder a la lista y eliminar el usuario con el índice especificado
				removeUser(it->first);
				// Eliminar la entrada del mapa
				_controllerToUserIndex.erase(it);
				connectedControllers--;
			}
			break;
		}
		case SDL_KEYDOWN: {
			switch (e.key.keysym.scancode)
			{
			case SDL_SCANCODE_F1:
				if (!_f1Pressed) {

					if (!_UserF1.conected) {
						_user++;
						_UserF1.userId = _user;
						addUser(_UserF1.userId);
						_userTypes[_UserF1.userId] = InputType::TECLADO_RATON;
						_UserF1.conected = true;
					}
					else {
						removeUser(_UserF1.userId);
						_UserF1.conected = false;
					}
					_f1Pressed = true;
				}
				break;
			case SDL_SCANCODE_F2:

				if (!UserF2.conected) {
					_user++;
					UserF2.userId = _user;
					addUser(UserF2.userId);
					_userTypes[UserF2.userId] = InputType::TECLADO;
					UserF2.conected = true;
				}
				else {
					removeUser(UserF2.userId);
					UserF2.conected = false;
				}
				_f2Pressed = true;
			}
			break;
		}
						break;
		case SDL_KEYUP: {
			switch (e.key.keysym.scancode)
			{
			case SDL_SCANCODE_F1:
				_f1Pressed = false;
				break;
			case SDL_SCANCODE_F2:
				_f2Pressed = false;
				break;
			}
		}
					  break;
		}
	}

	_events.clear();
}

const std::vector<UserID>& UserServicePC::getConnectedUsers() const
{
	return _connectedUsers;
}

const std::vector<UserID>& UserServicePC::getNewConnectedUsers() const
{
	return _newConnectedUsers;
}

const std::vector<UserID>& UserServicePC::getDisconnectedUsers() const
{
	return _disconnectedUsers;
}

InputType UserServicePC::getUserType(UserID userId) const
{
	auto it = _userTypes.find(userId);
	if (it != _userTypes.end())
	{
		return it->second;
	}
	return InputType::UNKNOWN; 
}

ControllerInfo UserServicePC::getControllerInfo(UserID uid)
{

	return _controllerToUserIndex.find(uid)->second;
}


bool UserServicePC::isValidUserColor(UserColor color)
{
	// Agrega lógica para verificar si el color es válido
	return color != UserColor::UserColorCount;
}
void UserServicePC::clean()
{
	_connectedUsers.clear();
	_newConnectedUsers.clear();
	_disconnectedUsers.clear();
	_controllerToUserIndex.clear();
	_colors.clear();
	_userTypes.clear();
}

void UserServicePC::receive(const SDL_Event& sdlEvent)
{
	this->_events.push_back(sdlEvent);
}

