#include "Vector2.h"
#include <math.h>

Vector2::Vector2() {
	x = 0; y = 0;
}

Vector2::Vector2(float x, float y) : x(x), y(y) {}

float Vector2::magnitude() {
	float r = x * x + y * y;
	return sqrt(r);
}

float Vector2::distance(const Vector2& other) {
	Vector2 dist = *this - other;
	return dist.magnitude();
}

void Vector2::normalize() {
	float m = magnitude();
	x /= m;
	y /= m;
}

void Vector2::setX(float x) {
	this->x = x;
}

void Vector2::setY(float y) {
	this->y = y;
}

float Vector2::getX() const {
	return this->x;
}

float Vector2::getY() const {
	return this->y;
}

// -------------- Operadores ---------------

Vector2 Vector2::operator+(const Vector2& other) {
	return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) {
	return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator+=(const Vector2& other) {
	x += other.x;
	y += other.y;
	return *this;
}

Vector2 Vector2::operator-=(const Vector2& other) {
	x -= other.x;
	y -= other.y;
	return *this;
}

Vector2 Vector2::operator*(float c) {
	return Vector2(x * c, y * c);
}

Vector2 Vector2::operator*=(float c) {
	x *= c;
	y *= c;
	return *this;
}
