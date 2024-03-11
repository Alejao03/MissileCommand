#pragma once
#include "Image.h"
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

class ResourceManager
{
private:
	static ResourceManager* _instance;
	std::unordered_map<std::string, Image*> _imageMap;
	Image* privGetImage(const std::string& imageName);

	ResourceManager();
	~ResourceManager();
public:
	/// <summary>
	/// Inicializa el controlador de recursos
	/// </summary>
	/// <returns></returns>
	static bool Init();
	/// <summary>
	///  Destruye el controlador de recursos y todo lo que ello supone
	/// </summary>
	static void Release();
	/// <summary>
	/// Instancia del controlador de recursos
	/// </summary>
	/// <returns></returns>
	static ResourceManager* Instance();
	/// <summary>
	/// Devuelve un puntero a la imagen con la ruta dada
	/// </summary>
	/// <param name="imagePath"></param>
	/// <returns>Image Pointer </returns>
	Image* getImage(const std::string& imagePath);
	/// <summary>
	///Elimina la  imagen con la ruta dada
	/// </summary>
	/// <param name="imagePath"></param>
	void removeImage(const std::string& imagePath);
};