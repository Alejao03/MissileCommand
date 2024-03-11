#pragma once

#include <iostream>
#include <fstream>
#include "Image.h"
#include "Vector2.h"

// facilitar el control de los sprites, entendidos como una porción de una imagen,
class Sprite
{
private:
	//Guardar Imagen
	const Image* image_;
	//Sprite en File
	Vector2 _filePos;
	int _widthImage, _heightImage;
	//Elementos de escala
	Vector2 _scaledPos;
	float scale_;
	int _scaledWidth;
	int _scaledHeight;

public:
	/// <summary>
	/// Constructor de clase Sprite
	/// </summary>
	/// <param name="image">Imagen de la que se obtiene el sprite</param>
	/// <param name="orgX">cordenada horizontal de la esquina superior izquierda desde la que empieza el sprite en la imagen</param>
	/// <param name="orgY">cordenada vertical de la esquina superior izquierda desde la que empieza el sprite en la imagen</param>
	/// <param name="width">ancho del sprite en la imagen proporcionada</param>
	/// <param name="height">alto del sprite en la imagen proporcionada</param>
	Sprite(const Image* image, Vector2 orgPos, int width, int height);
	/// <summary>
	/// Dibuja el sprite en la posicion deseada
	/// </summary>
	/// <param name="x">posicion horizontal de la esquina superior izquierda de la pantalla en la que se pintara el sprite </param>
	/// <param name="y">posicion vertical de la esquina superior izquierda de la pantalla en la que se pintara el sprite</param>
	/// <param name="width">ancho que el sprite va a tener en la pantalla</param>
	/// <param name="height">alto que el sprite va a tener en la pantalla</param>
	/// <returns></returns>
	const void draw(Vector2 pos, int width, int height) const;
	/// <summary>
	/// cordenada horizontal de la esquina superior izquierda desde la que empieza el sprite en la imagen
	/// </summary>
	/// <returns></returns>
	float getXImage() const;
	/// <summary>
	/// cordenada vertical de la esquina superior izquierda desde la que empieza el sprite en la imagen
	/// </summary>
	/// <returns></returns>
	float getYImage() const;
	/// <summary>
	/// ancho del sprite en la imagen proporcionada
	/// </summary>
	/// <returns></returns>
	int getWidthImage() const;
	/// <summary>
	/// alto que el sprite va a tener en la pantalla
	/// </summary>
	/// <returns></returns>
	int getHeightImage() const;
};
