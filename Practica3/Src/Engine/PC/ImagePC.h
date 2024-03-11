#pragma once
#include <cstdio>
#include <vector>

struct SDL_Texture;
struct SDL_Renderer;

class ImagePC
{
	int _width;
	int _height;
	SDL_Texture* _texture;
	/// <summary>
	/// devuelve la textura de la imagen
	/// </summary>
	/// <returns></returns>
	SDL_Texture* getTexture() const;
public:
	/// <summary>
	/// Constructor de la clase imagen 
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>

	ImagePC(int width, int height);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="pixelDat">se elimina este parametro</param>
	/// <param name="rnd"></param>
	/// <returns></returns>
	bool createImage(uint32_t* pixelDat, SDL_Renderer* rnd);

	/// <summary>
	/// Destructor de la clase imagen 
	/// </summary>
	~ImagePC();
	/// <summary>
	/// ancho de la imagen
	/// </summary>
	/// <returns></returns>
	int getWidth() const;
	/// <summary>
	/// Alto de la imagen
	/// </summary>
	/// <returns></returns>
	int getHeight() const;
	friend class RendererPC;
};