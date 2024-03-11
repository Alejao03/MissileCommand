#include "ImagePC.h"
#include "SDL.h"

ImagePC::ImagePC(int width, int height) : _width(width), _height(height), _texture(nullptr)
{

}

SDL_Texture* ImagePC::getTexture() const
{
	return _texture;
}

bool ImagePC::createImage(uint32_t* pixelDat, SDL_Renderer* rnd)
{
	// 0xAARRGGBB
	uint32_t alpha_mask = 0xFF000000;
	uint32_t red_mask = 0x00FF0000;
	uint32_t green_mask = 0x0000FF00;
	uint32_t blue_mask = 0x000000FF;

	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixelDat, _width, _height, 32, _width * 4, red_mask, green_mask, blue_mask, alpha_mask);

	if (surface == nullptr) {
		delete[] pixelDat;
		return false;
	}
	// Crear texture a partir del surface
	_texture = SDL_CreateTextureFromSurface(rnd, surface);

	// Liberar recursos del surface
	SDL_FreeSurface(surface);
	if (_texture == NULL) {
		delete[] pixelDat;
		return false;
	}
	delete[] pixelDat;

	return true;
}

ImagePC::~ImagePC()
{
	if (_texture != nullptr)
		SDL_DestroyTexture(_texture);
}

int ImagePC::getWidth() const
{
	return _width;
}

int ImagePC::getHeight() const
{
	return _height;
}
