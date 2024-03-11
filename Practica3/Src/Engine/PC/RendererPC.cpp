#include "RendererPC.h"
#include <SDL.h>
#include <assert.h>
#include <iostream>
#include "ImagePC.h"

RendererPC* RendererPC::_instance = nullptr;

RendererPC::RendererPC(int width, int height) : _width(width), _height(height), _renderer(nullptr), _window(nullptr)
{
}

RendererPC::~RendererPC()
{
	if (_renderer)
	{
		SDL_DestroyRenderer(_renderer);
		_renderer = nullptr;
	}

	if (_window)
	{
		SDL_DestroyWindow(_window);
		_window = nullptr;
	}
}

//Inicializacion del Renderer con el ancho y alto de la ventana usando SDL CreateWindow
bool RendererPC::initPrivate(int winWidth, int winHeight, std::string name)
{
	//Create window
	_window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winWidth, winHeight, SDL_WINDOW_SHOWN); // SDL_WINDOW_FULLSCREEN

	if (_window == NULL)
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	else
	{
		std::cout << "Window created!" << std::endl;
	}

	//Create renderer for window
	_renderer = SDL_CreateRenderer(_window, -1, VSYNC ? SDL_RENDERER_PRESENTVSYNC : SDL_RENDERER_ACCELERATED);

	if (_renderer == NULL)
	{
		std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		// Si falla la creación del renderer, destruye la ventana previamente creada
		SDL_DestroyWindow(_window);
		_window = nullptr;
		return false;
	}
	else
	{
		std::cout << "Renderer created!" << std::endl;
	}

	//Set BlendMode
	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

	return true;
}

void RendererPC::setUpSDLColor(uint64_t color)
{
	uint8_t _a = (color >> 24) & 0xFF;
	uint8_t _r = (color >> 16) & 0xFF;
	uint8_t _g = (color >> 8) & 0xFF;
	uint8_t _b = (color >> 0) & 0xFF;
	SDL_SetRenderDrawColor(_renderer, _r, _g, _b, _a);
}

bool RendererPC::Init(int winWidth, int winHeight, std::string name)
{
	assert(_instance == nullptr);
	_instance = new RendererPC(winWidth, winHeight);

	if (!_instance->initPrivate(winWidth, winHeight, name))
	{
		// Si la inicialización privada falla, libera todos los recursos y destruye la instancia
		Release();
		return false;
	}

	return true;
}


//"Limpieza" de pantalla con un color (El tipo es ARGB y se transforma RGBA)
void RendererPC::clear(uint32_t color)
{

	setUpSDLColor(color);
	SDL_RenderClear(_renderer);
}

//Dibujo de un pixel del color "color" (ARGB->RGBA) en la posicion X Y
void RendererPC::putPixel(int x, int y, uint32_t color)
{
	setUpSDLColor(color);

	SDL_RenderDrawPoint(_renderer, x, y);
}

//Dibujo de un rectangulo del color "color" (ARGB->RGBA) en la posicion X Y
void RendererPC::drawRect(int x, int y, int width, int height, uint32_t color, bool fill)
{
	setUpSDLColor(color);
	SDL_Rect Rect = { x, y, width, height };
	if (fill)
		SDL_RenderFillRect(_renderer, &Rect);
	else
		SDL_RenderDrawRect(_renderer, &Rect);
}

//Dibujo de un circulo con color , usando el algoritmo del punto medio
void RendererPC::drawCircle(int cX, int cY, int r, uint32_t color) // QUITAR ESCALA REVISAR
{
	int x = r;  // Inicializar la variable "x" con el radio del círculo
	int y = 0;  // Inicializar la variable "y" con 0
	int d = (int)((5.0 / 4.0) - r);  // Calcular el primer valor de decisión
	int h = (int)(d - (1.0 / 4.0));  // Calcular el primer valor de h, sirve para saber que pixeles vamos a dibujar
	// Iniciar un bucle mientras el valor de "x" sea mayor o igual a "y"
	// Dibujar píxeles en varios octantes del círculo y líneas horizontales
	while (x >= y) {
		drawLine(cX - x, cY + y, cX + x, cY + y, color); // Línea horizontal superior

		drawLine(cX - x, cY - y, cX + x, cY - y, color); // Línea horizontal inferior
		drawLine(cX - y, cY + x, cX + y, cY + x, color); // Línea horizontal superior

		drawLine(cX - y, cY - x, cX + y, cY - x, color); // Línea horizontal inferior

		// Actualizar el valor de "h" según las reglas del algoritmo de Bresenham
		if (h < 0) {
			h = h + 2 * y + 3;
		}
		else {
			h = h + (2 * y) + 3 + (-(2 * x)) + 2;
			x--;
		}

		y++; // Incrementar el valor de "y" para pasar al siguiente píxel en la columna

	}
}

//Dibujo de una linea  del color "color" (ARGB->RGBA) desde la posicion X Y hasta la posicion X2 Y2
void RendererPC::drawLine(int x1, int y1, int x2, int y2, uint32_t color)
{
	setUpSDLColor(color);

	//Dibuja Linea desde (x,y) a (x2,y2)
	SDL_RenderDrawLine(_renderer, x1, y1, x2, y2);
}

void RendererPC::present()
{
	//Update screen
	SDL_RenderPresent(_renderer);
}

//Getters para el ancho y alto de el Renderers

int RendererPC::getWidth() const
{
	return _width;
}

int RendererPC::getHeight() const
{
	return _height;
}

void RendererPC::drawImage(const ImagePC* image, int orgX, int orgY, int orgW, int orgH, int dstX, int dstY, int dstW, int dstH)
{
	SDL_Rect srcRect = { orgX, orgY, orgW, orgH };
	SDL_Rect dstRect = { dstX, dstY, dstW, dstH };

	SDL_RenderCopyEx(_renderer, image->getTexture(), &srcRect, &dstRect, 0, nullptr, SDL_FLIP_NONE);
}

ImagePC* RendererPC::createImage(int width, int height, uint32_t* pixelData)
{
	auto img = new ImagePC(width, height);
	if (img->createImage(pixelData, _renderer)) return img;
	return nullptr;

}

void RendererPC::releaseImage(ImagePC* img)
{
	delete img;
	img = nullptr;
}

//Devolver la instancia
RendererPC* RendererPC::Instance()
{
	return _instance;
}

//Liberar memoria
void RendererPC::Release()
{
	if (_instance != nullptr)
		delete _instance;
	_instance = nullptr;
}