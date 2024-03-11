#include "Sprite.h"
#include "Renderer.h"
#include "Image.h"

#include <assert.h>

Sprite::Sprite(const Image* image, Vector2 orgPos, int width, int height) {
	assert(image != nullptr);
	image_ = image;

	//Sprite en File
	_filePos = orgPos;
	_widthImage = width;
	_heightImage = height;
}

// Draws the sprite in the screen with center in (x,y)
const void Sprite::draw(Vector2 pos, int width, int height) const
{									//sx1, sy1, sx2, sy2, tx1, ty1, tx2, ty2
	Renderer::Instance()->drawImage(image_, _filePos.getX(), _filePos.getY(), _widthImage, _heightImage, pos.getX(), pos.getY(), width, height);
}

// Implementación de métodos de acceso
float Sprite::getXImage() const {
	return _filePos.getX();
}

float Sprite::getYImage() const {
	return _filePos.getY();
}

int Sprite::getWidthImage() const {
	return _widthImage;
}

int Sprite::getHeightImage() const {
	return _heightImage;
}
