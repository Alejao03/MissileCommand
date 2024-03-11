#pragma once
#include "Sprite.h"
#include "Vector2.h"

enum CommandType { CLEAR, DRAW_SPRITE, PRESENT, STOP, DRAW_RECT, DRAW_LINE, DRAW_CIRCLE };
/// <summary>
/// Parametros para pintar un sprite
/// </summary>
struct SpriteParam {
	const Sprite* sprite;
	Vector2 pos;
	int width;
	int height;
};
/// <summary>
/// Parametros necesarios para pintar una linea
/// </summary>
struct LineParam {
	Vector2 startPos;
	Vector2 pos;
	uint32_t color;
};
/// <summary>
/// Parametros necesarios para pintar un rectangulo
/// </summary>
struct RectParam {
	Vector2 pos;
	int width;
	int height;
	uint32_t color;
	bool fill;
};
/// <summary>
/// Parametros necesarios para pintar un circulo
/// </summary>
struct CircleParam {
	Vector2 pos;
	int r;
	uint32_t color;
};
struct Command {
public:
	 Command() {};
	CommandType type;
	union {
		uint32_t color; // CLEAR
		SpriteParam spriteParam; // DRAW_SPRITE		
		LineParam lineParam; // LINE_PARAM
		RectParam rectParam; // RECT_PARAM
		CircleParam circleParam;//CIRCLE_PARAM
	};
};