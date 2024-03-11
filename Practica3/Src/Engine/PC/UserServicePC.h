#pragma once
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include "SDLEventListener.h"
#include "UserInfo.h"

struct _SDL_GameController;

typedef int32_t SDL_Keycode;
typedef struct _SDL_GameController SDL_GameController;
typedef int32_t Sint32;
typedef Sint32 SDL_JoystickID;
typedef int64_t UserID;

enum class InputType {
	TECLADO, TECLADO_RATON, MANDO, UNKNOWN
};

struct UserInfoPC {
	UserID userId;
	bool conected;
	InputType typeInput;
};

struct ControllerInfo {
	SDL_GameController* _controller;
	SDL_JoystickID _controllerID;
};

class UserServicePC : public SDLEventListener
{
public:
	/// <summary>
	/// Constructor
	/// </summary>
	UserServicePC();
	/// <summary>
/// Destructor
/// </summary>
	~UserServicePC();

	/// <summary>
	/// Método que inicializa el UserService
	/// </summary>
	/// <returns>true si todo ha ido bien, false en caso de que algo falle</returns>
	static bool Init();
	/// <summary>
	/// Método que libera la memoria que haya creado el UserService
	/// </summary>
	static void Release();
	/// <summary>
	/// 
	/// </summary>
	/// <returns>puntero a la instancia del UserService, nullptr si no existe </returns>
	static UserServicePC* Instance();
	/// <summary>
	/// Añadimos al usuario a la lista de usuarios conectados y a la de nuevos usuarios, guardando su ID y su color correspondiente
	/// </summary>
	void addUser(UserID userId);
	/// <summary>
	/// Eliminamos al usuario a la lista de usuarios conectados y lo añadimos a la lista de usuarios desconectados,
	///  liberando el color de dicho usuario
	/// </summary>
	void removeUser(UserID userId);
	/// <summary>
	/// Buscamos si el ID de usuario esta en la lista de usuarios conectados
	/// </summary>
	/// <returns>Si el usuario esta en la lista devuelve true, en caso contrario devuelve false</returns>
	bool isUserConnected(UserID userId);
	/// <summary>
	/// Devuelve el color del usuario asociado
	/// </summary>
	/// <param name="userId">id del usuario del que se quiere su color</param>
	/// <returns></returns>
	UserColor getUserColor(UserID userId);//DUDA cambiar esto a que devuelva UserColor
	/// <summary>
	/// Buscamos si la lista de usuarios esta vacia
	/// </summary>
	/// <returns>Si la lista de usuarios esta vacia devuelve true, en caso contrario devuelve false</returns>
	bool isEmpty();

	/// <summary>
	/// Realizamos la lectura tanto de conexion y desconexion de mandos, como de conexion y desconexion 
	/// de usuarios de teclado y teclado-raton mediante las teclas F1 y F2
	/// </summary>
	void update();
	/// <summary>
	/// Usuarios conectados actualmente
	/// </summary>
	/// <returns>vector de las id de los usuarios</returns>
	const std::vector<UserID>& getConnectedUsers() const;
	/// <summary>
	/// Usuarios nuevos conectados desde el ultimo frame
	/// </summary>
	/// <returns>vector de las id de los usuarios</returns>
	const std::vector<UserID>& getNewConnectedUsers() const;
	/// <summary>
	/// Usuarios  desconectados desde el ultimo frame
	/// </summary>
	/// <returns>vector de las id de los usuarios</returns>
	const std::vector<UserID>& getDisconnectedUsers() const;
	/// <summary>
	/// Tipo de input que usa el usuario con la id proporcionada
	/// </summary>
	/// <param name="userId"></param>
	/// <returns>tipo de input</returns>
	InputType getUserType(UserID userId) const;
	/// <summary>
	/// Buscamos la informacion del controller con su ID
	/// </summary>
	/// <returns>Devuelve el struct ControllerInfo perteneciente al ID</returns>
	ControllerInfo getControllerInfo(UserID uid);
	/// <summary>
/// Recibe los eventos de SDL
/// </summary>
	void receive(const SDL_Event& event) override;
private:
	static UserServicePC* _instance;
	std::vector<SDL_Event> _events;
	std::vector<UserID> _connectedUsers;
	std::vector<UserID> _newConnectedUsers;
	std::vector<UserID> _disconnectedUsers;
	std::unordered_map<UserID, ControllerInfo> _controllerToUserIndex;
	std::unordered_map<UserID, UserColor> _colors;
	std::unordered_map<UserID, InputType> _userTypes;
	UserID _user;
	bool _f1Pressed;
	bool _f2Pressed;
	UserInfoPC _UserF1;
	UserInfoPC UserF2;
	int connectedControllers;

	/// <summary>
	/// Buscamos el siguiente color disponible para los usuarios
	/// </summary>
	/// <returns>El primer color disponible</returns>
	UserColor findAvailableColor();



	/// <summary>
	/// Comprueba si el color del usuario es valido
	/// </summary>
	/// <returns>Si el color es valido devuelve true, en caso contrario devuelve false</returns>
	bool isValidUserColor(UserColor color);
	/// <summary>
	/// Limpiamos todas las listas
	/// </summary>
	void clean();
};