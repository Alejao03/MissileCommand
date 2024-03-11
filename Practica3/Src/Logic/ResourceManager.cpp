#include "ResourceManager.h"
#include "Renderer.h"
#include "Platform.h"
#include <exception>
#include <assert.h>
#include <cstdint>
#include <Endians.h>
#include <iostream>
#include "EndianConversions.h"
#include "Image.h"
ResourceManager* ResourceManager::_instance = nullptr;

ResourceManager::ResourceManager()
{
}

// Gets an image from the resource manager, returns nullptr if the image is not found
Image* ResourceManager::privGetImage(const std::string& imageName)
{
	auto image = this->_imageMap.find(imageName);
	if (image != this->_imageMap.end()) {
		return image->second;
	}
	else {
		return nullptr;
	}
}

ResourceManager::~ResourceManager()
{
	// Borrar imagenes (Eliminacion para todas las imagenes del array de pixeles)
	for (auto it = _imageMap.begin(); it != _imageMap.end(); ++it) {
		Renderer::Instance()->releaseImage(it->second);
	}
}

bool ResourceManager::Init()
{
	assert(_instance == nullptr);
	_instance = new ResourceManager();
	return true;
}

void ResourceManager::Release()
{
	if (_instance != nullptr)
		delete _instance;
	_instance = nullptr;
}

ResourceManager* ResourceManager::Instance()
{
	return _instance;
}

// Loads an image from a file and stores it in the resource manager
// Returns the image if the image was loaded correctly, nullptr otherwise
Image* ResourceManager::getImage(const std::string& path)
{
	// Image key is filename plus extension without path
	std::string key = path.substr(path.find_last_of("\\") + 1, path.find_last_of(".") - path.find_last_of("\\") - 1);

	Image* img = this->privGetImage(key);
	if (!img)
	{
		FILE* file = Platform::Instance()->openBinaryFile(path);

		if (file == nullptr)
			return nullptr;

		// Read the width and height
		uint32_t width = 0;
		uint32_t height = 0;
		size_t error = 0;
		error = fread(&width, sizeof(uint32_t), 1, file);
		if (error == 0)
		{
			std::cerr << "Failed to read image width from file: " << path << std::endl;
			fclose(file);
			return nullptr;
		}

		error = fread(&height, sizeof(uint32_t), 1, file);
		if (error == 0)
		{
			std::cerr << "Failed to read image height from file: " << path << std::endl;
			fclose(file);
			return nullptr;
		}

#ifdef  PLATFORM_LITTLE_ENDIAN
		width = SWAP_ENDIANESS32(width);
		height = SWAP_ENDIANESS32(height);
#endif // ! ENDIANESS_LITTLE

		// Read the pixel data
		uint32_t* pixelData = new uint32_t[width * height];
		const uint32_t pixelDataSize = width * height;
		//int  bytesRead = fread(pixelData, sizeof(uint32_t), pixelDataSize, file);

		if (!pixelData) {
			std::cerr << "Failed to allocate memory for pixel data." << std::endl;
			fclose(file);
			return nullptr;
		}
		error = fread(pixelData, sizeof(uint32_t), pixelDataSize, file);

		for (int i = 0; i < pixelDataSize; i++)
		{
#ifdef  PLATFORM_LITTLE_ENDIAN
			pixelData[i] = SWAP_ENDIANESS32(pixelData[i]);
#endif
			pixelData[i] = CONVERT_RGBA_TO_ARGB(pixelData[i]);
		}

		// Close the file
		int returnation = fclose(file);
		if (returnation == EOF)
		{
			delete[] pixelData; // Clean up the allocated memory
			return nullptr;
		}
		img = Renderer::Instance()->createImage(width, height, pixelData);
		if (img)
		{
			this->_imageMap.emplace(key, img);
		}
		else
		{
			std::cerr << "Failed to create image from pixel data: " << path << std::endl;
			delete[] pixelData; // Clean up the allocated memory
			return nullptr;
		}
	}

	// Create the sprite
	return img;
}
//DUDA lo dijo en clase pero se deja o se quita?
void ResourceManager::removeImage(const std::string& imagePath)
{
	std::string key = imagePath.substr(imagePath.find_last_of("\\") + 1, imagePath.find_last_of(".") - imagePath.find_last_of("\\") - 1);
	auto it = _imageMap.find(key);
	if (it != _imageMap.end()) {
		Renderer::Instance()->releaseImage(it->second);
		_imageMap.erase(it);
	}
}
