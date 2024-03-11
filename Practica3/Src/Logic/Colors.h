#pragma once
#include <cstdint>


/// <summary>
/// Colores predefinidos en formato ARGB del tipo uint32_t
///El método Color::getColorArray() devuelve un array de estos colores predefinidos.
//El resto de metodos sirven para devolver un color en concreto
/// </summary>
struct Color { //ARGB
	static uint32_t Red() { return 0xffff0000; }
	static uint32_t Green() { return 0xff008000; }
	static uint32_t Blue() { return 0xff4040ff; }
	static uint32_t Background() { return 0xff202020; }
	static uint32_t Pink() { return 0xffdb7093; }
	static uint32_t White() { return 0xffffffff; }
	static const uint32_t* getColorArray() {
		static uint32_t colorArray[] = { Blue(),Red(), Green(),  Pink(), Background(),White() };
		return colorArray;
	}
};