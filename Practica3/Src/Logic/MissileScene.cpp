#include "MissileScene.h"
#include "Utils.h"
#include "Missile.h"
#include "City.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Image.h"
#include "Vector2.h"
#include "Colors.h"
#include "RenderThread.h"
#include "PlayerTurret.h"
#include <iostream>


MissileScene::MissileScene() : _renderThread(nullptr), _citiesDestroyed(0), _timer(0.0f), _refreshCitiesTime(RESTART_TIME), _newUsers(false) {}

bool MissileScene::init(RenderThread* renderThread)
{
	//esto esta asi para probar en mi ordenador que si no no me deja 
	//Comentar y descomentar y ya lo borraremos

	this->_renderThread = renderThread;
	if (!initCities()) return false;
	if (!initTurrets()) return false;

	Image* img = ResourceManager::Instance()->getImage("assets\\cursores.rgba");
	if (img == nullptr)return false;

	for (int cell = 0; cell < NUM_CURSORS; ++cell) {
		float row = 0;
		float col = cell;
		float xImage = col * (img->getWidth() / NUM_CURSORS);
		float yImage = row * img->getHeight();

		Sprite* frame = new Sprite(img, { xImage, yImage }, img->getWidth() / NUM_CURSORS, img->getHeight());
		_cursorSprites.push_back(frame);
	}

	//Misiles
	for (int i = 0; i < NUM_MISSILES; ++i) {
		Missile* misile = new Missile({ (float)Utils::randomInt(0, SCREEN_WIDTH), 0.0f }, { (float)Utils::randomInt(0, SCREEN_WIDTH), (float)SCREEN_HEIGHT }, SCREEN_HEIGHT - _cityWidth / 3, _cities, _turrets, this->_renderThread, Color::White(), 12.0f);
		_misiles.push_back(misile);
	}
	for (PlayerTurret* t : _turrets) {
		t->setMissiles(_misiles);
	}

	for (UserID user : UserService::Instance()->getConnectedUsers()) {

	_cursors.push_back(new Cursor(Vector2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), _cursorSprites[UserService::Instance()->getUserColor(user)], user, _renderThread, _turrets, SCREEN_WIDTH * 0.05, SCREEN_WIDTH * 0.05));
	}


	return true;
}

bool MissileScene::initCities() {
	Image* img = ResourceManager::Instance()->getImage("assets\\ciudad.rgba");
	if (img == nullptr)return false;
	//Dimensiones Sprite 
	const int numframes_ = 8;
	float originalSpriteWidth = img->getWidth() / numframes_;
	float originalSpriteHeight = img->getHeight();
	//Resolucion Original
	float spriteResolution = originalSpriteWidth / originalSpriteHeight;
	//Tienen que ocupar el ancho total de la pantalla
	float spriteWidth = SCREEN_WIDTH / NUM_CITIES;
	_cityWidth = spriteWidth;
	//En este caso la altura esta calculada usando el ancho / resolucion orginal del sprite 
	//El alto es igual al ancho por que el factr de escala es 1 (256x256)
	//Dividimos entre la resolucion para que no se deformen al modificar el ancho 
	float spriteHeight = spriteWidth / spriteResolution;

	_floorHeight = spriteHeight * 2 / 3;

	// Ciudades
	for (int i = 0; i < NUM_CITIES; ++i) {
		int x = i * spriteWidth;

		int y = SCREEN_HEIGHT - spriteHeight;
		City* city = new City({ (float)x, (float)y }, spriteWidth, spriteHeight, this->_renderThread, img);

		_cities.push_back(city);
	}
	return true;
}

bool MissileScene::initTurrets() {
	//Torretas
	Image* imgTurret = ResourceManager::Instance()->getImage("assets\\torretas.rgba");
	if (imgTurret == nullptr)return false;
	//Dimensiones Sprite 
	const int numframes_ = 4;
	float originalSpriteWidth = imgTurret->getWidth() / numframes_;
	float originalSpriteHeight = imgTurret->getHeight();
	//Resolucion Original
	float spriteResolution = originalSpriteWidth / originalSpriteHeight;
	// Calcular dimensiones de la torreta
	float turretWidth = (_cityWidth / 4.0f);
	float turretHeight = turretWidth / spriteResolution;
	//Con el factor de escala de tamaño podemos calcular el factor de escala de forma vertical y horizontal

	float hScaleFactor = turretWidth / originalSpriteWidth;
	float vScaleFactor = turretHeight / originalSpriteHeight;


	// Colocar una torreta entre cada par de ciudades
	for (int i = 0; i < NUM_TURRETS; ++i) {
		//float xInter = (_cities[i + 1]->getX() - (_cities[i]->getX())) * (i + 1);
		float xCenter = _cityWidth * (i + 1);
		

		float x = xCenter - (turretWidth / 2.0);
	

		float y = SCREEN_HEIGHT - _floorHeight - (turretHeight / 2.0f);

		//De momento el color va a piñon puesto que no tenemos ningun input 
		PlayerTurret* turret = new PlayerTurret({ x, y }, turretWidth, turretHeight, _renderThread, imgTurret, static_cast<TurretColor>(0));
		_turrets.push_back(turret);
	}
	adjustTorretColor();
	return true;
}

void MissileScene::release()
{
	/*delete _cursor;
	_cursor = nullptr;*/
	while (!_cursors.empty())
	{
		Cursor* aux = _cursors.back();
		_cursors.pop_back();
		delete aux;
	}
	while (!_cursorSprites.empty())
	{
		Sprite* aux = _cursorSprites.back();
		_cursorSprites.pop_back();
		delete aux;
	}
	while (!_misiles.empty())
	{
		Missile* aux = _misiles.back();
		_misiles.pop_back();
		delete aux;
	}

	while (!_cities.empty())
	{
		City* aux = _cities.back();
		_cities.pop_back();
		delete aux;
	}
	while (!_turrets.empty())
	{
		PlayerTurret* aux = _turrets.back();
		_turrets.pop_back();
		delete aux;
	}
}

void MissileScene::update(float deltaTime)
{


	// Actualizar todos los misiles en la lista
	for (Missile* misile : _misiles) {
		misile->update(deltaTime);
	}

	// Actualizar todas las ciudades en la lista
	int nCitiesDestroyed = 0;
	int nTurretsDestroyed = 0;
	for (City* city : _cities) {
		city->update(deltaTime);
		if (city->isDestroyed())nCitiesDestroyed++;
	}
	for (PlayerTurret* turret : _turrets) {
		turret->update(deltaTime);
		if (turret->isDestroyed())nTurretsDestroyed++;
	}
	addCursors();

	if (_cursors.empty()) //Si no hay ningun player
	{
		for (int i = 0; i < _turrets.size(); i++) {
			_turrets.at(i)->AdjustUser(static_cast<TurretColor>(0)); //Color Azul
		}
	}

	removeCursors();
	for (Cursor* c : _cursors)
		c->update(deltaTime);
	if (nCitiesDestroyed == _cities.size() || nTurretsDestroyed == _turrets.size()) {
		_timer += deltaTime;
		if (_timer > _refreshCitiesTime) {
			restartScene();
		}
	}
}

void MissileScene::restartScene()
{
	_timer = 0;
	for (City* city : _cities) {
		city->restart();
	}
	for (Missile* m : _misiles) {
		m->restartJourney();
	}
	for (PlayerTurret* t : _turrets) {
		t->restart();
	}
	_citiesDestroyed = 0;
}

void MissileScene::addCursors()
{
	_newConnectedUsers = UserService::Instance()->getNewConnectedUsers();
	for (int32_t u : _newConnectedUsers)
	{
		_cursors.push_back(new Cursor(Vector2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), _cursorSprites[UserService::Instance()->getUserColor(u)], u, this->_renderThread, _turrets, SCREEN_WIDTH * 0.05, SCREEN_WIDTH * 0.05));
	}

	if (!_newConnectedUsers.empty()) //Actualizar color torretas
	{
		adjustTorretColor();

	}
}
void MissileScene::adjustTorretColor() {
	std::vector<UserID> users = UserService::Instance()->getConnectedUsers();
	switch (users.size())
	{
	case 1:
		for (int i = 0; i < _turrets.size(); i++) {
			_turrets.at(i)->AdjustUser(static_cast<TurretColor>(UserService::Instance()->getUserColor(users.at(0))));
		}
		break;
	case 2:
		for (int i = 0; i < _turrets.size(); i++) {
			_turrets.at(i)->AdjustUser(static_cast<TurretColor>(UserService::Instance()->getUserColor(users.at(i / 2))));
		}
		break;
	case 3:
		for (int i = 0; i < _turrets.size(); i++) {
			if (i < 2)
				_turrets.at(i)->AdjustUser(static_cast<TurretColor>(UserService::Instance()->getUserColor(users.at(0))));
			else if (i < 3)
				_turrets.at(i)->AdjustUser(static_cast<TurretColor>(UserService::Instance()->getUserColor(users.at(1))));
			else
				_turrets.at(i)->AdjustUser(static_cast<TurretColor>(UserService::Instance()->getUserColor(users.at(2))));
		}
		break;
	case 4:
		for (int i = 0; i < _turrets.size(); i++) {
			_turrets.at(i)->AdjustUser(static_cast<TurretColor>(UserService::Instance()->getUserColor(users.at(i))));
		}
		break;
	}
}
void MissileScene::removeCursors()
{
	_disconnnectedUsers = UserService::Instance()->getDisconnectedUsers();

	for (int32_t u : _disconnnectedUsers)
	{
		for (auto it = _cursors.begin(); it != _cursors.end(); )
		{
			Cursor* c = *it;
			if (u == c->getUserId())
			{
				delete c;
				it = _cursors.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
	if(!_disconnnectedUsers.empty())
		adjustTorretColor();

}

void MissileScene::render()
{
	Command renderCommand{}; // Pseudo inicializacion para evitar warning

	//Suelo
	renderCommand.type = DRAW_RECT;
	renderCommand.rectParam = { {0.0f, (float)(SCREEN_HEIGHT - _floorHeight)}, SCREEN_WIDTH, _floorHeight, Color::Green(),true };
	this->_renderThread->enqueCommand(renderCommand);


#ifdef _DEBUG
	float misileDestroyHeight = SCREEN_HEIGHT - (_floorHeight + (_floorHeight / 3)) / 3;
	renderCommand.type = DRAW_LINE;
	LineParam lineParam = { {0.0f, misileDestroyHeight}, {(float)SCREEN_WIDTH, misileDestroyHeight}, Color::Blue() };
	renderCommand.lineParam = lineParam;
	this->_renderThread->enqueCommand(renderCommand);

#endif // _DEBUG

	//Renderizar todos los misiles en la lista
	for (Missile* misile : _misiles) {
		misile->render();
	}

	// Renderizar todas las ciudades en la lista
	for (City* city : _cities) {
		city->render();
	}

	for (PlayerTurret* turret : _turrets) {
		turret->render();
	}

	for (Cursor* c : _cursors)
		if (c != nullptr)
			c->render();

	
}
