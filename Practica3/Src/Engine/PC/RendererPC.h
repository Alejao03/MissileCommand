#pragma once

#include <cstdint>

// Por alguna razon parte de esta clase la exporta como dependencia externa lo que nos impide hacer class ImagePC hablado con Pedro Pablo en clase
#include "ImagePC.h"
#include <string>
class SDL_Renderer;
class SDL_Window;

//FLIP MODE
const bool VSYNC = true;

class RendererPC
{
private:
	static RendererPC* _instance;
	SDL_Renderer* _renderer; 
	SDL_Window* _window;
	int _width;
	int _height;
	RendererPC(int height, int width);
	bool initPrivate(int winWidth, int winHeight,std::string name);
	void setUpSDLColor(uint64_t color);
	~RendererPC();

public:
	/// <summary>
	/// Pone toda la pantalla del color que se le pase
	/// </summary>
	/// <param name="color">Color que se desea pintar en HEX, debe estar en formato ARGB </param>
	void clear(uint32_t color);
	/// <summary>
	/// Método que pinta un pixel en la posicion x,y con el color que se le pasa 
	/// </summary>
	/// <param name="x">posicion en el eje horizontal</param>
	/// <param name="y">posicion en el eje vertical</param>
	/// <param name="color">Color que se desea pintar en HEX, debe estar en formato ARGB </param>
	void putPixel(int x, int y, uint32_t color);
	/// <summary>
	/// Método que pinta un rectangulo en la posicion x,y, con anchura y altura width,height y con el color que se le pasa 
	/// </summary>
	/// <param name="x">posicion en el eje horizontal</param>
	/// <param name="y">posicion en el eje vertical</param>
	/// <param name="width">anchura en pixeles del rectangulo</param>
	/// <param name="height">altura en pixeles del rectangulo</param>
	/// <param name="color">Color que se desea pintar en HEX, debe estar en formato ARGB </param>
	/// <param name="fill">Rellena el rectangulo de color</param>
	void drawRect(int x, int y, int width, int height, uint32_t color, bool fill);
	/// <summary>
	/// Método para pintar un circulo con centro en (cx,cy) con radio r y del color indicado
	/// </summary>
	/// <param name="cX">centro del circulo en el eje x</param>
	/// <param name="cY">centro del circulo en el eje y</param>
	/// <param name="r">radio del circulo</param>
	/// <param name="color">Color que se desea pintar en HEX, debe estar en formato ARGB </param>
	void  drawCircle(int cX, int cY, int r, uint32_t color);
	/// <summary>
	/// Pinta la linea del punto(X,Y) al punto (X2,Y2) con el color correspondiente
	/// </summary>
	/// <param name="x">posicion del punto incial en el eje x</param>
	/// <param name="y">posicion del punto incial en el eje y</param>
	/// <param name="x2">posicion del punto final en el eje x</param>
	/// <param name="y2">posicion del punto final en el eje y</param>
	/// <param name="color">Color que se desea pintar en HEX, debe estar en formato ARGB </param>
	void drawLine(int x1, int y1, int x2, int y2, uint32_t color);
	/// <summary>
	/// Renderiza el frame 
	/// </summary>
	void present();
	/// <summary>
	/// Obtiene el ancho
	/// </summary>
	/// <returns>ancho</returns>
	int getWidth() const;
	/// <summary>
	/// Obtiene el alto
	/// </summary>
	/// <returns>alto</returns>
	int getHeight() const;
	/// <summary>
	/// Dibuja el fragmento de la imagen en el destino
	/// </summary>
	/// <param name="image">Imagen completa</param>
	/// <param name="orgX">cordenada x de la esquina superior izquierda de la posicion en la imagen desde la que se desea empezar</param>
	/// <param name="orgY">cordenada y de la esquina superior izquierda de la posicion en la imagen desde la que se desea empezar</param>
	/// <param name="orgW">ancho de la imagen que deseamos tomar</param>
	/// <param name="orgH">alto de la imagen que deseamos tomar</param>
	/// <param name="dstX">cordenada x de la esquina superior izquierda de la posicion de la pantalla en la que se desea empezar</param>
	/// <param name="dstY">cordenada y de la esquina superior izquierda de la posicion de la pantalla en la que se desea empezar</param>
	/// <param name="dstW">ancho de la zona de la pantalla en la que se desea pintar</param>
	/// <param name="dstH">alto de la zona de la pantalla en la que se desea pintar</param>
	void drawImage(const ImagePC* image, int orgX, int orgY, int orgW, int orgH, int dstX, int dstY, int dstW, int dstH);
	/// <summary>
	/// Crea un objeto de tipo imagen con un alto y un ancho
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="pixelData"></param>
	/// <returns></returns>
	ImagePC* createImage(int width, int height, uint32_t* pixelData);
	/// <summary>
	/// Libera la memoria de la imagen
	/// </summary>
	/// <param name="img">imagen a eliminar</param>
	void releaseImage(ImagePC* img);
	/// <summary>
	/// Inicializa el Render de la plataforma 
	/// </summary>
	/// <param name="winWidth">ancho si no se indica sera 1920</param>
	/// <param name="winHeight">alto si no se indica sera 1080</param>
	/// <param name="name">Nombre de la ventana del juego</param>
	/// <returns>true si se ha podido inicializar, false en caso contrario</returns>
	static bool Init(int winWidth = 1920, int winHeight = 1080,std::string name="Juego");
	/// <summary>
	/// Método que libera la memoria que haya creado el renderer
	/// </summary>
	static void Release();
	/// <summary>
	/// 
	/// </summary>
	/// <returns>puntero a la instancia de la plataforma, nullptr si no existe </returns>
	static RendererPC* Instance();
};
